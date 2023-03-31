#include "BWifi.h"
#include "BTooth.h"
#include "MQTT.h"
#include "config.h"
#include "display.h"

unsigned long lastTime1 = 0;
unsigned long timerDelay1 = 3000;

void setup() {
  Serial.begin(115200);
  #ifdef RELAISMODE
    pinMode(RELAIS_PIN, OUTPUT);
    #ifdef DEBUG
      Serial.println(F("deactivate relais contact"));
    #endif
    digitalWrite(RELAIS_PIN, RELAIS_LOW);
  #endif
  #ifdef SLEEP_TIME_ON_BT_NOT_AVAIL
    esp_sleep_enable_timer_wakeup(SLEEP_TIME_ON_BT_NOT_AVAIL * 60 * 1000000ULL);
  #endif
  #ifdef DISPLAY
    initDisplay();
  #endif
  initBWifi(false);
  initBluetooth();
  initMQTT();
  #ifdef DISPLAY
    wrDisp_Status("Running!");
  #endif
}

void loop() {
  #ifdef DISPLAY
    handleDisplay();
  #endif
  handleBluetooth();
  handleMQTT(); 
  handleWebserver();
}
