#ifndef MQTT_H
#define MQTT_H
#include "Arduino.h"
#include "DeviceType.h"

extern void publishTopic(enum field_names field_name, String value);
extern void publishHAConfig();
extern void publishDeviceState();
extern void publishDeviceStateStatus();
extern void deviceServoPress(int degree);
extern void handleMQTT();
//TODO: for Command test with no mqtt
extern void callback_str(char* topic, String strPayload, unsigned int length);
extern void initMQTT();
extern bool isMQTTconnected();
extern int getPublishErrorCount();
unsigned long getLastMQTTMessageTime();
unsigned long getLastMQTTDeviceStateMessageTime();
unsigned long getLastMQTTDeviceStateStatusMessageTime();

#endif
