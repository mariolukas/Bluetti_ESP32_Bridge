#ifndef MQTT_H
#define MQTT_H
#include "Arduino.h"
#include "DeviceType.h"

extern void publishTopic(enum field_names field_name, String value);
extern void handleMQTT();
extern void initMQTT();
extern bool isMQTTconnected();

#endif
