// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      control.c
//      Purpose :   AltCore outer control program.
//      Date :      19th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "altcore_module.h"
#include "altcore_module_local.h"

static bool altcoreEnabled = false;

/**
 * @brief      Initialise the Alternate Core VSync Handler.
 */
void CORInitialise(void) {
    altcoreEnabled = false;
}

/**
 * @brief      Start the sprite rendering core working
 */
void CORStart(void) {
    if (!altcoreEnabled) {
        altcoreEnabled = true;
        VerticalSyncRoutine(true);
        #ifndef RUNTIME
        multicore_launch_core1(CORCore1Main);
        #endif
    }
}

void CORStop(void) {
    if (altcoreEnabled) {
        altcoreEnabled = false;
    }
}

/**
 * @brief      The main program for the sprite rendering core - not used in the runtime.
 */
void CORCore1Main(void) {
    verticalSyncOccurred = false;                                                   // Forces wait one frame.
    while (COMAppRunning() && altcoreEnabled) {
        if (verticalSyncOccurred) {
            verticalSyncOccurred = false;
            VerticalSyncRoutine(false);
        }
    }    
}

