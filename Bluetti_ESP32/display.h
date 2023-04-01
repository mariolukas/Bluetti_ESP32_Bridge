#ifndef DISPLAY_H
#define DISPLAY_H

// for setup only
void initDisplay();
void wrDisp_IP(String strIP="254.254.254.254");
void wrDisp_Running();
void wrDisp_Status(String strStatus="NoStatus");
void drawProgressbar(int x,int y, int width,int height, int progress);
void blueToothSignal(bool blConnected=false);

// can be used both is setup and in loop
void wifisignal(int intMode=0, int intSignal=-100);
void disp_setPrevStateIcon(byte bytePrevState);

// public usable on loops
void handleDisplay();

void disp_setWifiMode(byte wMode);
void disp_setBlueTooth(bool boolBtConn=false);
void disp_setWifiSignal(int extWifMode=0, int extSignal=-100);
void disp_setStatus(String strStatus);
void disp_setIP(String strIP);
#endif
#include <arduino.h>