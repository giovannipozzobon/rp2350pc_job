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

    GFXInitialise(); 
    VMDSetVideoMemory(vRAM,sizeof(vRAM));                                   // Set video ram and size
    VMDSetMode(MODE_640_480_16);                                            // Set mode.

    int speckle = 640*480;
    speckle = 320;

    for (int x = 0;x < 160;x++) {
        for (int y = 2;y < 9;y++) {
            vRAM[x+y*vi.bytesPerLine] = 0x11*(x / 10);
        }
    }

    draw.foreground = 1;
    GFXDraw(Move,10,100);GFXDraw(Rect,240,290);
    GFXDraw(Move,310,120);GFXDraw(FillRect,540,300);
    draw.foreground = 3;
    GFXDraw(Move,10,100);GFXDraw(Ellipse,240,290);
    GFXDraw(Move,310,120);GFXDraw(FillEllipse,540,300);
    while (COMAppRunning()) {                                                                     
        vi.drawSurface[random()%speckle] = (random() & 1) ? 0:random();  
        YIELD();                         
    }
    return 0;
}

