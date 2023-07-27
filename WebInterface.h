#pragma once
#include "GUI.h"

// Allow OTA software updates
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

//############# CONFIG ############## //

//Use the inbuilt esp32 SPIFFS file system
//#define USE_SPIFFS

//default port
#define WEB_SERVER_PORT 80 

//WIFI Serial Monitor
// use WebSerial.println("");
// WebSerial is accessible at "<IP Address>/webserial" in browser
#define USE_WEBSERIAL

//after starting server halts program until IO0 is pulled LOW
//#define BLOCKMODE

//join existing network (STATION)
#define STATIONMODE
//or create own network (ACCESS POINT)
//#define ACCESSPOINTMODE

//use mDNS to assign a name to the esp32 on its network
//address will be http://hostname.local/
#define USE_MDNS
#if defined(USE_MDNS)
  const char *hostname = "clock";
#endif

//set esp32 to have a static IP address
//#define STATIC_IP
#if defined(STATIC_IP)
  IPAddress local_IP(192, 168, 0, 160);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
#endif

//set username/password for OTA
//#define ENABLE_LOGIN
#if defined(ENABLE_LOGIN)
  const char *OTA_username = "admin";
  const char *OTA_password = "iesarc";
#endif

String projectTitle = "Ping Pong Clock";
String projectDesc =
    "<b>Lorkin CC</b> <p>Ping Pong Clock Interface</p>";
  
// wifi network ssid/password

// const char *SSID = "Wokwi-GUEST";
// const char *PASSWORD = "";
const char *SSID = "Telstra8A1D32";
const char *PASSWORD = "n4f3skdmwb";

//############# END OF CONFIG ############## //

#if defined(USE_MDNS)
  #include <ESPmDNS.h>
#endif

#if defined(USE_WEBSERIAL)
  #include "WebSerial.h"
#endif
AsyncWebServer server(WEB_SERVER_PORT);

#if defined(USE_SPIFFS)
  #include "SPIFFS.h"
#endif

//send a message from the webserial monitor to the board
void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
  //process received data here

}

// Convert the WiFi (error) response to a string we can understand
const char *wl_status_to_string(wl_status_t status)
{
    switch (status) {
    case WL_NO_SHIELD:
        return "WL_NO_SHIELD";
    case WL_IDLE_STATUS:
        return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL:
        return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED:
        return "WL_SCAN_COMPLETED";
    case WL_CONNECTED:
        return "WL_CONNECTED";
    case WL_CONNECT_FAILED:
        return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST:
        return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED:
        return "WL_DISCONNECTED";
    default:
        return "UNKNOWN";
    }
}

String getUptime() {
  unsigned long cM = millis();
  unsigned long sec = cM / 1000;
  unsigned long min = sec / 60;
  unsigned long hrs = min / 60;
  unsigned long days = hrs / 24;
  cM %= 1000; sec %= 60; min %= 60; hrs %= 24;
  return String(hrs)+":"+String(min)+":"+String(sec)+":"+String(cM);
}

String getChipTime() {
  time_t now;
  char strftime_buf[64];
  struct tm timeinfo;
  time(&now);
  // Set timezone to China Standard Time
  setenv("TZ", "GMT-8", 1);
  tzset();
  localtime_r(&now, &timeinfo);
  strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
  return String(strftime_buf);
}

String getChipFrequency() {
  return String("240") + " MHz";
}

// #ifdef __cplusplus
//   extern "C" {
// #endif
//   uint8_t temprature_sens_read();
// #ifdef __cplusplus
// }
// #endif
// uint8_t temprature_sens_read();
String getChipTemperature() {
  // return String((temprature_sens_read() - 32) / 1.8) + "°C";
  return "err";
}

