#ifndef CONFIG_H
#define CONFIG_H
#include "Arduino.h"

#define DEBUG                 1
//#define RESET_WIFI_SETTINGS   1

#define EEPROM_SALT 13374

#define BLUETOOTH_QUERY_MESSAGE_DELAY 3000

#ifndef BLUETTI_TYPE
  #define BLUETTI_TYPE BLUETTI_AC300
#endif


#endif
