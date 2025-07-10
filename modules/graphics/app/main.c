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
    VMDSetMode(MODE_640_480_16);                                           // Set mode.

    int speckle = 640*480;
    speckle = 320;

    for (int x = 0;x < 100;x++) {
        GFXRawMove(x+2,x*2+10);
        GFXRawPlot();
    }
    while (COMAppRunning()) {                                                                     
        vi.drawSurface[random()%speckle] = (random() & 1) ? 0:random();  
        YIELD();                         
    }
    return 0;
}