String getLastReset() {
  String reset_reason = "";
  switch((uint16_t)esp_reset_reason()) {
    case ESP_RST_POWERON:
      reset_reason = "ESP_RST_POWERON";
      break;
    case ESP_RST_EXT:
      reset_reason = "ESP_RST_EXT";
      break;
    case ESP_RST_SW:
      reset_reason = "ESP_RST_SW";
      break;
    case ESP_RST_PANIC:
      reset_reason = "ESP_RST_PANIC";
      break;
    case ESP_RST_INT_WDT:
      reset_reason = "ESP_RST_INT_WDT";
      break;
    case ESP_RST_TASK_WDT:
      reset_reason = "ESP_RST_TASK_WDT";
      break;
    case ESP_RST_WDT:
      reset_reason = "ESP_RST_WDT";
      break;
    case ESP_RST_DEEPSLEEP:
      reset_reason = "ESP_RST_DEEPSLEEP";
      break;
    case ESP_RST_BROWNOUT:
      reset_reason = "ESP_RST_BROWNOUT";
      break;
    case ESP_RST_SDIO:
      reset_reason = "ESP_RST_SDIO";
      break;
    case ESP_RST_UNKNOWN:
    default:
      reset_reason = "ESP_RST_UNKNOWN";
      break;
  }
    return reset_reason +" | "+ String((uint16_t)esp_reset_reason());
}

String getRAM() {
    //warning 32 bit value
    return String((uint16_t)esp_get_free_heap_size()) +" / "+
    String((uint16_t)esp_get_minimum_free_heap_size()) + " Bytes";
}

String getSPIFFSInfo() {
  #if defined(USE_SPIFFS)
    return String(SPIFFS.usedBytes()) + " / " + String(SPIFFS.totalBytes()) + " Bytes";
  #else
    return "ERR SPIFFS NOT INITIALISED";
  #endif
}

String getHomepage() {
  String homepage = R"(
    <!DOCTYPE HTML>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <head><title>)" + String(projectTitle) + R"(</title>
    <style>
          .block {
            margin:20px 5px 20px 5px;border:1px;border-radius:5px;border:1px solid darkgray;background-color:white;
          }
          .block_inner {
            font-size:0.8em;background-color:#EEEEEE;padding:5px;border-radius:10px;
          }
    </style>              
    </head>
    <body style='font-size:1.1em;'>
      <h3>Ping Pong Clock Interface</h3>

      )" + String(getUI()) + R"(

      <p><a href="/webserial">📋 Web Serial</a></p><p></p>
      <p><a href="/update">↻ Update Firmware</a></p><p></p>
      <p><a href="/filesystem">📂 File System</a></p><p></p>
      <p><a href="" onclick="javascript:event.target.port=801">Router Config</a></p><p></p>

      <div style="display: table;">
        <div style="display: table-row">
          <div style="width: 400px; display: table-cell;"> 
            <fieldset style="margin:20px 5px 20px 5px;border:1px;border-radius:5px;border:1px solid darkgray;background-color:white">
              <legend style="font-size:1.5em;">💻 System:</legend>
              <div style="font-size:0.8em;background-color:#EEEEEE;padding:5px;border-radius:10px;">
                <p><b>Uptime: </b>)" + String(getUptime()) + R"(</p>
                <p><b>Chip Time: </b>)" + String(getChipTime()) + R"(</p>
                <p><b>Chip Frequency: </b>)" + String(getChipFrequency()) + R"(</p>
                <p><b>Chip Temperature: </b>)" + String(getChipTemperature()) + R"(</p>
                <p><b>Last Reset: </b>)" + String(getLastReset()) + R"(</p>
                <p><b>RAM free/min: </b>)" + String(getRAM()) + R"(</p>
                <p><b>SPIFFS used/max: </b>)" + String(getSPIFFSInfo()) + R"(</p>
              </div>
            </fieldset>
          </div>
          <div style="width: 400px; display: table-cell;">
            <fieldset style="margin:20px 5px 20px 5px;border:1px;border-radius:5px;border:1px solid darkgray;background-color:white">
              <legend style="font-size:1.5em;">📶 WiFi:</legend>
              <div style="font-size:0.8em;background-color:#EEEEEE;padding:5px;border-radius:10px;">
                <p><b>SSID: </b>)" + String(WiFi.SSID()) + R"(</p>
                <p><b>Channel: </b>)" + String(WiFi.channel()) + R"(</p>
                <p><b>IP Address: </b>)" + String(WiFi.localIP().toString()) + R"(</p>
                <p><b>Gateway: </b>)" + String(WiFi.gatewayIP().toString()) + R"(</p>
                <p><b>MAC Address: </b>)" + String(WiFi.macAddress()) + R"(</p>
                <p><b>RSSI: </b>)" + String(WiFi.RSSI()) + R"(</p>
              </div>
            </fieldset>
          </div>
        </div>
      </div>
    </body></html>
)";
  return homepage;
}

