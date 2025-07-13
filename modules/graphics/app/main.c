// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Graphics module test program.
//      Date :      9th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

uint8_t vRAM[640*480];

int MAINPROGRAM() {

    VMDInitialise(); 
    VMDSetVideoMemory(vRAM,sizeof(vRAM));                                   // Set video ram and size

    GFXDraw(Mode,MODE_640_480_256,0);                                       // Set mode.

    int speckle = 640*480;
    speckle = 320;
    for (int x = 0;x < 160;x++) {
        for (int y = 2;y < 9;y++) {
            //vRAM[x+y*vi.bytesPerLine] = 0x11*(x / 10);
        }
    }
    draw.foreground = 0xE0;
    GFXDraw(Move,120,130);GFXDraw(Rect,570,400);
    draw.foreground = 0xFF;
    
    draw.xLeft = 120;draw.yTop = 130;draw.xRight = 570;draw.yBottom = 400;
    GFXDraw(Move,0,0);GFXDraw(Line,639,479);
    GFXDraw(Move,0,140);GFXDraw(Line,639,140);
    while (COMAppRunning()) {                                                                     
        vi.drawSurface[random()%speckle] = (random() & 1) ? 0:random();     // Knows we aren't crashing. May consider flashing LED :)
        GFXDraw(Colour,random() & 0xFF,0);
        // GFXDraw(Plot,random() % 640,random() % 480);
        YIELD();                         
    }
    return 0;
}

