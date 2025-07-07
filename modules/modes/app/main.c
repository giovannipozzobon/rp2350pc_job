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

static uint16_t modeInformation = 1;
uint8_t framebuffer[640*480];
static uint16_t bytesPerLine,scanLineShifter;
static uint16_t xSize,ySize;

static void plotPixel(uint16_t x,uint16_t y,uint8_t colour);

static uint8_t *_DVIGetDisplayLine(uint16_t scanLine) {
    return framebuffer + (scanLine >> scanLineShifter) * bytesPerLine;
}

int MAINPROGRAM() {
    DVIInitialise();                                                                // Initialise the DVI system.
    DVISetLineAccessorFunction(_DVIGetDisplayLine);                                 // Set callback to access line memory.

    modeInformation = 0x4001;
    DVISetMode(modeInformation);                                                    // Switches mode at next top of frame                                              

    memset(framebuffer,0x18,640*480);                                               // Fast screen clear
    for (int i = 0;i < 640*480;i++) framebuffer[i] = (random() & 63) ? 0:random();  // Random dots.

    bytesPerLine = 640 / (modeInformation & 15);
    scanLineShifter = 0;

    xSize = 640;;
    if (modeInformation == 0x4001) {
        xSize = bytesPerLine = 320;
        scanLineShifter = 1;  
    } 
    if (modeInformation == 0x8001) {
        xSize = bytesPerLine = 160;
    }
    ySize = 480 >> scanLineShifter;

    while (COMAppRunning()) {                                                                     
        YIELD();                                                                // This is for the runtime library.s
    }
    return 0;
}

static void plotPixel(uint16_t x,uint16_t y,uint8_t colour) {
    uint8_t *address,mask,shift;
    if (x >= 640 || y >= 480) return;

    switch(modeInformation & 0x0F) {
        case 1:
            framebuffer[x+y*bytesPerLine] = colour;
            break;
        case 2:
            address = framebuffer + (x >> 1) + y * bytesPerLine;
            shift = ((x & 1) == 0) ? 0 : 4;
            mask = 0xF0 >> shift;colour = (colour & 0x0F) << shift;
            *address = ((*address) & mask) | colour;
            break;
        case 4:
            address = framebuffer + (x >> 2) + y * bytesPerLine;
            shift = (x & 3) * 2;
            mask = (0x03 << shift) ^ 0xFF;
            colour = (colour & 3) << shift;
            *address = ((*address) & mask) | colour;
            break;
        case 8:
            address = framebuffer + (x >> 3) + y * bytesPerLine;
            shift = (x & 7);
            *address &= ((0x01 << shift) ^ 0xFF);
            if ((colour & 1) != 0) {
                *address |= (0x01 << shift);
            }
            break;
    }
}
