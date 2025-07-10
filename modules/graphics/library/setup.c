// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      setup.c
//      Purpose :   Graphics module setup
//      Date :      9th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

/**
 * @brief      Initialise the graphics system.
 */
void GFXInitialise(void) {
    static bool isInitialised = false;                                              // Only initialise once.
    if (isInitialised) return;
    isInitialised = true;
    VMDInitialise();                                                                // Initialise
    draw.currentMode = 0xFFFFFFFF;                                                  // Effectively "no current mode set"
}

/**
 * @brief      Called to check if the mode has changed, it sets up the draw system with the default.
 */
void GFXCheckModeChange(void) {
    if (vi.mode != draw.currentMode) {                                              // Has the mode changed, if so reset graphics for it.        
        draw.currentMode = vi.mode;                                                 // Update the current mode.

        draw.xLeft = draw.yTop = 0;                                                 // Set the physical draw window to the whole physical screen
        draw.xRight = vi.xScreen-1;draw.yBottom = vi.yScreen-1;                     // This window is inclusive coordinates.

        draw.foreground = 0xFF;draw.background = 0;                                 // Default foreground/background colour.
        GFXRawMove(0,0);                                                            // Move to the home position.
    }
}