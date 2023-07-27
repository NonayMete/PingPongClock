#pragma once
//Stores non-volatile preferences and settings
#include <Preferences.h>


//use defines for variable names so keys can be short
#define SETTING_WIFI_SSID   "001"
#define SETTING_WIFI_PASS   "002"

#define SETTING_24_TIME     "100"
#define SETTING_BRIGHTNESS  "101"
#define SETTING_BACKGROUND  "102"
#define SETTING_BACKGROUND_COLOR  "103"

enum Backgrounds {
    Rainbow,
    ColourSolid,
    ColourGradient,
    Fire,
    Black
};

class Settings {
    public:
        Settings() {
            preferences.begin("settings", false);
            //read all settings from NVS
            Serial.println("Loading settings...");
            is24HrTime = preferences.getBool(SETTING_24_TIME, false);
            brightness = preferences.getUInt(SETTING_BRIGHTNESS, 50);
            background = preferences.getUInt(SETTING_BACKGROUND, Backgrounds::ColourSolid);
            background_color = preferences.getULong(SETTING_BACKGROUND_COLOR, CRGB::Red);
            preferences.end();
        }
        bool get24HrTime() {
            return is24HrTime;
        }
        void set24HrTime(bool value) {
            if (value == is24HrTime) return;
            is24HrTime = value;
            needsSave = true;
        }

        uint getBrightness() {
            return brightness;
        }
        void setBrightness(uint value) {
            if (value == brightness) return;
            brightness = value;
            needsSave = true;
        }

        uint getBackground() {
            return background;
        }
        void setBackground(uint value) {
            if (value == background) return;
            background = value;
            needsSave = true;
        }

        unsigned long getBackgroundColor() {
            return background_color;
        }
        
        void setBackgroundColor(unsigned long value) {
            if (value == background_color) return;
            background_color = value;
            needsSave = true;
        }


        void saveSettings() {
            if (!needsSave) return;
            preferences.begin("settings", false);

            preferences.putBool(SETTING_24_TIME, is24HrTime);
            preferences.putUInt(SETTING_BRIGHTNESS, brightness);
            preferences.putUInt(SETTING_BACKGROUND, background);
            preferences.putULong(SETTING_BACKGROUND_COLOR, background_color);

            preferences.end();
            needsSave = false;
            Serial.println("Settings saved");
        }
    private:
        Preferences preferences;
        bool needsSave = false;

        //settings
        bool is24HrTime;
        uint brightness;
        uint background;
        unsigned long background_color;
};

//make settings instance
Settings settings;

void saveSettings(void * parameter) {
  //check if settings need to be saved to NVS every approx 1 seconds
  for(;;) {
    settings.saveSettings();
    delay(1000);
  }
}