#ifndef PAYLOAD_PARSER_H
#define PAYLOAD_PARSER_H
#include "Arduino.h"
#include "DeviceType.h"

#define HEADER_SIZE 4
#define CHECKSUM_SIZE 2

uint16_t parse_uint_field(uint8_t data[]);
bool parse_bool_field(uint8_t data[]);
float pase_decimal_field(uint8_t data[], uint8_t scale);
uint64_t parse_serial_field(uint8_t data[]);
float parse_version_field(uint8_t data[]);
String parse_string_field(uint8_t data[]);
String pase_enum_field(uint8_t data[]);

extern void pase_bluetooth_data(uint8_t page, uint8_t offset, uint8_t* pData, size_t length);

#endif
