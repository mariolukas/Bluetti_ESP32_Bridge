#include "BluettiConfig.h"
#include "BWifi.h"
#include <EEPROM.h>
#include <WiFiManager.h>
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


void initBWifi(){

  eeprom_read();
  
  if (wifiConfig.salt != EEPROM_SALT) {
    Serial.println("Invalid settings in EEPROM, trying with defaults");
    ESPBluettiSettings defaults;
    wifiConfig = defaults;
  }

  WiFiManagerParameter custom_mqtt_server("server", "MQTT Server Address", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("port", "MQTT Server Port", mqtt_port, 6);
  WiFiManagerParameter custom_bluetti_device("bluetti", "Bluetti Bluetooth ID", bluetti_device_id, 40);

  WiFiManager wifiManager;

#ifdef RESET_WIFI_SETTINGS
  wifiManager.resetSettings();
  ESPBluettiSettings defaults;
  wifiConfig = defaults;
  eeprom_saveconfig();
#endif

  wifiManager.setSaveConfigCallback(saveConfigCallback);
  
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_bluetti_device);

  wifiManager.autoConnect("Bluetti_ESP32");

  if (shouldSaveConfig) {
     strcpy(wifiConfig.mqtt_server, custom_mqtt_server.getValue());
     strcpy(wifiConfig.mqtt_port, custom_mqtt_port.getValue());
     strcpy(wifiConfig.bluetti_device_id, custom_bluetti_device.getValue());
     eeprom_saveconfig();
  }

};
