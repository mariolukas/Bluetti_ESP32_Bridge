#ifndef BLUETTI_CONFIG_H
#define BLUETTI_CONFIG_H

#include "DeviceType.h"
#include "PowerStation.h"
#include "config.h"

#if POWER_STATION(AC300)
  #include "Device_AC300.h"
#elif POWER_STATION(AC200M)
 #include "Device_AC200M.h"
#elif POWER_STATION(EP500)
 #include "Device_EP500.h"
#elif POWER_STATION(EB3A)
 #include "Device_EB3A.h"
#elif POWER_STATION(EP500P)
 #include "Device_EP500P.h"
#elif POWER_STATION(AC500)
 #include "Device_AC500.h"
#elif POWER_STATION(EP600)
 #include "Device_EP600.h"
#elif POWER_STATION(PBOX)
 #include "Device_EP600.h"
#endif

#endif
