# ESP32-Mirage

ESP32-based satellite image clock and environmental monitoring system with modular, configurable features.

## Features

ESP32-Mirage is a comprehensive IoT display system that provides real-time information about your environment and the world around you:

### Core Features

- **🛰️ Satellite Image Clock**: Display real-time satellite imagery as background with current time and date
- **👥 PAX Counter**: Count nearby people using WiFi/BLE device detection
- **✈️ Planes Nearby**: Track aircraft in your area using ADS-B data
- **🌤️ Weather Information**: Current conditions and 3-day forecast
- **🌫️ Air Quality Index (AQI)**: Real-time air quality monitoring with PM2.5 and PM10 levels
- **🚗 Live Traffic Information**: Traffic conditions and estimated delays
- **📰 Daily News/Headlines**: Top news stories from around the world
- **🔔 Sound Alerts**: Audio notifications when thresholds are met (plane proximity, severe weather, poor air quality)
- **📅 Calendar**: Display upcoming events from your calendar
- **🌠 Astronomical Events**: Track upcoming eclipses, meteor showers, and other celestial events

### Architecture

ESP32-Mirage uses a **fully modular architecture** where each feature is implemented as an independent module that can be:
- ✅ Enabled or disabled via configuration
- ⚙️ Configured with custom update intervals
- 🔌 Added, removed, or modified without affecting other modules

## Hardware Requirements

- **ESP32 Development Board** (ESP32-DevKitC or similar)
- **TFT Display** (ST7735/ST7789 or compatible, typically 240x240 or 240x320)
- **Buzzer/Speaker** (optional, for sound alerts)
- **WiFi Connection** (for data fetching)

### Wiring Diagram

```
ESP32          TFT Display
-----          -----------
GPIO 5    -->  CS
GPIO 17   -->  RST
GPIO 16   -->  DC
GPIO 23   -->  MOSI (SDA)
GPIO 18   -->  SCK
3.3V      -->  VCC
GND       -->  GND

GPIO 25   -->  Buzzer (optional)
```

## Software Setup

### Using PlatformIO (Recommended)

1. **Install PlatformIO** (VS Code extension or CLI)

2. **Clone the repository**:
   ```bash
   git clone https://github.com/vs4vijay/ESP32-Mirage.git
   cd ESP32-Mirage
   ```

3. **Configure your settings** in `include/Config.h`:
   - Set your WiFi credentials (or use WiFiManager)
   - Add API keys for enabled services
   - Set your location (latitude/longitude)
   - Configure module enable/disable flags
   - Adjust update intervals

4. **Build and upload**:
   ```bash
   pio run --target upload
   ```

5. **Monitor serial output**:
   ```bash
   pio device monitor
   ```

### Using Arduino IDE

1. **Install ESP32 board support** in Arduino IDE
2. **Install required libraries**:
   - Adafruit GFX Library
   - Adafruit ST7735/ST7789 Library
   - ArduinoJson
   - WiFiManager

3. **Open** `src/main.cpp` in Arduino IDE
4. **Configure** settings in `include/Config.h`
5. **Select** your ESP32 board and port
6. **Upload** the sketch

## Configuration

### Module Configuration

Each module can be enabled/disabled in `include/Config.h`:

```cpp
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
```

### Update Intervals

Customize how often each module fetches new data:

```cpp
// Update Intervals (milliseconds)
#define SATELLITE_IMAGE_UPDATE_INTERVAL 3600000  // 1 hour
#define PAX_COUNTER_UPDATE_INTERVAL 60000        // 1 minute
#define PLANES_UPDATE_INTERVAL 30000             // 30 seconds
#define WEATHER_UPDATE_INTERVAL 1800000          // 30 minutes
// ... etc
```

### API Keys

Most modules require API keys from their respective services:

