#include "BluettiConfig.h"
#include "BWifi.h"
#include "BTooth.h"
#include "MQTT.h"
#include "index.h"  //Web page header file
#include <EEPROM.h>
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer/archive/master.zip
#include <AsyncTCP.h> // https://github.com/me-no-dev/AsyncTCP/archive/master.zip
#include <ESPmDNS.h>
#include <ElegantOTA.h> // https://github.com/ayushsharma82/ElegantOTA/archive/master.zip
#include "display.h"

AsyncWebServer server(80);
AsyncEventSource events("/events");

unsigned long lastTimeWebUpdate = 0;  

String lastMsg = ""; 

bool msgViewerDetails = false;
bool shouldSaveConfig = false;
int wifiReconnectCounter = 0;

char mqtt_server[40] = "127.0.0.1";
char mqtt_port[6]  = "1883";
char bluetti_device_id[40] = "e.g. ACXXXYYYYYYYY";

void saveConfigCallback () {
  shouldSaveConfig = true;
}


ESPBluettiSettings wifiConfig;

ESPBluettiSettings get_esp32_bluetti_settings(){
    return wifiConfig;
    return wifiConfig;
}

void eeprom_read(){
  Serial.println(F("Loading Values from EEPROM"));
  EEPROM.begin(512);
  EEPROM.get(0, wifiConfig);
  EEPROM.end();
}

void eeprom_saveconfig(){
  Serial.println(F("Saving Values to EEPROM"));
  EEPROM.begin(512);
  EEPROM.put(0, wifiConfig);
  EEPROM.commit();
  EEPROM.end();
}

void setWiFiPowerSavingMode(){
  //esp_wifi_set_ps(WIFI_PS_MAX_MODEM); // maximum power saving, does not make sense here
  //esp_wifi_set_ps(WIFI_PS_NONE); // will cause kernel panic and reboot on my ESP32 (AlexBurghardt)
  esp_wifi_set_ps(WIFI_PS_MIN_MODEM); // default
}

