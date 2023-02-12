#ifndef POWER_STATION_H
#define POWER_STATION_H
#include "Arduino.h"

#define UNKNOWN_DEVICE -1
#define AC300           1
#define AC200M          2
#define EP500           3
#define EB3A            4
#define EP500P          5
#define AC500           6

#define POWER_STATION(type) (BLUETTI_TYPE==type)

#endif
