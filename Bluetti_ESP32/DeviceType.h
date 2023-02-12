#ifndef __DEVICE_TYPE_H__
#define __DEVICE_TYPE_H__
#include "Arduino.h"

enum field_types{
   UINT_FIELD,
   BOOL_FIELD,
   ENUM_FIELD,
   STRING_FIELD,
   DECIMAL_ARRAY_FIELD,
   DECIMAL_FIELD,
   VERSION_FIELD,
   SN_FIELD, 
   TYPE_UNDEFINED
};

enum field_names {
  DC_OUTPUT_POWER,
  DC_OUTPUT_ON,
  AC_OUTPUT_POWER,
  AC_OUTPUT_ON,
  POWER_GENERATION,
  TOTAL_BATTERY_PERCENT, 
  DC_INPUT_POWER,
  AC_INPUT_POWER,
  AC_INPUT_VOLTAGE,
  INTERNAL_PACK_VOLTAGE,
  SERIAL_NUMBER,
  ARM_VERSION,
  DSP_VERSION,
  DEVICE_TYPE, 
  UPS_MODE,
  AUTO_SLEEP_MODE,
  GRID_CHANGE_ON,
  FIELD_UNDEFINED,
  INTERNAL_AC_VOLTAGE,
  INTERNAL_AC_FREQUENCY,
  INTERNAL_CURRENT_ONE,
  PACK_NUM_MAX,
  INTERNAL_DC_INPUT_VOLTAGE,
  LED_MODE,
  POWER_OFF,
  ECO_ON,
  ECO_SHUTDOWN,
  CHARGING_MODE,
  POWER_LIFTING_ON
};

typedef struct device_field_data {
  enum field_names f_name;
  uint8_t f_page;
  uint8_t f_offset;
  int8_t f_size;
  int8_t f_scale;
  int8_t f_enum;
  enum field_types f_type;
} device_field_data_t;

#endif