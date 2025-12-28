#ifndef PLANES_NEARBY_H
#define PLANES_NEARBY_H

#include "ModuleInterface.h"
#include "Config.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

class PlanesNearby : public ModuleInterface {
private:
    unsigned long lastUpdate;
    unsigned long updateInterval;
    bool enabled;
    int planeCount;
    float nearestPlaneDistance;
    String nearestPlaneCallsign;
    
public:
    PlanesNearby() : 
        lastUpdate(0), 
        updateInterval(PLANES_UPDATE_INTERVAL),
        enabled(ENABLE_PLANES_NEARBY),
        planeCount(0),
        nearestPlaneDistance(9999.9),
        nearestPlaneCallsign("") {}
    
    bool begin() override {
        if (!enabled) {
            Serial.println("[PlanesNearby] Module disabled");
            return false;
        }
        Serial.println("[PlanesNearby] Initializing...");
        return true;
    }
    
    void update() override {
        if (!enabled || !needsUpdate()) return;
        
        Serial.println("[PlanesNearby] Fetching nearby aircraft...");
        
        // Fetch from OpenSky Network or similar ADS-B API
        HTTPClient http;
        String url = "https://opensky-network.org/api/states/all?lamin=" + 
                     String(LATITUDE - 0.5) + "&lomin=" + String(LONGITUDE - 0.5) +
                     "&lamax=" + String(LATITUDE + 0.5) + "&lomax=" + String(LONGITUDE + 0.5);
        
        http.begin(url);
        int httpCode = http.GET();
        
        if (httpCode == 200) {
            String payload = http.getString();
            DynamicJsonDocument doc(4096);
            deserializeJson(doc, payload);
            
            JsonArray states = doc["states"];
            planeCount = states.size();
            
            nearestPlaneDistance = 9999.9;
            for (JsonVariant state : states) {
                float lat = state[6];
                float lon = state[5];
                String callsign = state[1].as<String>();
                
                // Calculate distance (simplified)
                float distance = sqrt(pow(lat - LATITUDE, 2) + pow(lon - LONGITUDE, 2)) * 111.0;
                
                if (distance < nearestPlaneDistance) {
                    nearestPlaneDistance = distance;
                    nearestPlaneCallsign = callsign;
                }
            }
            
            Serial.printf("[PlanesNearby] Found %d planes, nearest: %s (%.2f km)\n", 
                         planeCount, nearestPlaneCallsign.c_str(), nearestPlaneDistance);
        } else {
            Serial.printf("[PlanesNearby] HTTP Error: %d\n", httpCode);
        }
        
        http.end();
        lastUpdate = millis();
    }
    
    bool isEnabled() const override {
        return enabled;
    }
    
    const char* getName() const override {
        return "PlanesNearby";
    }
    
    bool needsUpdate() const override {
        return enabled && (millis() - lastUpdate >= updateInterval);
    }
    
    unsigned long getLastUpdate() const override {
        return lastUpdate;
    }
    
    int getPlaneCount() const {
        return planeCount;
    }
    
    float getNearestDistance() const {
        return nearestPlaneDistance;
    }
    
    String getNearestCallsign() const {
        return nearestPlaneCallsign;
    }
};

#endif // PLANES_NEARBY_H
