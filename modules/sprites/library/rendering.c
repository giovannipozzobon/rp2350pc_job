// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      rendering.c
//      Purpose :   Sprite rendering
//      Date :      19th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "sprites_module.h"
#include "sprites_module_local.h"

/**
 * @brief      The code executed at VSYNC when sprites are enabled.
 */
void SPRVerticalSyncRoutine(void) {
    static int vPos = 0;    
    vPos = (vPos+1) & 0x7F;
    memcpy(vi.displaySurface,vi.drawSurface,vi.bufferSize);
    for (int i = 0;i < 10000;i++) {
        vi.displaySurface[random() % (320*20)+vPos*320] = random();
    }
}

