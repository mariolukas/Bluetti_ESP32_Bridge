#ifndef BDISPLAY_H
#define BDISPLAY_H
#include "Arduino.h"
#include "config.h"

extern void initDisplay();
extern void handleDisplay();

// refresh values every DISPLAY_WRITE_DELAY milliseconds
#define DISPLAY_WRITE_DELAY 2000
// switch/rotate pages after REFRESHES_PER_PAGE refreshes
#define REFRESHES_PER_PAGE 2
// amount of pages to be displayed in rotation
#define PAGES_TO_DISPLAY 2

#endif
