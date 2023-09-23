#ifndef DEVICE_AC300_H
#define DEVICE_AC300_H
#include "Arduino.h"

/* Not implemented yet
enum output_mode {
    STOP = 0,
    INVERTER_OUTPUT = 1,
    BYPASS_OUTPUT_C = 2,
    BYPASS_OUTPUT_D = 3,
    LOAD_MATCHING = 4
};

enum ups_mode {
    CUSTOMIZED = 1,
    PV_PRIORITY = 2,
    STANDARD = 3,
    TIME_CONTROl = 4  
};

enum auto_sleep_mode {
  THIRTY_SECONDS = 2,
  ONE_MINNUTE = 3,
  FIVE_MINUTES = 4,
  NEVER = 5  
};
*/

enum auto_sleep_mode {
  THIRTY_SECONDS = 2,
  ONE_MINNUTE = 3,
  FIVE_MINUTES = 4,
  NEVER = 5
};

enum LedMode {
    LED_LOW = 1,
    LED_HIGH = 2,
    LED_SOS = 3,
    LED_OFF = 4
};

enum EcoShutdown {
    ONE_HOUR = 1,
    TWO_HOURS = 2,
    THREE_HOURS = 3,
    FOUR_HOURS = 4
};

enum ChargingMode {
    STANDARD = 0,
    SILENT = 1,
    TURBO = 2
};

// { FIELD_NAME, PAGE, OFFSET, SIZE, SCALE (if scale is needed e.g. decimal value, defaults to 0) , ENUM (if data is enum, defaults to 0) , FIELD_TYPE }
static device_field_data_t bluetti_device_state[] = {
  /*Page 0x00 Core */
  {DEVICE_TYPE,           0x00, 0x0A, 7, 0, 0, STRING_FIELD},
  {SERIAL_NUMBER,         0x00, 0x11, 4, 0 ,0, SN_FIELD},
  {ARM_VERSION,           0x00, 0x17, 2, 0, 0, VERSION_FIELD},
  {DSP_VERSION,           0x00, 0x19, 2, 0, 0, VERSION_FIELD},
  {DC_INPUT_POWER,        0x00, 0x24, 1, 0, 0, UINT_FIELD},
  {AC_INPUT_POWER,        0x00, 0x25, 1, 0, 0, UINT_FIELD},
  {AC_OUTPUT_POWER,       0x00, 0x26, 1, 0, 0, UINT_FIELD},
  {DC_OUTPUT_POWER,       0x00, 0x27, 1, 0, 0, UINT_FIELD},
  {POWER_GENERATION,      0x00, 0x29, 1, 1, 0, DECIMAL_FIELD},
  {TOTAL_BATTERY_PERCENT, 0x00, 0x2B, 1,0,0, UINT_FIELD},
  {AC_OUTPUT_ON,          0x00, 0x30, 1, 0, 0, BOOL_FIELD},
  {DC_OUTPUT_ON,          0x00, 0x31, 1, 0, 0, BOOL_FIELD},
  {AC_OUTPUT_MODE,        0x00, 0x46, 1, 0, 0, UINT_FIELD},

  {INTERNAL_AC_VOLTAGE,       0x00, 0x47, 1, 1, 0, DECIMAL_FIELD},
  //INTERNAL_POWER_ONE AC Output usage
  {INTERNAL_CURRENT_ONE,      0x00, 0x48, 1, 1, 0, DECIMAL_FIELD},
  {INTERNAL_POWER_ONE,        0x00, 0x49, 1, 0, 0, UINT_FIELD},
  {INTERNAL_AC_FREQUENCY,     0x00, 0x4A, 1, 2, 0, DECIMAL_FIELD},
  //INTERNAL_POWER_TWO AC Internal usage?
  {INTERNAL_CURRENT_TWO,      0x00, 0x4B, 1, 1, 0, DECIMAL_FIELD},
  {INTERNAL_POWER_TWO,        0x00, 0x4C, 1, 0, 0, UINT_FIELD},
  {AC_INPUT_VOLTAGE,          0x00, 0x4D, 1, 1, 0, DECIMAL_FIELD},
  //INTERNAL_POWER_THREE AC Load from grid
  {INTERNAL_CURRENT_THREE,    0x00, 0x4E, 1, 1, 0, DECIMAL_FIELD},
  {INTERNAL_POWER_THREE,      0x00, 0x4F, 1, 0, 0, UINT_FIELD},


  {AC_INPUT_FREQUENCY,        0x00, 0x50, 1, 2, 0, DECIMAL_FIELD},
  {INTERNAL_DC_INPUT_VOLTAGE, 0x00, 0x56, 1, 1, 0, DECIMAL_FIELD},
  {INTERNAL_DC_INPUT_POWER,   0x00, 0x57, 1, 0, 0, UINT_FIELD},
  {INTERNAL_DC_INPUT_CURRENT, 0x00, 0x58, 1, 1, 0, DECIMAL_FIELD},
  {PACK_NUM_MAX,              0x00, 0x5B, 1, 0, 0, UINT_FIELD },

  {INTERNAL_PACK_VOLTAGE,     0x00, 0x5C, 1, 1 ,0, DECIMAL_FIELD},
  {PACK_BATTERY_PERCENT,      0x00, 0x63, 1, 0, 0, UINT_FIELD},

  {PACK_NUM,                  0x00, 0x60, 1, 0, 0, UINT_FIELD},

  {UPS_MODE,                 0x0B, 0xB9, 1, 0, 0, UINT_FIELD},
   //{PACK_NUM,                 0x0B, 0xBE, 1, 0, 0, UINT_FIELD},
  {GRID_CHARGE_ON,           0x0B, 0xC3, 1, 0, 0, BOOL_FIELD},
  {AUTO_SLEEP_MODE,          0x0B, 0xF5, 1, 0, 0, UINT_FIELD}

  
};

static device_field_data_t bluetti_device_command[] = {
  /*Page 0x00 Core */
  {AC_OUTPUT_ON,      0x0B, 0xBF, 1, 0, 0, BOOL_FIELD}, 
  {DC_OUTPUT_ON,      0x0B, 0xC0, 1, 0, 0, BOOL_FIELD}
};

static device_field_data_t bluetti_polling_command[] = {
  {FIELD_UNDEFINED, 0x00, 0x0A, 0x28 ,0 , 0, TYPE_UNDEFINED},
  {FIELD_UNDEFINED, 0x00, 0x46, 0x15 ,0 , 0, TYPE_UNDEFINED},
 // {FIELD_UNDEFINED, 0x0B, 0xB9, 0x3D ,0 , 0, TYPE_UNDEFINED}

  {FIELD_UNDEFINED, 0x0B, 0xDA, 0x01 ,0 , 0, TYPE_UNDEFINED},
  {FIELD_UNDEFINED, 0x0B, 0xF5, 0x07 ,0 , 0, TYPE_UNDEFINED},
  //Pack Polling
  {FIELD_UNDEFINED, 0x00, 0x5B, 0x25 ,0 , 0, TYPE_UNDEFINED}
};

#endif
