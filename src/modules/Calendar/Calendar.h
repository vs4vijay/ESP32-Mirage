#ifndef CALENDAR_H
#define CALENDAR_H

#include "ModuleInterface.h"
#include "Config.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

class Calendar : public ModuleInterface {
private:
    unsigned long lastUpdate;
    unsigned long updateInterval;
    bool enabled;
    String events[5];
    String eventTimes[5];
    int eventCount;
    
public:
    Calendar() : 
        lastUpdate(0), 
        updateInterval(CALENDAR_UPDATE_INTERVAL),
        enabled(ENABLE_CALENDAR),
        eventCount(0) {}
    
    bool begin() override {
        if (!enabled) {
            Serial.println("[Calendar] Module disabled");
            return false;
        }
        Serial.println("[Calendar] Initializing...");
        return true;
    }
    
    void update() override {
        if (!enabled || !needsUpdate()) return;
        
        Serial.println("[Calendar] Fetching upcoming events...");
        
        // Fetch from Google Calendar API or similar
        HTTPClient http;
        String url = "https://www.googleapis.com/calendar/v3/calendars/primary/events?key=" + 
                     String(CALENDAR_API_KEY) + "&timeMin=" + getCurrentTimeISO() +
                     "&maxResults=5&orderBy=startTime&singleEvents=true";
        
        http.begin(url);
        http.addHeader("Authorization", "Bearer " + String(CALENDAR_API_KEY));
        int httpCode = http.GET();
        
        if (httpCode == 200) {
            String payload = http.getString();
            DynamicJsonDocument doc(8192);
            deserializeJson(doc, payload);
            
            JsonArray items = doc["items"];
            eventCount = 0;
            
            for (JsonVariant item : items) {
                if (eventCount >= 5) break;
                events[eventCount] = item["summary"].as<String>();
                eventTimes[eventCount] = item["start"]["dateTime"].as<String>();
                eventCount++;
            }
            
            Serial.printf("[Calendar] Fetched %d upcoming events\n", eventCount);
            for (int i = 0; i < eventCount; i++) {
                Serial.printf("  %d. %s at %s\n", i+1, events[i].c_str(), eventTimes[i].c_str());
            }
        } else {
            Serial.printf("[Calendar] HTTP Error: %d\n", httpCode);
        }
        
        http.end();
        lastUpdate = millis();
    }
    
    String getCurrentTimeISO() {
        // Return current time in ISO format
        // For simplicity, return a placeholder
        return "2024-01-01T00:00:00Z";
    }
    
    bool isEnabled() const override {
        return enabled;
    }
    
    const char* getName() const override {
        return "Calendar";
    }
    
    bool needsUpdate() const override {
        return enabled && (millis() - lastUpdate >= updateInterval);
    }
    
    unsigned long getLastUpdate() const override {
        return lastUpdate;
    }
    
    int getEventCount() const {
        return eventCount;
    }
    
    String getEvent(int index) const {
        if (index >= 0 && index < eventCount) {
            return events[index];
        }
        return "";
    }
    
    String getEventTime(int index) const {
        if (index >= 0 && index < eventCount) {
            return eventTimes[index];
        }
        return "";
    }
};

#endif // CALENDAR_H
