#ifndef DEVICE_EP600_H
#define DEVICE_EP600_H
#include "Arduino.h"

// Based on https://doc.chromedshark.com/bluetti/ep600.html

// { FIELD_NAME, PAGE, OFFSET, SIZE, SCALE (if scale is needed e.g. decimal value, defaults to 0) , ENUM (if data is enum, defaults to 0) , FIELD_TYPE }
static device_field_data_t bluetti_device_state[] = {
  /*Page 0x00 Core */
  {TOTAL_BATTERY_PERCENT, 0x00, 0x66, 1, 0, 0, UINT_FIELD},
  {DEVICE_TYPE,           0x00, 0x6E, 6, 0, 0, STRING_FIELD},
  {SERIAL_NUMBER,         0x00, 0x74, 4, 0 ,0, SN_FIELD},
  //{DC_INPUT_POWER,    0x00, 0x24, 1, 0, 0, UINT_FIELD},
  //{AC_INPUT_POWER,    0x00, 0x25, 1, 0, 0, UINT_FIELD},
  //{AC_OUTPUT_POWER,   0x00, 0x26, 1, 0, 0, UINT_FIELD},
  {POWER_GENERATION,      0x00, 0x90, 1, 3, 0, DECIMAL_FIELD},
};

static device_field_data_t bluetti_device_command[] = {};

// {FIELD_NAME, PAGE, OFFSET, FIELDS_TO_READ, 0, 0, TYPE_UNDEFINED}
static device_field_data_t bluetti_polling_command[] = {
  {FIELD_UNDEFINED,       0x00, 0x64, 0x3E, 0, 0, TYPE_UNDEFINED},
};

#endif