void initBWifi(bool resetWifi){

  eeprom_read();

  WiFiManagerParameter custom_mqtt_server("server", "MQTT Server Address", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("port", "MQTT Server Port", mqtt_port, 6);
  WiFiManagerParameter custom_mqtt_username("username", "MQTT Username", "", 40);
  WiFiManagerParameter custom_mqtt_password("password", "MQTT Password", "", 40, "type=password");
  WiFiManagerParameter custom_ota_username("ota_username", "OTA Username", "", 40);
  WiFiManagerParameter custom_ota_password("ota_password", "OTA Password", "", 40, "type=password");
  WiFiManagerParameter custom_bluetti_device("bluetti", "Bluetti Bluetooth ID", bluetti_device_id, 40);

  WiFiManager wifiManager;

  if (resetWifi){
    wifiManager.resetSettings();
    ESPBluettiSettings defaults;
    wifiConfig = defaults;
    eeprom_saveconfig();
  } else if (wifiConfig.salt != EEPROM_SALT) {
    Serial.println("Invalid settings in EEPROM, trying with defaults");
    ESPBluettiSettings defaults;
    wifiConfig = defaults;
  } else {
    wifiManager.setConfigPortalTimeout(300);
  }

  wifiManager.setSaveConfigCallback(saveConfigCallback);

  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_username);
  wifiManager.addParameter(&custom_mqtt_password);
  wifiManager.addParameter(&custom_ota_username);
  wifiManager.addParameter(&custom_ota_password);
  wifiManager.addParameter(&custom_bluetti_device);
  
  wifiManager.setAPCallback([&](WiFiManager* wifiManager) {
		Serial.printf("Entered config mode:ip=%s, ssid='%s'\n", 
                        WiFi.softAPIP().toString().c_str(), 
                        wifiManager->getConfigPortalSSID().c_str());
                        #ifdef DISPLAYSSD1306
                          wrDisp_wifisignal(2); //AP mode
                          wrDisp_IP(WiFi.softAPIP().toString().c_str());
                          wrDisp_Status("Setup Wifi");
                        #endif
	});
  
  if (!wifiManager.autoConnect("Bluetti_ESP32")) {
    ESP.restart();
  }

  if (shouldSaveConfig) {
     strlcpy(wifiConfig.mqtt_server, custom_mqtt_server.getValue(), 40);
     strlcpy(wifiConfig.mqtt_port, custom_mqtt_port.getValue(), 6);
     strlcpy(wifiConfig.mqtt_username, custom_mqtt_username.getValue(), 40);
     strlcpy(wifiConfig.mqtt_password, custom_mqtt_password.getValue(), 40);
     strlcpy(wifiConfig.ota_username, custom_ota_username.getValue(), 40);
     strlcpy(wifiConfig.ota_password, custom_ota_password.getValue(), 40);
     strlcpy(wifiConfig.bluetti_device_id, custom_bluetti_device.getValue(), 40);
     eeprom_saveconfig();
  }

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    // display will have blinking wifi signal until connected.
    #ifdef DISPLAYSSD1306
      disp_setPrevStateIcon(0);
      wrDisp_wifisignal(0);
      delay(200);
      Serial.print(".");
      disp_setPrevStateIcon(1);
      wrDisp_wifisignal(0);
    #else
      delay(500);
      Serial.print(".");
    #endif

  }
  
  WiFi.setAutoReconnect(true);

  Serial.println(F(""));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
  #ifdef DISPLAYSSD1306
    wrDisp_IP(WiFi.localIP().toString().c_str());
    disp_setWifiSignal(1, WiFi.RSSI());
  #endif
  if (MDNS.begin(DEVICE_NAME)) {
    Serial.println(F("MDNS responder started"));
  }

  //setup web server handling
  #if MSG_VIEWER_DETAILS
      msgViewerDetails = true;
      Serial.println(F("webserver BT/MQTT variable logging enabled..."));
    #else
      msgViewerDetails = false;
      Serial.println(F("webserver BT/MQTT variable logging disabled..."));
  #endif

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", index_html, processorWebsiteUpdates);
  });
  server.on("/switchLogging", HTTP_GET, [](AsyncWebServerRequest *request){
      msgViewerDetails = !msgViewerDetails;
      if(msgViewerDetails){
        Serial.println(F("webserver BT/MQTT variable logging enabled..."));
      }
      else{
        Serial.println(F("webserver BT/MQTT variable logging disabled..."));
      }
      request->send_P(200, "text/html", index_html, processorWebsiteUpdates);
  });
  server.on("/rebootDevice", [](AsyncWebServerRequest *request) {
      request->send(200, "text/plain", "reboot in 2sec");
      delay(2000);
      ESP.restart();
  });
  server.on("/resetConfig", [](AsyncWebServerRequest *request) {
      request->send(200, "text/plain", "reset Wifi and reboot in 2sec");
      delay(2000);
      initBWifi(true);
  });
  //setup web server events
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    client->send("hello my friend, I'm just your data feed!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  if (!wifiConfig.ota_username) {
    ElegantOTA.begin(&server);
  } else {
    ElegantOTA.begin(&server, wifiConfig.ota_username, wifiConfig.ota_password);
  }

  server.begin();
  Serial.println(F("HTTP server started"));

}

