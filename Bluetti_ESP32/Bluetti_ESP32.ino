#include "BWifi.h"
#include "BTooth.h"
#include "MQTT.h"
#include "config.h"

void setup() {
  Serial.begin(115200);
  #ifdef RELAISMODE
    pinMode(RELAIS_PIN, OUTPUT);
    #ifdef DEBUG
      Serial.println("deactivate relais contact");
    #endif
    digitalWrite(RELAIS_PIN, RELAIS_LOW);
  #endif
  initBWifi(false);
  initBluetooth();
  initMQTT();
}

void loop() {
  handleBluetooth();
  handleMQTT(); 
  handleWebserver();
}
