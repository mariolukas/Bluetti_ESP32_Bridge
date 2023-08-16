#ifndef DEVICE_EP600_H
#define DEVICE_EP600_H
#include "Arduino.h"

// Based on https://doc.chromedshark.com/bluetti/ep600.html
// and https://github.com/warhammerkid/bluetti_mqtt

// { FIELD_NAME, PAGE, OFFSET, SIZE, SCALE (if scale is needed e.g. decimal value, defaults to 0) , ENUM (if data is enum, defaults to 0) , FIELD_TYPE }
static device_field_data_t bluetti_device_state[] = {
  {TOTAL_BATTERY_PERCENT,     0x00, 0x66, 1, 0, 0, UINT_FIELD},
  {DEVICE_TYPE,               0x00, 0x6E, 6, 0, 0, STRING_FIELD},   // TODO: swap string
  {SERIAL_NUMBER,             0x00, 0x74, 4, 0 ,0, SN_FIELD},
  {POWER_GENERATION,          0x00, 0x90, 1, 3, 0, DECIMAL_FIELD},

  {BATTERY_MIN_PERCENTAGE,    0x07, 0xE6, 1, 0, 0, UINT_FIELD},
  {AC_CHARGE_MAX_PERCENTAGE,  0x07, 0xE7, 1, 0, 0, UINT_FIELD},

  {AC_INPUT_POWER_MAX,        0x08, 0xA5, 1, 0, 0, UINT_FIELD},
  {AC_INPUT_CURRENT_MAX,      0x08, 0xA6, 1, 0, 0, UINT_FIELD},
  {AC_OUTPUT_POWER_MAX,       0x08, 0xA7, 1, 0, 0, UINT_FIELD},
  {AC_OUTPUT_CURRENT_MAX,     0x08, 0xA8, 1, 0, 0, UINT_FIELD},
};

static device_field_data_t bluetti_device_command[] = {};

// {FIELD_NAME, PAGE, OFFSET, FIELDS_TO_READ, 0, 0, TYPE_UNDEFINED}
static device_field_data_t bluetti_polling_command[] = {
  {FIELD_UNDEFINED,           0x00, 0x64, 0x3E, 0, 0, TYPE_UNDEFINED},
  {FIELD_UNDEFINED,           0x07, 0xD0, 0x30, 0, 0, TYPE_UNDEFINED},
  {FIELD_UNDEFINED,           0x08, 0x00, 0x29, 0, 0, TYPE_UNDEFINED},
};

#endif
