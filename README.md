# Bluetti_ESP32
Bluetti Power Station ESP32 Bluetooth to MQTT Bridge

## Description
This is an ESP32 based Bluetooth to MQTT Bride for Bluetti Powerstations. The project is based on https://github.com/warhammerkid/bluetti_mqtt
The code is tested on a AC300. Other Powerstations should also work but are untested yet. The discussion on https://diysolarforum.com/threads/monitoring-bluetti-systems.37870/ was a great help for understanding the protocol. 

## HOWTO USE

You will need to install a board support package for your ESP32. Additionally the follwing libraries are needed: 

* https://github.com/tzapu/WiFiManager
* https://github.com/knolleary/pubsubclient

Compile with partition scheme set to:
 	
* Huge App (3MB No OTA/1MB SPIFFS)

This setting is required because the Bluetooth stack eats a lot of memory. 

Optional: Do changes in config.h file. The device can be set by changing 'BLUETTI_TYPE'. 

*INFO*: Until now only BLUETTI_AC300 is supported.


## TODO

* adding addional Device_xxx.h files
* adding a web interface 
* adding logging poll commands


## Disclaimer

The code within this repository comes with no guarantee, use it on your own risk.

Don't touch these firmwares if you don't know how to put the device in the programming mode if something goes wrong.
As per the GPL v3 license, I assume no liability for any damage to you or any other person or equipment.