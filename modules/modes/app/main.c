// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Video mode module test program
//      Date :      7th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "common_module.h"
#include "dvi_module.h"
#include "modes_module_local.h"

uint8_t framebuffer[640*480];
static uint16_t modeInformation = 1;

static uint8_t *_DVIGetDisplayLine(uint16_t scanLine) {
    #ifdef RENDER_240
    scanLine = scanLine >> 1;
    #endif
    return framebuffer + scanLine * 640;
}

int MAINPROGRAM(int argc,char *argv[]) {
    DVIInitialise();                                                                // Initialise the DVI system.
    DVISetLineAccessorFunction(_DVIGetDisplayLine);                                 // Set callback to access line memory.
    DVISetMode(modeInformation);                                                    // Switches mode at next top of frame                                              
    while (COMAppRunning()) {                                                                     
        memset(framebuffer,random(),640*480);     
        YIELD();                                                                    // This is for the runtime library.s
        uint32_t next = COMClockMS() + 1000;
        while (COMClockMS() < next) {}
    }
	return 0;
}