void handleWebserver() {
  
  //Serial.println(F("DEBUG handleWebserver"));
  if ((millis() - lastTimeWebUpdate) > MSG_VIEWER_REFRESH_CYCLE*1000) {
    
    // check wifi status every MSG_VIEWER_REFRESH_CYCLE and set display 
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println(F("WiFi is disconnected, try to reconnect..."));
      #ifdef DISPLAYSSD1306
        disp_setWifiMode(0);
        disp_setStatus("Wifi err..");
      #endif
      WiFi.disconnect();
      WiFi.reconnect();
      AddtoMsgView(String(millis()) + ": WLAN ERROR! try to reconnect");
      wifiReconnectCounter++;
      //delay(1000); no delay as we only check every 5 seconds. Removing 1 second blocking of the program in the loop.
    } else {
      #ifdef DISPLAYSSD1306
        disp_setWifiSignal(1,WiFi.RSSI());
        if (wifiReconnectCounter > 0)
        {
          //only update display ones after wifi is recovered.
          disp_setStatus("Running!");
          wifiReconnectCounter = 0;
        }
      #endif
    }

    #ifdef DISPLAYSSD1306
      // update display
      disp_setBlueTooth(isBTconnected());
      disp_setMqttStatus(isMQTTconnected());
    #endif

    // Send Events to the Web Server with current data
    events.send("ping",NULL,millis());
    events.send(String(millis()).c_str(),"runtime",millis());
    events.send(String(WiFi.RSSI()).c_str(),"rssi",millis());
    events.send(String(isMQTTconnected()).c_str(),"mqtt_connected",millis());
    events.send(String(getLastMQTTMessageTime()).c_str(),"mqtt_last_msg_time",millis());
    events.send(String(isBTconnected()).c_str(),"bt_connected",millis());
    events.send(String(getLastBTMessageTime()).c_str(),"bt_last_msg_time",millis());
    if(msgViewerDetails){
      events.send(lastMsg.c_str(),"last_msg",millis());
    } 
    
    lastTimeWebUpdate = millis();
  }
}


String processorWebsiteUpdates(const String& var){
  
  if(var == "IP"){
    return String(WiFi.localIP().toString());
  }
  else if(var == "RSSI"){
    return String(WiFi.RSSI());
  }
  else if(var == "SSID"){
    return String(WiFi.SSID());
  }
  else if(var == "MAC"){
    return String(WiFi.macAddress());
  }
  else if(var == "RUNTIME"){
    return String(millis());
  }
  else if(var == "MQTT_IP"){
    char msg[40];
    if (strlen(wifiConfig.mqtt_server) == 0){
      strlcpy(msg, "No MQTT server configured", 40);
    }else{
      strlcpy(msg, wifiConfig.mqtt_server, 40);
    }
    
    return msg;
  }
  else if(var == "MQTT_PORT"){
    char msg[6];
    strlcpy(msg, wifiConfig.mqtt_port, 6);
    return msg;
  }
  else if(var == "MQTT_CONNECTED"){
    return String(isMQTTconnected());
  }
  else if(var == "LAST_MQTT_MSG_TIME"){
    return String(getLastMQTTMessageTime());
  }
  else if(var == "DEVICE_ID"){
    char msg[40];
    strlcpy(msg, wifiConfig.bluetti_device_id, 40);
    return msg;
  }
  else if(var == "BT_CONNECTED"){
    return String(isBTconnected());
  }
  else if(var == "LAST_BT_MSG_TIME"){
    return String(getLastBTMessageTime());
  }
  else if(var == "BT_ERROR"){
    return String(getPublishErrorCount());
  }
  else if(var == "LAST_MSG"){
    if (msgViewerDetails){
      return String("...waiting for data...");
    }
    else{
      return String("...disabled...");
    }
  }
  else //return something, else this if then else will crash in case calles without VAR set....
  {
    return String("");
  }
}

void AddtoMsgView(String data){
  
  String tempMsg = "";
  
  int firstPos = lastMsg.indexOf("</p>");
  int nextPos = firstPos;
  int numEntry = 0;
  while(nextPos > 0){
    nextPos = lastMsg.indexOf("</p>",nextPos+4);
    if (nextPos > 0){
      numEntry++;
    }
  }

  if (numEntry > MSG_VIEWER_ENTRY_COUNT-2){
    tempMsg = lastMsg.substring(firstPos+4);
    lastMsg = tempMsg + "<p>" + data + "</p>";
  }
  else{
    lastMsg = lastMsg + "<p>" + data + "</p>";
  }
}
