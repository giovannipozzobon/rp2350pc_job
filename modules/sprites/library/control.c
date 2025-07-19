// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      control.c
//      Purpose :   Sprite outer control program.
//      Date :      19th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "sprites_module.h"
#include "sprites_module_local.h"

static bool spritesEnabled = true;

/**
 * @brief      Start the sprite rendering core working
 */
void SPRStart(void) {
    // TODO: Check 2 buffers available first
    #ifndef RUNTIME
    multicore_launch_core1(SPRCore1Main);
    #endif
    vi.displaySurface = vi.buffer[1];
    spritesEnabled = true;
}

/**
 * @brief      The main program for the sprite rendering core - not used in the runtime.
 */
void SPRCore1Main(void) {
    verticalSyncOccurred = false;                                                   // Forces wait one frame.
    while (COMAppRunning()) {
        if (verticalSyncOccurred) {
            verticalSyncOccurred = false;
            SPRVerticalSyncRoutine();
        }
    }    
}

