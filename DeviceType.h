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

// This has to be edited/extended in order to add new values that shall be published via mqtt
// Don't forget to edit <String map_field_name(enum field_names f_name)> in MQTT.cpp as well
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
  INTERNAL_CELL1_VOLTAGE,
  INTERNAL_CELL2_VOLTAGE,
  INTERNAL_CELL3_VOLTAGE,
  INTERNAL_CELL4_VOLTAGE,
  INTERNAL_CELL5_VOLTAGE,
  INTERNAL_CELL6_VOLTAGE,
  INTERNAL_CELL7_VOLTAGE,
  INTERNAL_CELL8_VOLTAGE,
  INTERNAL_CELL9_VOLTAGE,
  INTERNAL_CELL10_VOLTAGE,
  INTERNAL_CELL11_VOLTAGE,
  INTERNAL_CELL12_VOLTAGE,
  INTERNAL_CELL13_VOLTAGE,
  INTERNAL_CELL14_VOLTAGE,
  INTERNAL_CELL15_VOLTAGE,
  INTERNAL_CELL16_VOLTAGE
  
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
