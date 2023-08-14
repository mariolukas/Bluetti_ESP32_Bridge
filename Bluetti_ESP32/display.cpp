#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"
#include "display.h"


// used for millis loops
const unsigned long flProgressBar = 200; // speed of update progressbar
const unsigned long flWifiBTStarting = 500; //flash speed Of wifi & BT icon on starting 
const unsigned long flWifiSignal = 5000; //frequency updating wifi signal strength icon
unsigned long prevTimerProgressBar = 0;
unsigned long prevTimerWifiStarting = 0;
unsigned long prevTimerBtStarting = 0;
unsigned long prevTimerRuntime = 0;
unsigned long prevTimerWifiSignal = 0;

// Used variables
int progress = 0;
bool btConnected = false;
byte byteWifiMode;
int intWifiSignal;
byte year = 0;
byte hours = 0;
byte minutes = 0;
byte days = 0;
bool enableProgressbar=false;
String strdispIP = "254.254.254.254";
String strdispStatus="NA";
byte prevStateIcons = 0;
byte prevBTStateIcons = 0;



#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void initDisplay()
{
    #if DISPLAY_RST_PORT
        // Reset required for some displays like LoRa TTGO v1.0
        pinMode(DISPLAY_RST_PORT, OUTPUT);
        digitalWrite(DISPLAY_RST_PORT, HIGH);
        delay(20);
        digitalWrite(DISPLAY_RST_PORT, LOW);
        delay(20);
        digitalWrite(DISPLAY_RST_PORT, HIGH);
        delay(20);
    #endif
    Wire.begin(DISPLAY_SDA_PORT, DISPLAY_SCL_PORT);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false))
    {
        Serial.println(F("display: SSD1306 allocation failed"));
        for (;;)
            ;
    }
    display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(BLACK,WHITE);
    display.setCursor(1, 1);
    display.drawRect(0,0,73,10,WHITE);
    display.println("BlueTTI Wifi");
    wrDisp_IP();
    wrDisp_Running();
    wrDisp_Status("Init....");
    btConnected = false;   
    byteWifiMode = 0;
}
void handleDisplay()
{
    // progress bar
    if (enableProgressbar == true)
    {
        if (millis() - prevTimerProgressBar >= flProgressBar)
        {
            progress = (progress + 5) % 110;
            drawProgressbar(10, 59, 108, 5, progress);
            prevTimerProgressBar = millis();
        }
    }
    // update running time every 5 seconds
    if (millis() - prevTimerRuntime >= 5000)
    {
        wrDisp_Running();
        prevTimerRuntime = millis();
    }
    // blue not connected is blinking
    if (btConnected == false)
    {
        if (millis() - prevTimerBtStarting >= flWifiBTStarting)
        {
            if (prevBTStateIcons == 0)
            {
                // Flash background white
                prevBTStateIcons = 1;
                blueToothSignal(btConnected);
            }
            else
            {
                // flash background Black
                prevBTStateIcons = 0;
                blueToothSignal(btConnected);
            }
            prevTimerBtStarting = millis();
        }
    }
    if (byteWifiMode == 0)
    {
        if (millis() - prevTimerWifiStarting >= flWifiBTStarting)
        {
            if (prevStateIcons == 0)
            {
                // Flash background white
                prevStateIcons = 1;
                wifisignal(0);
            }
            else
            {
                // flash background Black
                prevStateIcons = 0;
                wifisignal(0);
            }
            prevTimerWifiStarting = millis();
        }
    } else
    {
        if (millis() - prevTimerWifiSignal >= flWifiSignal)
        {
            wifisignal(byteWifiMode, intWifiSignal); 
            prevTimerWifiSignal = millis();
        }
    }

}
void wrDisp_IP(String strIP)
{
    display.fillRect(0,14,114,8,0);
    display.setTextColor(WHITE,BLACK);
    display.setCursor(0, 14);
    display.println("IP:"+ strIP);
    display.display();
}
void wrDisp_Running()
{
    display.fillRect(0,22,114,8,0);
    display.setTextColor(WHITE,BLACK);
    display.setCursor(0, 22);
    // important: millis resets after 49days - currently not taken into account in the calculations
    // example output: 365d23h60m
    
    if (((millis()/1000) > 60) && (millis()/1000 <3600))
    {
        
        minutes = ((int)((millis()/1000)/60));
    } else if ((millis()/1000 > 3600) && (millis()/1000 <86400))
    {
        
        hours = ((int)(millis()/1000)/3600); 
        minutes = ((int)(((millis()/1000)-(hours*3600))/60));
    } else if ((millis()/1000 > 86400))
    {
        days = ((int)(millis()/1000)/86400);
        hours = ((int)(((millis()/1000)/3600)-((days*24)))); 
        minutes = ((int)(((millis()/1000)-(hours*3600))/60));
    }
    display.println("Runtime: "+ String(days)+"d"+String(hours)+"h"+String(minutes)+"m"); //running time will be max 49days until millis is reset
    display.display();
}
void wrDisp_Status(String strStatus)
{
    display.fillRect(0,30,114,8,0);
    display.setTextColor(WHITE,BLACK);
    display.setCursor(0, 30);
    display.println("Status:" + strStatus);
    display.display();
}
void blueToothSignal(bool blConnected)
{
    display.fillRect(115, 18, 13, 13, 0);
    display.display();
    if (blConnected == true)
    {
        display.writePixel(118, 21, 1);
        display.writePixel(118, 27, 1);
        display.writePixel(119, 22, 1);
        display.writePixel(119, 26, 1);
        display.writePixel(120, 25, 1);
        display.writePixel(120, 23, 1);
        display.drawLine(121, 19, 121, 29, 1);
        display.writePixel(122, 19, 1);
        display.writePixel(122, 24, 1);
        display.writePixel(122, 29, 1);
        display.writePixel(123, 20, 1);
        display.writePixel(123, 23, 1);
        display.writePixel(123, 25, 1);
        display.writePixel(123, 28, 1);
        display.writePixel(124, 21, 1);
        display.writePixel(124, 22, 1);
        display.writePixel(124, 26, 1);
        display.writePixel(124, 27, 1);
        display.display();
    }
    else
    {
        display.writePixel(118, 21, prevBTStateIcons);
        display.writePixel(118, 27, prevBTStateIcons);
        display.writePixel(119, 22, prevBTStateIcons);
        display.writePixel(119, 26, prevBTStateIcons);
        display.writePixel(120, 25, prevBTStateIcons);
        display.writePixel(120, 23, prevBTStateIcons);
        display.drawLine(121, 19, 121, 29, prevBTStateIcons);
        display.writePixel(122, 19, prevBTStateIcons);
        display.writePixel(122, 24, prevBTStateIcons);
        display.writePixel(122, 29, prevBTStateIcons);
        display.writePixel(123, 20, prevBTStateIcons);
        display.writePixel(123, 23, prevBTStateIcons);
        display.writePixel(123, 25, prevBTStateIcons);
        display.writePixel(123, 28, prevBTStateIcons);
        display.writePixel(124, 21, prevBTStateIcons);
        display.writePixel(124, 22, prevBTStateIcons);
        display.writePixel(124, 26, prevBTStateIcons);
        display.writePixel(124, 27, prevBTStateIcons);
        display.display();
    }
}
void wifisignal(int intMode, int intSignal)
{
    // intMode:
    // 0, not connected
    // 1, connected
    // 2, AP mode

    // -55 or higher: 4 bars
    // -56 to -66: 3 bars
    // -67 to -77: 2 bars
    // -78 to -88: 1 bar
    // -89 or lower: 0 bars -> not implemented
    display.fillRect(115, 0, 13, 13, 0);
    display.display();
    byte textColor = 1; // 1 for White and 0 for black
    if (intMode == 1)
    {
        if (textColor == 0)
        {
            // Black on white
            display.fillRect(115, 0, 13, 13, 1);
            display.display();
        } else
        {
            // White on black
            display.fillRect(115, 0, 13, 13, 0);
            //display.drawRect(115, 0, 13, 13, 1);
            display.display();

        }
        if ((intSignal > -88) && (intSignal <= -78))
        {
            // extreme weak signal 1 bar
            display.writePixel(121, 11, textColor);
        } else if ((intSignal > -77) && (intSignal <= -67))
        {
            // 2 bars

            // one bar
            display.writePixel(121, 11, textColor);
            // 2 bar
            display.writePixel(119, 9, textColor);
            display.writePixel(123, 9, textColor);
            display.writePixel(120, 8, textColor);
            display.writePixel(121, 8, textColor);
            display.writePixel(122, 8, textColor);
        }  else if ((intSignal > -66) && (intSignal <= -56))
        {
            // 3 bars

            // one bar
            display.writePixel(121, 11, textColor);
            // 2 bar
            display.writePixel(119, 9, textColor);
            display.writePixel(123, 9, textColor);
            display.writePixel(120, 8, textColor);
            display.writePixel(121, 8, textColor);
            display.writePixel(122, 8, textColor);
            // 3 bar
            display.writePixel(118, 6, textColor);
            display.writePixel(118, 6, textColor);
            display.writePixel(124, 6, textColor);
            display.writePixel(119, 5, textColor);
            display.writePixel(120, 5, textColor);
            display.writePixel(121, 5, textColor);
            display.writePixel(122, 5, textColor);
            display.writePixel(123, 5, textColor);
        }
        else if (intSignal > -55)
        {
            // 4 bars

            // one bar
            display.writePixel(121, 11, textColor);
            // 2 bar
            display.writePixel(119, 9, textColor);
            display.writePixel(123, 9, textColor);
            display.writePixel(120, 8, textColor);
            display.writePixel(121, 8, textColor);
            display.writePixel(122, 8, textColor);
            // 3 bar
            display.writePixel(118, 6, textColor);
            display.writePixel(118, 6, textColor);
            display.writePixel(124, 6, textColor);
            display.writePixel(119, 5, textColor);
            display.writePixel(120, 5, textColor);
            display.writePixel(121, 5, textColor);
            display.writePixel(122, 5, textColor);
            display.writePixel(123, 5, textColor);
            // 4 bar
            display.writePixel(116, 4, textColor);
            display.writePixel(126, 4, textColor);
            display.writePixel(117, 3, textColor);
            display.writePixel(125, 3, textColor);
            display.writePixel(118, 2, textColor);
            display.writePixel(119, 2, textColor);
            display.writePixel(120, 2, textColor);
            display.writePixel(121, 2, textColor);
            display.writePixel(122, 2, textColor);
            display.writePixel(123, 2, textColor);
            display.writePixel(124, 2, textColor);
        }
        display.display();   
    }
    else if (intMode == 0)
    {
        
        if (prevStateIcons == 0)
        {
            //display.fillRect(115, 0, 13, 13, 1);
        }
        else
        {
            //display.fillRect(115, 0, 13, 13, 0);
        }
        // one bar
        display.writePixel(121, 11, prevStateIcons);
        // 2 bar
        display.writePixel(119, 9, prevStateIcons);
        display.writePixel(123, 9, prevStateIcons);
        display.writePixel(120, 8, prevStateIcons);
        display.writePixel(121, 8, prevStateIcons);
        display.writePixel(122, 8, prevStateIcons);
        // 3 bar
        display.writePixel(118, 6, prevStateIcons);
        display.writePixel(118, 6, prevStateIcons);
        display.writePixel(124, 6, prevStateIcons);
        display.writePixel(119, 5, prevStateIcons);
        display.writePixel(120, 5, prevStateIcons);
        display.writePixel(121, 5, prevStateIcons);
        display.writePixel(122, 5, prevStateIcons);
        display.writePixel(123, 5, prevStateIcons);
        // 4 bar
        display.writePixel(116, 4, prevStateIcons);
        display.writePixel(126, 4, prevStateIcons);
        display.writePixel(117, 3, prevStateIcons);
        display.writePixel(125, 3, prevStateIcons);
        display.writePixel(118, 2, prevStateIcons);
        display.writePixel(119, 2, prevStateIcons);
        display.writePixel(120, 2, prevStateIcons);
        display.writePixel(121, 2, prevStateIcons);
        display.writePixel(122, 2, prevStateIcons);
        display.writePixel(123, 2, prevStateIcons);
        display.writePixel(124, 2, prevStateIcons);
        // not connected / trying to connect
        // wifi logo should blink, trying to make connection
        display.display();
    }
    else if (intMode == 2)
    {
        // AP mode
        // wifi logo should contain AP as text
        display.fillRect(115, 0, 13, 13, 1);
        display.setTextColor(BLACK, WHITE);
        display.setCursor(116, 3);
        display.print("AP");
        display.display();
    }


}
void disp_setWifiSignal(int extWifMode, int extSignal)
{
    intWifiSignal = extSignal;
    byteWifiMode = extWifMode;
    wifisignal(extWifMode,extSignal);
}
void disp_setWifiMode(byte wMode)
{
    byteWifiMode = wMode;
}
void disp_setIP(String strIP)
{
    if (strIP != strdispIP)
    {
        wrDisp_Status(strIP);
        strdispIP = strIP;
    }
}
void disp_setStatus(String strStatus)
{
    if (strStatus != strdispStatus)
    {
        wrDisp_Status(strStatus);
        strdispStatus = strStatus;
    }
}
void disp_setBlueTooth(bool boolBtConn)
{
    btConnected = boolBtConn;
}
void disp_setPrevStateIcon(byte bytePrevState)
{
    prevStateIcons = bytePrevState;
}
void drawProgressbar(int x,int y, int width,int height, int progress)
{

   // clear old data
   //display.drawRect(x, y, width, height, BLACK);
   display.fillRect(x, y, width , height, BLACK);
   display.display();
   
   progress = progress > 100 ? 100 : progress;
   progress = progress < 0 ? 0 :progress;

   float bar = ((float)(width-1) / 100) * progress;
 
   //display.drawRect(x, y, width, height, WHITE);
   display.fillRect(x, y, bar , height, WHITE);
   display.display();
}
