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

// { FIELD_NAME, PAGE, OFFSET, SIZE, SCALE (if scale is needed e.g. decimal value, defaults to 0) , ENUM (if data is enum, defaults to 0) , FIELD_TYPE }
static device_field_data_t bluetti_device_state[] = {
  /*Page 0x00 Core */
  {DEVICE_TYPE,       0x00, 0x0A, 7, 0, 0, STRING_FIELD},
  {SERIAL_NUMBER,     0x00, 0x11, 4, 0 ,0, SN_FIELD},
  {ARM_VERSION,       0x00, 0x17, 2, 0, 0, VERSION_FIELD},
  {DSP_VERSION,       0x00, 0x19, 2, 0, 0, VERSION_FIELD},
  {DC_INPUT_POWER,    0x00, 0x24, 1, 0, 0, UINT_FIELD},
  {AC_INPUT_POWER,    0x00, 0x25, 1, 0, 0, UINT_FIELD},
  {AC_OUTPUT_POWER,   0x00, 0x26, 1, 0, 0, UINT_FIELD},
  {DC_OUTPUT_POWER,   0x00, 0x27, 1, 0, 0, UINT_FIELD},
  {POWER_GENERATION,  0x00, 0x29, 1, 1, 0, DECIMAL_FIELD},
  {TOTAL_BATTERY_PERCENT, 0x00, 0x2B, 1,0,0, UINT_FIELD},
  {AC_OUTPUT_ON,      0x00, 0x30, 1, 0, 0, BOOL_FIELD},
  {DC_OUTPUT_ON,      0x00, 0x31, 1, 0, 0, BOOL_FIELD},

  /*Page 0x00 Details 
  {INTERNAL_AC_VOLTAGE,       0x00, 0x47, 1, 1, 0, DECIMAL_FIELD},
  {INTERNAL_CURRENT_ONE,      0x00, 0x48, 1, 1, 0, DECIMAL_FIELD},

  //Page 0x00 Battery Details
  {PACK_NUM_MAX, 0x00, 0x5B, 1, 0, 0, UINT_FIELD },

  //Page 0x00 Battery Data 
  {PACK_VOLTAGE, 0x00, 0x62, 1, 2 ,0 ,DECIMAL_FIELD},
  */
  
};

static device_field_data_t bluetti_device_command[] = {
  /*Page 0x00 Core */
  {AC_OUTPUT_ON,      0x0B, 0xBF, 1, 0, 0, BOOL_FIELD}, 
  {DC_OUTPUT_ON,      0x0B, 0xC0, 1, 0, 0, BOOL_FIELD}
};

static device_field_data_t bluetti_polling_command[] = {
  {FIELD_UNDEFINED, 0x00, 0x0A, 0x28 ,0 , 0, TYPE_UNDEFINED},
  {FIELD_UNDEFINED, 0x00, 0x46, 0x15 ,0 , 0, TYPE_UNDEFINED},
  {FIELD_UNDEFINED, 0x0B, 0xB9, 0x3D ,0 , 0, TYPE_UNDEFINED}
};

#endif
