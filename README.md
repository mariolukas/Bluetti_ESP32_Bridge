## About
This is an ESP32 based Bluetooth to MQTT Bride for BLUETTI power stations. The project is based on https://github.com/warhammerkid/bluetti_mqtt
The code is tested on a AC300. Other Powerstations should also work but are untested yet. The discussion on https://diysolarforum.com/threads/monitoring-bluetti-systems.37870/ was a great help for understanding the protocol. 

## Community
Join the Discord Server https://discord.gg/fWDSBTCVmB

## Features

* easy configuration with WiFi manager
* display support OLED 128x64 
  * tested ESP32 WROOM with display: https://github.com/LilyGO/TTGO-T2-ESP32
* mqtt support
* support for BLUETTI power stations
  * AC300 (tested)
  * AC200 (tested)
  * EB3A (tested)
  * EP500 (untested)
  * EP500P (tested)
  * EP600 (some values still missing)
* supported BLUETTI functions
  * commands
    * ac output on/off
    * dc output on/off
  * states
    * ac input power
    * dc input power
    * ac output power
    * dc output power
    * dsp firmware version
    * arm firmware version
    * serial number
    * device type
    * power generation
    * total battery percent

## Getting Started

### Configuration

Create a copy of config.sample.h and name it config.h
Change at least the device type to fit your Bluetti device.

### Compiling and Flashing to ESP32

#### Arduino IDE

You will need to install a board support package for your ESP32. Additionally the following libraries are needed: 

* https://github.com/tzapu/WiFiManager
* https://github.com/knolleary/pubsubclient
* https://github.com/ayushsharma82/ElegantOTA
* https://github.com/me-no-dev/ESPAsyncWebServer
* https://github.com/me-no-dev/AsyncTCP/archive

Change the partition scheme with Tools -> Partition Scheme to

* Minimal SPIFFS (1.9 MB App with OTA/ 190KB SPIFFS)

![Wifi Manager start menu](doc/images/partition.png)

This setting is required because the Bluetooth stack already uses a lot of the ESP32 memory.

Optional: Do changes in config.h file. The device can be set by changing 'BLUETTI_TYPE'.

Finally upload the Sketch to your ESP32.

*INFO*: Until now only BLUETTI_AC300, BLUETTI_EP500P was tested. If you own one of the supported devices please let me know if it works.

#### PlatformIO

Compiling
```
$ pio run
```

Flashing Factory Image
```
$ esptool.py write_flash 0x0 build/Bluetti_ESP32_Bridge.factory.bin
```

Updating only App (don't delete settings)
```
# Write Partition A
$ esptool.py write_flash 0x10000 build/Bluetti_ESP32_Bridge.ota.bin
...
# Write Partition B
$ esptool.py write_flash 0x1F0000 build/Bluetti_ESP32_Bridge.ota.bin
```

The configuration interface also offers OTA updates. You can flash also `build/Bluetti_ESP32_Bridge.ota.bin` there. If you already configured your device you can use `http://<ip_address/update` to update your firmware (no-reconfiguration needed)

### Usage

Just connect the ESP32 to a power supply or the power station itself. Connect your mobile phone or computer
to the Wifi mangaer of the ESP32 firmware, it should show up as "ESP32_Bluetti".

After the connection to the hotspot is established a wifi manager popup will appear. Otherwise
open 192.168.4.1 in your local webbrowser.

Select "Configure WiFi"

![Wifi Manager start menu](doc/images/wifi_manager.png)

Configure your WiFi and set the address of your MQTT server and enter the Bluetooth ID of your
Bluetti Device. Optionally you can specify username and password to protect the web OTA interface.
You can use a mobile phone and/or the Bluetti APP for finding the correct Bluetooth ID of your device.

![Wifi Manager start menu](doc/images/wifi_setup.png)

Save the settings. The ESP32 starts sending messages to your MQTT server.

Example ( ioBroker ):
![MQTT ioBroker](doc/images/iobroker.png)

### MQTT Topics

#### Commands
Commands are subscribed from

* /bluetti/<your_device_id>/command
  * ac_output_on
  * dc_output_on

#### State
States are published to
* /bluetti/<your_device_id>/state
  * ac_output_on
  * dc_output_on
  * dc_input_power
  * ac_input_power
  * ac_output_power
  * dc_output_power
  * serial
  * dsp_version
  * arm_version
  * power_generation
  * total_battery_percent

## Display
Config Display:
* By default, display is disabled. 
* Configurations (customize of file Bluetti_ESP32/config.h): 
  * Enable display: uncomment #define DISPLAYSSD1306 1
  * Enable reset of display on init: uncomment DISPLAY_RST_PORT
    * Known needed for LoRa TTGO v1.0
  * set SCL & SDA ports: default ports are set to SCL=4 & SDA5, to change update DISPLAY_SCL_PORT and DISPLAY_SDA_PORT 

Display functionality:
* Show current assiged IP address (AP mode or normal)
* Show different wifi connection logo, depending on the mode its in and wifi Strength in normal mode (4 bars)
* Show the running time of the device in the format "11d12h15m" Currently max until 49 days as this is the time millis() is reset. 
* Show status message, currently shows the init and running status, also BLEscan when scanning 
* a progressbar is available but currently not used anywhere. (to see where it can be used)
* Show bluetooth icon status. Connected is static, blinking is trying to connect, together with message in case of scanning.
* Show MQTT icon status. Connected is static, blinking is trying to connect.

Example display screen:
![DisplayImage](doc/images/display.jpg)


## TODO

* add full feature set to device files
* adding support for SD-Card reader, for writing csv data to an sd-card
* adding logging poll commands

## Disclaimer

The code within this repository comes with no guarantee, use it on your own risk.

Don't touch these firmwares if you don't know how to put the device in the programming mode if something goes wrong.
As per the GPL v3 license, I assume no liability for any damage to you or any other person or equipment.
