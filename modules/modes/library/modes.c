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
    vi._dviMode = 0x0002;                                                           // Default video mode (640x480x16)

    vi.bytesPerLine = 640 / (vi._dviMode & 15);
    vi.scanLineDivider = 1;
    vi.xScreen = 640;;
    if (vi._dviMode == 0x4001) {
        vi.xScreen = vi.bytesPerLine = 320;
        vi.scanLineDivider = 2;  
    } 
    if (vi._dviMode == 0x8001) {
        vi.xScreen = vi.bytesPerLine = 160;
    }
    vi.yScreen = 480 / vi.scanLineDivider;

    DVISetMode(vi._dviMode);            
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