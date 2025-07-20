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
static uint32_t runMode = 0;                                                        // The mode this is running in.

/**
 * @brief      Initialise the Alternate Core VSync Handler.
 */
void CORInitialise(void) {
    static bool isInitialised = false;                                              // Only initialise once.
    if (isInitialised) return;
    isInitialised = true;
    altcoreEnabled = false;                                                         // Initially disable
    handlerCount = 0;                                                               // No handlers installed
    COMInitialise();                                                                // Initialise common
    USBInitialise();
    VMDInitialise();                                                                // Initialise modes (which initialises DVI)
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
        runMode = vi.mode;                                                          // Remember the mode it is set up on.
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
#ifndef RUNTIME
void CORCore1Main(void) {
    verticalSyncOccurred = false;                                                   // Forces wait one frame.
    while (COMAppRunning() && altcoreEnabled) {                                     // Until runtime has stopped or altcore has been stopped
        if (runMode != vi.mode) {                                                   // If the mode has changed, stop running this core.
            altcoreEnabled = false;
        }
        if (verticalSyncOccurred && altcoreEnabled) {                               // Wait for VSYNC
            verticalSyncOccurred = false;
            for (int i = 0;i < handlerCount;i++) {                                  // When occurs call all the handlers.
                (*handlers[i])(false);
            }
        }
    }    
}
#endif

/**
 * @brief      This is used by the runtime - it calls all the handlers, but when the runtime yields to refresh the display.
 */
#ifdef RUNTIME
void CORExecuteAllHandlers(void) {
    if (runMode != vi.mode) {                                                       // If the mode has changed, stop running this core.
        altcoreEnabled = false;
    }
    if (altcoreEnabled) {                                                           // If enabled, call all the handlers.
        for (int i = 0;i < handlerCount;i++) {                                  
            (*handlers[i])(false);
        }        
    }
}
#endif

