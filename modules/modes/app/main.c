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

#include "modes_module.h"
#include "modes_module_local.h"

static uint16_t modeInformation = 1;
uint8_t framebuffer[640*480];
static uint16_t bytesPerLine,scanLineDivider;
static uint16_t xSize,ySize;

static void plotPixel(uint16_t x,uint16_t y,uint8_t colour);
static void drawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t colour);

static uint8_t *_DVIGetDisplayLine(uint16_t scanLine) {
    return framebuffer + (scanLine / scanLineDivider) * bytesPerLine;
}

int MAINPROGRAM() {
    DVIInitialise();                                                                // Initialise the DVI system.
    DVISetLineAccessorFunction(_DVIGetDisplayLine);                                 // Set callback to access line memory.

    modeInformation = 0x4001;
    DVISetMode(modeInformation);                                                    // Switches mode at next top of frame                                              

    memset(framebuffer,0x18,640*480);                                               // Fast screen clear
    for (int i = 0;i < 640*480;i++) framebuffer[i] = (random() & 63) ? 0:random();  // Random dots.

    bytesPerLine = 640 / (modeInformation & 15);
    scanLineDivider = 1;

    xSize = 640;;
    if (modeInformation == 0x4001) {
        xSize = bytesPerLine = 320;
        scanLineDivider = 2;  
    } 
    if (modeInformation == 0x8001) {
        xSize = bytesPerLine = 160;
    }
    ySize = 480 / scanLineDivider;

    uint8_t c = 0x7;

    drawLine(10,10,xSize-20,ySize/2,c);
    drawLine(xSize-20,ySize/2,xSize/2,ySize-20,c);
    drawLine(10,10,xSize/2,ySize-20,c);

    while (COMAppRunning()) {                                                                     
        YIELD();                                                                // This is for the runtime library.s
    }
    return 0;
}

#define max(a,b) (((a) > (b)) ? (a):(b))

/**
 * @brief      Draws a line
 *
 * @param[in]  x1      x1
 * @param[in]  y1      y1
 * @param[in]  x2      x2
 * @param[in]  y2      y2
 * @param[in]  colour  colour
 */
static void drawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t colour) {
    int16_t dx = x2-x1;
    int16_t dy = y2-y1;
    int16_t l = max(abs(dx),abs(dy));
    for (int i = 0;i < l;i++) {
        plotPixel(x1+dx*i/l,y1+dy*i/l,colour);
    }
}

/**
 * @brief      Plot a pixel
 *
 * @param[in]  x       x
 * @param[in]  y       y
 * @param[in]  colour  colour
 */
static void plotPixel(uint16_t x,uint16_t y,uint8_t colour) {
    uint8_t *address,mask,shift;
    if (x >= xSize || y >= ySize) return;

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
