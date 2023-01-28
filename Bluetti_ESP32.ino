#include "config.h"
#include "BWifi.h"
#include "BTooth.h"
#include "MQTT.h"
#include "BDisplay.h"
#include "GlobalVar.h"


unsigned long lastTime1 = 0;
unsigned long timerDelay1 = 3000;

void setup() {

  Serial.begin(115200);
  #ifdef RELAISMODE
    pinMode(RELAIS_PIN, OUTPUT);
    #ifdef DEBUG
      Serial.println("deactivate relais contact");
    #endif
    digitalWrite(RELAIS_PIN, RELAIS_LOW);
  #endif
  setCpuFrequencyMhz(240);

  initDisplay();
  initBWifi(false);
  initBluetooth();
  initMQTT();

}

void loop() {

  handleDisplay();
  handleBluetooth();
  handleMQTT(); 
  handleWebserver();

}
