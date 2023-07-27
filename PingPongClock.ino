//#include <FreeRTOS.h>
#include <FastLED.h>
#include "time.h"
#include "Scheduler.h"
#include "leds.h"
#include "config.h"
#include "WebInterface.h"
#include "GUI.h"


#define DATA_PIN 23  // The pin connected to the Data input on your strip

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 28800; // adjust to your timezone in seconds
const int   daylightOffset_sec = 0; // adjust for daylight saving time

TaskHandle_t TaskHandle_1;

void setup() {
  Serial.begin(115200);
  settings = Settings();

  Serial.println("24hr time: " + String(settings.get24HrTime()));
  Serial.println("Brightness: " + String(settings.getBrightness()));

  //setup webserver + OTA programming
  otaInit(); 

  //initialise LED's
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(map(settings.getBrightness(), 0, 100, 0, 255));

  //Init background time grabber
  xTaskCreatePinnedToCore(
                    updateTime,  /* Function to implement the task */
                    "updateTime", /* Name of the task */
                    3000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    0,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    0);         /* Core where the task should run */
  
  //Init websocket LED GUI updater
  xTaskCreatePinnedToCore(updateWebPage, "updateWebPage", 2100, NULL, 0, NULL, 0);
  
  //Init NVS settings storage
  xTaskCreatePinnedToCore(saveSettings, "saveSettings", 3000, NULL, 0, &TaskHandle_1, 0);

  for(int row=0; row<CLOCK_ROWS; row++) {
    for(int col=0; col<CLOCK_COLS; col++) {
      heat[row][col] = 0;
    }
  }
}

void loop() {
  static Scheduler processFrame = Scheduler::frequency(20); //20 Hz
  static Scheduler printOut = Scheduler::msec(2000);

  long frame_start = millis();
  
  //update brightness
  FastLED.setBrightness(map(settings.getBrightness(), 0, 100, 0, 255));

  //fill background
  switch (settings.getBackground())
  {
  case Backgrounds::ColourSolid:
    fill_solid(leds, NUM_LEDS, settings.getBackgroundColor());
    break;
  case Backgrounds::ColourGradient:
    fill_gradient_RGB(leds, NUM_LEDS, settings.getBackgroundColor(), CRGB::Black);
    break;
    
  case Backgrounds::Rainbow:
    rainbow();
    break;
  case Backgrounds::Fire:
    //Fire2012WithPalette();
    break;
  case Backgrounds::Black:
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    break;
  default:
    break;
  };
  
  //update digits to current time value
  updateDigits();
  displayDigits(CRGB::White);

  //blink dots off/on
  updateDots(CRGB::White);

  if(printOut.shouldRun()) {
    Serial.println("ping");
    // Then you can use the uxTaskGetStackHighWaterMark function like this:
    UBaseType_t uxHighWaterMark;
    uxHighWaterMark = uxTaskGetStackHighWaterMark( TaskHandle_1 );
    Serial.print("Stack high water mark: ");
    Serial.println(uxHighWaterMark);
  }
  
  //update LED's and wait for next frame
  FastLED.show();
  processFrame.wait(true); //warn if we're running behind
}

void displayDigits(CRGB color) {
  for(int i=0; i<10; i++) {
    if(digit1[i].on) {
      leds[digit1[i].index] = color;
    }
    if(digit2[i].on) {
      leds[digit2[i].index] = color;
    }
    if(digit3[i].on) {
      leds[digit3[i].index] = color;
    }
    if(digit4[i].on) {
      leds[digit4[i].index] = color;
    }
  }
}

void updateDots(CRGB color) {
  time_t now;
  struct tm timeinfo;
  time(&now);
  localtime_r(&now, &timeinfo);
  int sec = timeinfo.tm_sec;
  if(sec%2) {
    dots[0].on = false;
    dots[1].on = false;
  } else {
    dots[0].on = true;
    dots[1].on = true;
  }
  for(int i=0; i < 2; i++) {
    if(dots[i].on) leds[dots[i].index] = color;
  }
}

void updateDigits() {
  time_t now;
  char strftime_buf[64];
  struct tm timeinfo;
  time(&now);
  localtime_r(&now, &timeinfo);
  int hr = timeinfo.tm_hour;
  if (settings.get24HrTime() == false) {
    hr = hr % 12;
    if (hr == 0) {
      hr = 12;
    }
  }
  setDigitTo(digit2, int(hr%10));
  hr /= 10;
  setDigitTo(digit1, hr);
  int min = timeinfo.tm_min;
  setDigitTo(digit4, int(min%10));
  min /= 10;
  setDigitTo(digit3, min);
}

void updateTime(void * parameter) {
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  for(;;) {
    if(!getLocalTime(&timeinfo)) {
      WebSerial.println("Failed to obtain time");
    } else {
      Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
      WebSerial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    }
    delay(10 * 1000);
  }
}