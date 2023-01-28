#include <U8g2lib.h>
#include <Wire.h>
#include <Config.h>
#include "BDisplay.h"
#include <WiFiManager.h> // to have wl_status_t available
#include "GlobalVar.h" // to have wifi, BLE and mqtt status variables available

unsigned long lastDisplayWrite = 0;
int DisplayPage =1;
int DisplayRefreshes =1;

String map_wifi_status(wl_status_t WifiStatus){

  // whole enum list from WifiManager
  switch(WifiStatus) {
    case WL_NO_SHIELD:
      return "No Shield";
      break; 
    case WL_IDLE_STATUS:
      return "Idle";
      break; 
    case WL_NO_SSID_AVAIL:
      return "No SSID";
      break; 
    case WL_SCAN_COMPLETED:
      return "Scannned";
      break; 
    case WL_CONNECTED:
      return "Connected";
      break;       
    case WL_CONNECT_FAILED:
      return "Failed";
      break; 
    case WL_CONNECTION_LOST:
      return "Lost";
      break;       
    case WL_DISCONNECTED:
      return "Disconnect";
      break; 
    default:
      return "unknown";
      break;
  }
}

// 0.96" OLED
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// 1.3" OLED, this is recommended. Readability is way better than with the 0.96" OLED
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// Different font and fontsize for splashscreen
void u8g2_preparSplashScreen() {
  u8g2.setFont(u8g2_font_logisoso16_tf); 
  u8g2.setFontRefHeightExtendedText(); 
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}

// Normal font and fontsize for status display
void u8g2_prepareNormal() {
  u8g2.setFont(u8g2_font_t0_14_me); 
  u8g2.setFontRefHeightExtendedText(); 
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}

// smaller font and fontsize for status display
void u8g2_prepareReduced() {
  u8g2.setFont(u8g2_font_t0_12_me); 
  u8g2.setFontRefHeightExtendedText(); 
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}

// Initialize display and draw splashscreen
void initDisplay() {
  u8g2.begin();
  u8g2_preparSplashScreen();
  u8g2.clearBuffer();
  u8g2.drawStr(14, 7, "ESP32");
  u8g2.drawStr(14, 37, "Bluetti2MQTT");
  u8g2.sendBuffer();
}

void u8g2DrawFrame() {
  u8g2.drawRFrame(0, 0, 128, 64, 4);
}

// switch/rotate after <REFRESHES_PER_PAGE> refreshes
// if you want the rotation to be slower, increase <REFRESHES_PER_PAGE> in BDisplay.h
void SwitchPage() {
  if (DisplayRefreshes <= (REFRESHES_PER_PAGE-1)){
    DisplayRefreshes++;
  } else {
    DisplayRefreshes = 1;
    if (DisplayPage <= (PAGES_TO_DISPLAY-1)) {
      DisplayPage++;
    } else {
      DisplayPage = 1;
    }
  }
} 

// called in loop() to keep all things running
void handleDisplay() {
  
  //refresh displayed values every <DISPLAY_WRITE_DELAY> milliseconds
  if (millis() - lastDisplayWrite > DISPLAY_WRITE_DELAY){

    // Init
    u8g2.clearBuffer();
    // Preparation and frame drawing
    u8g2_prepareReduced();
    u8g2DrawFrame();

    // currently three views/pages with different information can be displayd in rotation
    switch(DisplayPage) {

      case 1:

        // Draw wifi status
        u8g2.drawStr(5, 4, "WIFI:");
        u8g2.setCursor(45, 4);
        u8g2.print(map_wifi_status(WiFi.status()));

        // Draw wifi rssi
        u8g2.drawStr(5, 19, "RSSI:");
        u8g2.setCursor(45, 19);
        WifiRssiValue = WiFi.RSSI();
        if ((WifiRssiValue >= -60) && (WifiRssiValue < 0)){
          u8g2.print(String(WifiRssiValue) + " (Super)");
        } else if (WifiRssiValue >= -70){
          u8g2.print(String(WifiRssiValue) + " (Good)");
        } else if (WifiRssiValue >= -80){
          u8g2.print(String(WifiRssiValue) + " (Fair)");
        } else if (WifiRssiValue < -80){
          u8g2.print(String(WifiRssiValue) + " (Weak)");
        } else {
          u8g2.print(String(WifiRssiValue));
        }

        // Draw BLE rssi and status
        u8g2.drawStr(5, 34, "  BT:");
        u8g2.setCursor(45, 34);
        if (BTIsConnected){    
          if ((BTRssiValue >= -55) && (BTRssiValue < 0)){
            u8g2.print(String(BTRssiValue) + " (Super)");
          } else if (BTRssiValue >= -67){
            u8g2.print(String(BTRssiValue) + " (Good)");
          } else if (BTRssiValue >= -90){
            u8g2.print(String(BTRssiValue) + " (Fair)");
          } else if (BTRssiValue < -90){
            u8g2.print(String(BTRssiValue) + " (Weak)");
          }
        } else {
          u8g2.print("Offline");
        }

        // Draw mgtt status
        u8g2.drawStr(5, 47, "MQTT:");
        u8g2.setCursor(45, 47);
        if (MGTTIsConnected){    
          u8g2.print("Connected");
        } else {
          u8g2.print("disconnected");
        }

        SwitchPage();

        break; 
      case 2:

        // Draw SSID
        u8g2.drawStr(5, 4, "ID:");
        u8g2.setCursor(28, 4);
        u8g2.print(WiFi.SSID());

        // Draw local IP
        u8g2.drawStr(5, 19, "IP:");
        u8g2.setCursor(28, 19);
        u8g2.print(WiFi.localIP());

        // Draw subnet mask
        u8g2.drawStr(5, 34, "NM:");
        u8g2.setCursor(28, 34);
        u8g2.print(WiFi.subnetMask());

        // Draw gateway IP
        u8g2.drawStr(5, 47, "GW:");
        u8g2.setCursor(28, 47);
        u8g2.print(WiFi.gatewayIP());
        
        SwitchPage();

        break; 
      case 3:

        // not used yet... in order to use it:
        // #define PAGES_TO_DISPLAY 3 in BDisplay.h
        u8g2.drawStr(6, 18, "NOT USED YET");

        SwitchPage();

        break; 
      default:

        // Preparation and frame drawing
        u8g2_prepareNormal();
        u8g2DrawFrame();

        u8g2.drawStr(6, 18, "ERROR");

        break;
    }

    // send buffer to display
    u8g2.sendBuffer();

    lastDisplayWrite = millis();
  }
}
