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


static bool altcoreEnabled = false;                                                 // true if alt core is active. 
static uint8_t handlerCount = 0;                                                    // number of active handlers
static CORVSYNCHANDLER handlers[MAXVSYNCHANDLER];                                   // handlers defined.

/**
 * @brief      Initialise the Alternate Core VSync Handler.
 */
void CORInitialise(void) {
    altcoreEnabled = false;                                                         // Initially disable
    handlerCount = 0;                                                               // No handlers installed
}

/**
 * @brief      Start the alt core working
 */
void CORStart(void) {
    if (!altcoreEnabled) {                                                          // Already started
        altcoreEnabled = true;                                                      // Mark as enabled
        #ifndef RUNTIME
        multicore_launch_core1(CORCore1Main);                                       // Run on 2nd core on real hardware only
        #endif
    }
}

/**
 * @brief      Add a handler to be called on VSYNC
 *
 * @param[in]  handler  Handler function.
 */
void CORAdd(CORVSYNCHANDLER handler) {
    if (handlerCount < MAXVSYNCHANDLER) {                                           // Add to handler list if possible
        handlers[handlerCount++] = handler;
        (*handler)(true);                                                           // And initialise the handler.
    }
}
/**
 * @brief      Stop the alt core working
 */
void CORStop(void) {
    if (altcoreEnabled) {
        altcoreEnabled = false;                                                     // Setting to false terminates the core 1 loop.
    }
}

/**
 * @brief      The main program for the sprite rendering core - not used in the runtime.
 */
void CORCore1Main(void) {
    verticalSyncOccurred = false;                                                   // Forces wait one frame.
    while (COMAppRunning() && altcoreEnabled) {                                     // Until runtime has stopped or altcore has been stopped
        if (verticalSyncOccurred) {                                                 // Wait for VSYNC
            verticalSyncOccurred = false;
            for (int i = 0;i < handlerCount;i++) {                                  // When occurs call all the handlers.
                (*handlers[i])(false);
            }
        }
    }    
}

