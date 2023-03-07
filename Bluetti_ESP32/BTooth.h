#ifndef BTOOTH_H
#define BTOOTH_H
#include "Arduino.h"
#include "NimBLEDevice.h"

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteWriteCharacteristic;
static BLERemoteCharacteristic* pRemoteNotifyCharacteristic;
static BLEAdvertisedDevice* bluettiDevice;

typedef struct __attribute__ ((packed)) {
  uint8_t prefix;            // 1 byte   
  uint8_t field_update_cmd;  // 1 byte 
  uint8_t page;              // 1 byte 
  uint8_t offset;            // 1 byte
  uint16_t len;              // 2 bytes  
  uint16_t check_sum;        // 2 bytes  
} bt_command_t;   


// The remote Bluetti service we wish to connect to.
static BLEUUID serviceUUID("0000ff00-0000-1000-8000-00805f9b34fb");

// The characteristics of Bluetti Devices
static BLEUUID    WRITE_UUID("0000ff02-0000-1000-8000-00805f9b34fb");
static BLEUUID    NOTIFY_UUID("0000ff01-0000-1000-8000-00805f9b34fb");

extern void initBluetooth();
extern void handleBluetooth();
bool connectToServer();
extern void handleBTCommandQueue();
extern void sendBTCommand(bt_command_t command);
extern bool isBTconnected();
extern unsigned long getLastBTMessageTime();
#endif
