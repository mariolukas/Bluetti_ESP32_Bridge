#ifndef DISPLAY_H
#define DISPLAY_H

// for setup only
void initDisplay();
void wrDisp_IP(String strIP="NoConf");
void wrDisp_Running();
void wrDisp_Status(String strStatus="boot..");
void drawProgressbar(int x,int y, int width,int height, int progress);
void wrDisp_blueToothSignal(bool blConnected);
void wrDisp_mqttConnected(bool blMqttConnected=false);
void wrDisp_wifisignal_rewrite_static();

// can be used both is setup and in loop
void wrDisp_wifisignal(int intMode=0, int intSignal=-100);
void disp_setPrevStateIcon(byte bytePrevState);
void disp_setBTPrevStateIcon(byte bytePrevState);

// public usable on loops
void handleDisplay();

void disp_setWifiMode(byte wMode);
void disp_setBlueTooth(bool boolBtConn=false);
void disp_setWifiSignal(int extWifMode=0, int extSignal=-100);
void disp_setStatus(String strStatus);
void disp_setIP(String strIP);
void disp_setMqttStatus(bool blMqttconnected=false);
#endif
#include <arduino.h>