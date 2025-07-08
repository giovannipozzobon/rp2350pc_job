// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      modes.c
//      Purpose :   Video mode library main.
//      Date :      8th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "modes_module.h"
#include "modes_module_local.h"

VIDINFO vi;

/**
 * @brief      Display line driver.
 *
 * @param[in]  scanLine  Current Scan Line
 *
 * @return     Address of line to render
 */
static uint8_t *VMDGetDisplayLine(uint16_t scanLine) {
    if (vi.displaySurface == NULL) return NULL;    
    return vi.displaySurface + (scanLine / vi.scanLineDivider) * vi.bytesPerLine;
}

/**
 * @brief      Initialise the modes library
 */
void VMDInitialise(void) {
    DVIInitialise();                                                                // Initialise the DVI system.
    DVISetLineAccessorFunction(VMDGetDisplayLine);                                  // Set callback to access line memory.

    vi.displaySurface = vi.drawSurface = NULL;                                      // No draw or display surface, yet.
}

/**
 * @brief      Change the display mode.
 *
 * @param[in]  mode  The modes are listed in modes_module.h. These are *not* the
 *                   mode values used in the DVI module.
 */
void VMDSetMode(uint32_t mode) {
    VMDModeSetupInformation(mode);                                                  // Set the structures
    DVISetMode(vi._dviMode);                                                        // Configure HSTX
}

/**
 * @brief      Set the video memory usage
 *
 * @param      memory  video memory address
 * @param[in]  size    byte size of vram
 */
void VMDSetVideoMemory(uint8_t *memory,uint32_t size) {
    vi.drawSurface = vi.displaySurface = memory;
}