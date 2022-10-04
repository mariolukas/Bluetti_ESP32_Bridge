#ifndef UTILS_H
#define UTILS_H
#include "Arduino.h"

#define MAX 100

typedef struct{ 
    uint8_t myarr[MAX];
    int mysize;
} wrapper;

extern uint16_t swap_bytes(uint16_t number);
extern wrapper slice(const uint8_t* arr, int size, uint8_t include, uint8_t exclude);
extern uint16_t modbus_crc(uint8_t buf[], int len);

#endif
