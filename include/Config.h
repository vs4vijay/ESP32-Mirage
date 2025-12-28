#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define WIFI_TIMEOUT_MS 20000

// Display Configuration
#define TFT_CS 5
#define TFT_RST 17
#define TFT_DC 16
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240

// Module Enable/Disable Configuration
#define ENABLE_SATELLITE_IMAGE_CLOCK true
#define ENABLE_PAX_COUNTER true
#define ENABLE_PLANES_NEARBY true
#define ENABLE_WEATHER true
#define ENABLE_AIR_QUALITY true
#define ENABLE_TRAFFIC true
#define ENABLE_NEWS true
#define ENABLE_SOUND_ALERTS true
#define ENABLE_CALENDAR true
#define ENABLE_ASTRONOMICAL_EVENTS true

// Update Intervals (milliseconds)
#define SATELLITE_IMAGE_UPDATE_INTERVAL 3600000  // 1 hour
#define PAX_COUNTER_UPDATE_INTERVAL 60000        // 1 minute
#define PLANES_UPDATE_INTERVAL 30000             // 30 seconds
#define WEATHER_UPDATE_INTERVAL 1800000          // 30 minutes
#define AQI_UPDATE_INTERVAL 3600000              // 1 hour
#define TRAFFIC_UPDATE_INTERVAL 300000           // 5 minutes
#define NEWS_UPDATE_INTERVAL 3600000             // 1 hour
#define CALENDAR_UPDATE_INTERVAL 3600000         // 1 hour
#define ASTRONOMICAL_UPDATE_INTERVAL 86400000    // 24 hours

// API Configuration (user must set these)
#define SATELLITE_API_KEY ""
#define WEATHER_API_KEY ""
#define AQI_API_KEY ""
#define TRAFFIC_API_KEY ""
#define NEWS_API_KEY ""
#define CALENDAR_API_KEY ""
#define PLANES_API_KEY ""
#define ASTRONOMICAL_API_KEY ""

// Location Configuration
#define LATITUDE 0.0
#define LONGITUDE 0.0
#define LOCATION_NAME "Unknown"

// Sound Alert Configuration
#define BUZZER_PIN 25
#define ALERT_PLANE_DISTANCE_KM 5.0
#define ALERT_WEATHER_SEVERITY 3
#define ALERT_AQI_THRESHOLD 150

// Display Mode
#define DISPLAY_ROTATION 0
#define TIME_FORMAT_24H true

#endif // CONFIG_H
