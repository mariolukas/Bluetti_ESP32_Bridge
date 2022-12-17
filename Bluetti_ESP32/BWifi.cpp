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

AsyncWebServer server(80);
AsyncEventSource events("/events");

unsigned long lastTimeWebUpdate = 0;  

String lastMsg = ""; 

bool shouldSaveConfig = false;

char mqtt_server[40] = "127.0.0.1";
char mqtt_port[6]  = "1883";
char bluetti_device_id[40] = "e.g. ACXXXYYYYYYYY";

void saveConfigCallback () {
  shouldSaveConfig = true;
}


ESPBluettiSettings wifiConfig;

ESPBluettiSettings get_esp32_bluetti_settings(){
    return wifiConfig;  
}

void eeprom_read(){
  Serial.println("Loading Values from EEPROM");
  EEPROM.begin(512);
  EEPROM.get(0, wifiConfig);
  EEPROM.end();
}

void eeprom_saveconfig(){
  Serial.println("Saving Values to EEPROM");
  EEPROM.begin(512);
  EEPROM.put(0, wifiConfig);
  EEPROM.commit();
  EEPROM.end();
}


void initBWifi(bool resetWifi){

  eeprom_read();
  
  if (wifiConfig.salt != EEPROM_SALT) {
    Serial.println("Invalid settings in EEPROM, trying with defaults");
    ESPBluettiSettings defaults;
    wifiConfig = defaults;
  }

  WiFiManagerParameter custom_mqtt_server("server", "MQTT Server Address", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("port", "MQTT Server Port", mqtt_port, 6);
  WiFiManagerParameter custom_mqtt_username("username", "MQTT Username", "", 40);
  WiFiManagerParameter custom_mqtt_password("password", "MQTT Password", "", 40, "type=password");
  WiFiManagerParameter custom_bluetti_device("bluetti", "Bluetti Bluetooth ID", bluetti_device_id, 40);

  WiFiManager wifiManager;

  if (resetWifi){
    wifiManager.resetSettings();
    ESPBluettiSettings defaults;
    wifiConfig = defaults;
    eeprom_saveconfig();
  }

  wifiManager.setSaveConfigCallback(saveConfigCallback);
  
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_username);
  wifiManager.addParameter(&custom_mqtt_password);
  wifiManager.addParameter(&custom_bluetti_device);

  wifiManager.autoConnect("Bluetti_ESP32");

  if (shouldSaveConfig) {
     strlcpy(wifiConfig.mqtt_server, custom_mqtt_server.getValue(), 40);
     strlcpy(wifiConfig.mqtt_port, custom_mqtt_port.getValue(), 6);
     strlcpy(wifiConfig.mqtt_username, custom_mqtt_username.getValue(), 40);
     strlcpy(wifiConfig.mqtt_password, custom_mqtt_password.getValue(), 40);
     strlcpy(wifiConfig.bluetti_device_id, custom_bluetti_device.getValue(), 40);
     eeprom_saveconfig();
  }

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  if (MDNS.begin(DEVICE_NAME)) {
    Serial.println("MDNS responder started");
  }

  //setup web server handling
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
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
  
  server.begin();
  Serial.println("HTTP server started");

}

void handleWebserver() {
  if ((millis() - lastTimeWebUpdate) > MSG_VIEWER_REFRESH_CYCLE*1000) {
    
    
    // Send Events to the Web Server with current data
    events.send("ping",NULL,millis());
    events.send(String(millis()).c_str(),"runtime",millis());
    events.send(String(WiFi.RSSI()).c_str(),"rssi",millis());
    events.send(String(isMQTTconnected()).c_str(),"mqtt_connected",millis());
    events.send(String(getLastMQTTMessageTime()).c_str(),"last_mqtt_msg_time",millis());
    events.send(String(isBTconnected()).c_str(),"bt_connected",millis());
    events.send(String(getLastBTMessageTime()).c_str(),"bt_last_msg_time",millis());
    events.send(lastMsg.c_str(),"last_msg",millis());

   
    
    lastTimeWebUpdate = millis();
  }
}

String processorWebsiteUpdates(const String& var){
  if(var == "IP"){
    return String(WiFi.localIP().toString());
  }
  if(var == "RSSI"){
    return String(WiFi.RSSI());
  }
  if(var == "SSID"){
    return String(WiFi.SSID());
  }
  if(var == "MAC"){
    return String(WiFi.macAddress());
  }
  if(var == "RUNTIME"){
    return String(millis());
  }
  else if(var == "MQTT_IP"){
    return String(wifiConfig.mqtt_server);
  }
  else if(var == "MQTT_PORT"){
    return String(wifiConfig.mqtt_port);
  }
  else if(var == "MQTT_CONNECTED"){
    return String(isMQTTconnected());
  }
  else if(var == "LAST_MQTT_MSG_TIME"){
    return String(getLastMQTTMessageTime());
  }
  else if(var == "DEVICE_ID"){
    return String(wifiConfig.bluetti_device_id);
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
    return String("...waiting for data...");
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