- **Satellite Images**: NASA EOSDIS, Himawari-8, or custom satellite API
- **Weather**: [OpenWeatherMap](https://openweathermap.org/api)
- **Air Quality**: [WAQI](https://aqicn.org/api/)
- **Traffic**: [Google Maps API](https://developers.google.com/maps)
- **News**: [NewsAPI](https://newsapi.org/)
- **Calendar**: [Google Calendar API](https://developers.google.com/calendar)
- **Planes**: [OpenSky Network](https://opensky-network.org/apidoc/) (free, no key required)

See `config.example.json` for a complete configuration template.

## Module Details

### SatelliteImageClock
Fetches real-time satellite imagery and displays it as background with current time/date overlay.

### PaxCounter
Uses WiFi promiscuous mode to detect nearby devices and estimate people count.

### PlanesNearby
Connects to ADS-B APIs (like OpenSky Network) to show nearby aircraft with distance and callsign.

### Weather
Provides current temperature, humidity, conditions, and 3-day forecast.

### AirQuality
Displays AQI with categorization (Good, Moderate, Unhealthy, etc.) and particulate matter levels.

### Traffic
Shows real-time traffic conditions with estimated delays for your commute.

### News
Fetches top headlines from news APIs.

### SoundAlerts
Triggers audio alerts based on configurable thresholds:
- Plane within specified distance
- Severe weather predicted
- Air quality exceeds threshold

### Calendar
Displays upcoming events from your calendar.

### AstronomicalEvents
Shows upcoming eclipses, meteor showers, and other celestial events.

## Adding New Modules

The modular architecture makes it easy to add new features:

1. **Create a new module class** inheriting from `ModuleInterface`
2. **Implement required methods**: `begin()`, `update()`, `isEnabled()`, etc.
3. **Add module to main.cpp** in the modules array
4. **Add configuration** in `Config.h`
5. **Done!** Your module will be automatically managed by the system

Example module template:

```cpp
#include "ModuleInterface.h"

class MyNewModule : public ModuleInterface {
private:
    unsigned long lastUpdate;
    bool enabled;
    
public:
    MyNewModule() : lastUpdate(0), enabled(true) {}
    
    bool begin() override {
        // Initialize your module
        return true;
    }
    
    void update() override {
        if (!enabled || !needsUpdate()) return;
        // Fetch and update data
        lastUpdate = millis();
    }
    
    bool isEnabled() const override { return enabled; }
    const char* getName() const override { return "MyNewModule"; }
    bool needsUpdate() const override { 
        return enabled && (millis() - lastUpdate >= updateInterval); 
    }
    unsigned long getLastUpdate() const override { return lastUpdate; }
};
```

## Troubleshooting

### WiFi Connection Issues
- Ensure correct SSID and password in `Config.h`
- The device creates a WiFi access point "ESP32-Mirage" if it can't connect
- Connect to this AP and configure WiFi through the web interface

### Module Not Updating
- Check that the module is enabled in `Config.h`
- Verify API keys are correct
- Check serial monitor for error messages
- Some APIs have rate limits - adjust update intervals

### Display Not Working
- Verify pin connections
- Check display type in `platformio.ini` library configuration
- Adjust `DISPLAY_ROTATION` in `Config.h` if needed

### API Errors
- Most APIs require free registration for API keys
- Some APIs have rate limits or require paid plans for frequent updates
- Check API documentation for usage limits

## Contributing

Contributions are welcome! Here are some ways to contribute:

- 🐛 Report bugs and issues
- 💡 Suggest new modules or features
- 🔧 Submit pull requests with improvements
- 📖 Improve documentation
- 🎨 Create display themes or layouts

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- OpenSky Network for free aircraft tracking data
- OpenWeatherMap for weather data
- Various open-source libraries that make this project possible

## Roadmap

- [ ] Add web interface for configuration
- [ ] Implement multiple display themes
- [ ] Add MQTT support for home automation integration
- [ ] Create mobile app for remote monitoring
- [ ] Add data logging and historical graphs
- [ ] Support for multiple display types
- [ ] OTA (Over-The-Air) updates

## Support

For issues, questions, or suggestions, please [open an issue](https://github.com/vs4vijay/ESP32-Mirage/issues) on GitHub.