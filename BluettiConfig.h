#ifndef BLUETTI_CONFIG_H
#define BLUETTI_CONFIG_H

#include "DeviceType.h"
#include "config.h"
#include "PowerStation.h"

// this selection does not seem to work properly, only the first header file in the selection gets included
// so put your <DEVICE_XXXX.h> at the top postion in this selection to make sure that your <DEVICE_XXXX.h> gets included
// This is at least my personal experience (AlexBurghardt)
#if POWER_STATION(AC200M)
  #include "Device_AC200M.h"

// the part below does not seem to be working as intended
#elif POWER_STATION(AC300)
  #include "Device_AC300.h"
#elif POWER_STATION(EP500)
 #include "Device_EP500.h"
#elif POWER_STATION(EB3A)
 #include "Device_EB3A.h"
#elif POWER_STATION(EP500P)
 #include "Device_EP500P.h"
#elif POWER_STATION(AC500)
 #include "Device_AC500.h"
#endif

#endif
