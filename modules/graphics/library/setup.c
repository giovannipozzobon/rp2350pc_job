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

struct DrawingState draw;                                                           // Current drawing state

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

        draw.x = draw.y = 0;                                                        // Go to home (e.g. top left)
        draw.inDrawingArea = true;                                                  // Initially we're in the drawing area, it's the full screen.
        draw.displayByte = vi.drawSurface;                                          // Draw surface position, the top left bit.
        draw.currentShift = 0;                                                      // Current shift is zero - remember left pixel is the LS Bits

        draw.background = 0;draw.foreground = vi.pixelsPerByte - 1;                 // Default to black background, all bit '1' in foreground (white)
        draw.mask = draw.background ^ 0xFF;                                         // This is also the inverse of the mask.
        draw.shift = vi.pixelsPerByte;                                              // Amount to adjust currentShift each time for the next/previous byte.
    }
}