// This is what me must do to get it all running
void otaInit() {
    if (WiFi.status() == WL_CONNECTED) {
        WiFi.disconnect(true);
        Serial.printf("WiFi was previously connected - now disconnected\n");
    }
    delay(100);

    //#if defined(STATIONMODE)
    Serial.printf("Setting ESP32 as Station, no power savings, SSID %s\n", SSID);
    WiFi.mode(WIFI_STA);
    WiFi.persistent(false);
    WiFi.setAutoReconnect(true);
    WiFi.setSleep(false);
    WiFi.setHostname(hostname);
    
    #if defined(STATIC_IP)
      if (!WiFi.config(local_IP,gateway,subnet)) {
        Serial.println("STA Failed to configure");
      }
    #endif

    if (WiFi.begin(SSID, PASSWORD)) {
        Serial.printf("WiFi begin success.\n");
    }
    //WiFi.setTxPower(WIFI_POWER_19_5dBm);
    delay(300);

    Serial.printf("Connecting to WiFi .");
    uint8_t wifiStatus = WiFi.waitForConnectResult();

    // Successful connection?
    // if ((wl_status_t)wifiStatus != WL_CONNECTED){
    //     Serial.printf("WiFi Status: %s, exiting\n", wl_status_to_string((wl_status_t)wifiStatus));
    //     while (1);
    // }
    // Just some information about the Wifi
      Serial.printf(
          "\nRunning on SSID %s with IP %s, Channel %d, MAC address %s\n",
          (char *)WiFi.SSID().c_str(),
          (char *)(WiFi.localIP()).toString().c_str(),
          WiFi.channel(),
          (char *)WiFi.macAddress().c_str());

    // #elif defined(ACCESSPOINTMODE)
    // Serial.printf("Setting ESP32 as Access Point, no power savings, SSID %s\n", SSID);
    // WiFi.mode(WIFI_MODE_AP);
    // WiFi.softAP(SSID, PASSWORD);
    // Serial.print("Access Point IP: ");
    // Serial.print(WiFi.softAPIP());
    // Serial.print("\t MAC Address: ");
    // Serial.print(WiFi.macAddress());
    // Serial.println();
    // #endif

    #if defined(USE_MDNS)
      Serial.println("Starting MDNS Service");
      if(!MDNS.begin(hostname)) {
        Serial.println("Error starting mDNS");
      }
    #endif

    //start SPIFFS
    #if defined(USESPIFFS)
      if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
      }
    #endif

    //WIFI Serial Monitor
    WebSerial.begin(&server);
    WebSerial.msgCallback(recvMsg);

    // This gets printed when you type the IP address followed by /whoami
    server.on("/whoami", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", projectDesc); });
    
    //  server.on("/forceclean", HTTP_GET, [](AsyncWebServerRequest *request)
    //           { log_i("forcing clean");
    //             forceClean = true;
    //             request->send(200, "text/html", 
    //           R"(<!DOCTYPE html><html><head><meta charset="UTF-8" /><meta http-equiv="refresh" content="1; URL='/'" />
    //             </head><body></body></html>)");});
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  Serial.printf("Sending response started\n");
                  request->send(
                      200,
                      "text/html", 
                      getHomepage()
                      );
                  Serial.printf("Sending response ends\n");
              });

    //setup websocket GUI stuff
    setupGUI(&server);
    server.addHandler(&ws);


    // Starting Async OTA web server
    #if defined(ENABLE_LOGIN)
      AsyncElegantOTA.begin(&server, OTA_username, OTA_password);
    #else
      AsyncElegantOTA.begin(&server);
    #endif
    server.begin();

    #if defined(BLOCKMODE) 
    //wait for boot button to be pressed before beginning program
    int tries = 0;
    while(digitalRead(0)) {
        if(tries > 5) break; 
        delay(500);
        Serial.println("IO");
        WebSerial.print("IO0: "); WebSerial.println(digitalRead(0));
        tries++;
    }
    if(!digitalRead(0)) {
      while(1);
    }
    #endif
}