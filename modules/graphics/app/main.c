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

    while (COMAppRunning()) {                                                                     
        vi.drawSurface[random()%(640*480)] = (random() & 31) ? 0:random();  
        YIELD();                         
    }
    return 0;
}

