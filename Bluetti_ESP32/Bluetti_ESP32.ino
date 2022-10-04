#include "BWifi.h"
#include "BTooth.h"
#include "MQTT.h"

void setup() {
  Serial.begin(115200);
  initBWifi();
  initBluetooth();
  initMQTT();
}

void loop() {
  handleBluetooth();
  handleMQTT(); 
}
