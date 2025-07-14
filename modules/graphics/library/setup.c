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
        GFXInitialiseDrawStructure();
    }
}

/**
 * @brief      Initialise the draw structure
 */
void GFXInitialiseDrawStructure(void) {
    draw.foreground = 0xFF;draw.background = 0;                                 // Default foreground/background colour.
    draw.isTransparent = false;

    draw.xFontScale = draw.yFontScale = 1;                                      // Font scalars
    draw.mapper = NULL;                                                         // No coordinate mapper.
    draw.font = GFXGetSystemCharacter;                                          // Default font.
    GFXResetClipping();                                                         // No clipping         
    GFXRawMove(0,0);                                                            // Move to the home position.

    for (int i = 0;i < 3;i++) {                                                 // Clear the previous coordinates arrays.
        draw.xPrev[i] = draw.yPrev[i] = 0;
    }
}

/**
 * @brief      Reset all clipping.
 */
void GFXResetClipping(void) {
    draw.clip = NULL;                                                               // No clipping.
}
        
