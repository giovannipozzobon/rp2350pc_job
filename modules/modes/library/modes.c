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

VMDINFO vi;                                                                         // Holds all the relevant information about the mode.

/**
 * @brief      Display line driver.
 *
 * @param[in]  scanLine  Current Scan Line
 *
 * @return     Address of line to render
 */
static uint8_t *VMDGetDisplayLine(uint16_t scanLine) {
    if (vi.displaySurface == NULL || !vi.enabled) return NULL;                      // No VRAM allocated, not enabled, blank.
    if (scanLine < vi._startDisplay || scanLine >= vi._startBlank) return NULL;     // Off top and bottom, blank
    return vi.displaySurface + ((scanLine-vi._startDisplay) / vi._scanLineDivider) * vi.bytesPerLine;
}

/**
 * @brief      Initialise the modes library
 */
void VMDInitialise(void) {
    static bool isInitialised = false;                                              // Only initialise once.
    if (isInitialised) return;
    isInitialised = true;

    DVIInitialise();                                                                // Initialise the DVI system.
    vi.displaySurface = vi.drawSurface = NULL;                                      // No draw or display surface, yet.
    VMDSetMode(MODE_640_480_MONO2);                                                 // Set mode.
    DVISetLineAccessorFunction(VMDGetDisplayLine);                                  // Set callback to access line memory.
    uint32_t delay = COMTimeMS()+500;                                               // Workaround. For some reason 320x240x256 mode
    while (COMTimeMS() < delay) {}                                                  // doesn't work if it is first, another mode has to be started.
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
    vi._videoRAM = memory;vi._videoRAMSize = size;
}