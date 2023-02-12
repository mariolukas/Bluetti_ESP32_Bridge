#ifndef DEVICE_EB3A_H
#define DEVICE_EB3A_H
#include "Arduino.h"

/* Not implemented yet
//Need to check which additional functions on EB3A are supported
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

  //Page 0x00 Details 
  //{INTERNAL_AC_VOLTAGE,       0x00, 0x47, 1, 1, 0, DECIMAL_FIELD},
  //{INTERNAL_CURRENT_ONE,      0x00, 0x48, 1, 1, 0, DECIMAL_FIELD},
  {AC_INPUT_VOLTAGE,            0x00, 0x4D, 1, 1, 0, DECIMAL_FIELD},
  {INTERNAL_DC_INPUT_VOLTAGE,   0x00, 0x56, 1, 1, 0, DECIMAL_FIELD},

  //Page 0x00 Battery Details
  {PACK_NUM_MAX, 0x00, 0x5B, 1, 0, 0, UINT_FIELD },

  //Page 0x00 Battery Data 
  //{PACK_VOLTAGE, 0x00, 0x62, 1, 2 ,0 ,DECIMAL_FIELD},
  
};

static device_field_data_t bluetti_device_command[] = {
  /*Page 0x00 Core */
  {AC_OUTPUT_ON,      0x0B, 0xBF, 1, 0, 0, BOOL_FIELD}, 
  {DC_OUTPUT_ON,      0x0B, 0xC0, 1, 0, 0, BOOL_FIELD},
  {LED_MODE,          0x0B, 0xDA, 1, 0, 0, ENUM_FIELD},
  {POWER_OFF,         0x0B, 0xF4, 1, 0, 0, BOOL_FIELD},
  {ECO_ON,            0x0B, 0xF7, 1, 0, 0, BOOL_FIELD},
  {ECO_SHUTDOWN,      0x0B, 0xF8, 1, 0, 0, ENUM_FIELD}, 
  {CHARGING_MODE,     0x0B, 0xF9, 1, 0, 0, ENUM_FIELD},
  {POWER_LIFTING_ON,  0x0B, 0xFA, 1, 0, 0, BOOL_FIELD},
};

static device_field_data_t bluetti_polling_command[] = {
  {FIELD_UNDEFINED, 0x00, 0x0A, 0x28 ,0 , 0, TYPE_UNDEFINED},
  {FIELD_UNDEFINED, 0x00, 0x46, 0x15 ,0 , 0, TYPE_UNDEFINED},
  {FIELD_UNDEFINED, 0x0B, 0xDA, 0x01 ,0 , 0, TYPE_UNDEFINED},
  {FIELD_UNDEFINED, 0x0B, 0xF4, 0x07 ,0 , 0, TYPE_UNDEFINED},
};

static device_field_data_t bluetti_logging_command[] = {
  {FIELD_UNDEFINED, 0x00, 0x0A, 0x35 ,0 , 0, TYPE_UNDEFINED},
  {FIELD_UNDEFINED, 0x00, 0x46, 0x42 ,0 , 0, TYPE_UNDEFINED},
  {FIELD_UNDEFINED, 0x00, 0x88, 0x4A ,0 , 0, TYPE_UNDEFINED},
  {FIELD_UNDEFINED, 0x0B, 0xB8, 0x43 ,0 , 0, TYPE_UNDEFINED}

};

#endif