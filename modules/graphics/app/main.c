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
            vRAM[x+y*vi.bytesPerLine] = 0x11*(x / 10);
        }
    }

    GFXDraw(Colour,0xFC,0x03);                                              // Yellow characters on Blue background
    GFXDraw(Move,40,140);                                                   // Move cursor
    GFXDraw(Scaling,2,3);                                                   // Font scaling to x 2 width x 3 height
    for (int i = 33;i < 99;i++) {                                           // Write characters
        GFXDraw(Character,i,0);
    }
    GFXDraw(Colour,0xE0,0xFFFF);                                            // This is Red (RRRGGGBB) on transparent

    GFXDraw(Move,40,140);                                                   // This shows the sequence draw. Go here. Line to here.
    GFXDraw(Line,639,479);

    while (COMAppRunning()) {                                                                     
        vi.drawSurface[random()%speckle] = (random() & 1) ? 0:random();     // Knows we aren't crashing. May consider flashing LED :)
        YIELD();                         
    }
    return 0;
}

