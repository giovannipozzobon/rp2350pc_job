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

    VMDInitialise();                                                        // Initialise
    VMDSetVideoMemory(vRAM,sizeof(vRAM));                                   // Set video ram and size
    VMDSetMode(MODE_320_240_256);                                           // Set mode.

    while (COMAppRunning()) {                                                                     
        vi.drawSurface[random()%(640*480)] = (random() & 255) ? 0:random();  
        YIELD();                         
    }
    return 0;
}

