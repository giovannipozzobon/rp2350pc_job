// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Sprite test program
//      Date :      27th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "sprites_module.h"

uint8_t vRAM[320*240];

int MAINPROGRAM() {

    USBInitialise();
    VMDInitialise();                                                        // Initialise

    VMDSetVideoMemory(vRAM,sizeof(vRAM));                                   // Set video ram and size
    VMDSetMode(MODE_320_240_256);                                           // Start in 320x240x256

    for (int i = 0;i < 320*240;i++) {                                       // Random dots.  
        vRAM[i] = (random() & (320*240)) ? 0:random();  
    } 

    while (COMAppRunning()) {                                                                     
        COMUpdate();                         
    }
    return 0;
}

