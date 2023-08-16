#ifndef CONFIG_H
#define CONFIG_H
#include "Arduino.h"

#define DEBUG                 1
// Display config section, comment DISPLAYSSD1306 to disable display
//#define DEBUGDISP 1
//#define DISPLAYSSD1306  1
#define DISPLAY_SCL_PORT 4
#define DISPLAY_SDA_PORT 5
//Uncomment to toggle display reset on start, required for displays like LoRa TTGO v1.0
//#define DISPLAY_RST_PORT 16

//#define RESET_WIFI_SETTINGS   

#define EEPROM_SALT 13374

#define DEVICE_NAME "BLUETTI-MQTT"
#define BLUETTI_TYPE EP600

#define BLUETOOTH_QUERY_MESSAGE_DELAY 3000

#define RELAISMODE 1
#define RELAIS_PIN 22
#define RELAIS_LOW LOW
#define RELAIS_HIGH HIGH

#define MAX_DISCONNECTED_TIME_UNTIL_REBOOT 5 //device will reboot when wlan/BT/MQTT is not connectet within x Minutes
#define SLEEP_TIME_ON_BT_NOT_AVAIL 2 //device will sleep x minutes if restarted is triggered by bluetooth error
                                     //set to 0 to disable
#define DEVICE_STATE_UPDATE  5
#define MSG_VIEWER_DETAILS 0 //enable detailed BT/MQTT messages via WebUI by default, can be changed in WebUI
#define DEVICE_STATE_STATUS_UPDATE  2.5 //Was 0.5 in original branc which is half the DEVICE_STATE_UPDATE value, kept the ratio
#define MSG_VIEWER_ENTRY_COUNT 20 //number of lines for web message viewer
#define MSG_VIEWER_REFRESH_CYCLE 5 //refresh time for website data in seconds


#ifndef BLUETTI_TYPE
  #define BLUETTI_TYPE AC300
#endif



#endif
