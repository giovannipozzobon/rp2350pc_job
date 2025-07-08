// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      modesetup.c
//      Purpose :   Video mode setup code.
//      Date :      8th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "modes_module.h"
#include "modes_module_local.h"


/**
 * @brief      Set up for 320 pixels, 256 colours.
 */
static void VMD_Mode_320_x_256(void) {
    vi._dviMode = 0x4001;
    vi.xScreen = 320;vi.bytesPerLine = 320;
    vi.pixelsPerByte = 1;
}

/**
 * @brief      Set up for 160 pixels, 256 colours.
 */
static void VMD_Mode_160_x_256(void) {
    vi._dviMode = 0x8001;
    vi.xScreen = 160;vi.bytesPerLine = 160;
    vi.pixelsPerByte = 1;
}

/**
 * @brief      Set up for 640 pixels, 256 colours.
 */
static void VMD_Mode_640_x_256(void) {
    vi._dviMode = 0x0001;
    vi.xScreen = 640;vi.bytesPerLine = 640;
    vi.pixelsPerByte = 1;
}

/**
 * @brief      Set up for 640 pixels, 16 colours.
 */
static void VMD_Mode_640_x_16(void) {
    vi._dviMode = 0x0002;
    vi.xScreen = 640;vi.bytesPerLine = 320;
    vi.pixelsPerByte = 2;
}

/**
 * @brief      Set up for 640 pixels, 4 greyscale
 */
static void VMD_Mode_640_x_Mono4(void) {
    vi._dviMode = 0x0004;
    vi.xScreen = 640;vi.bytesPerLine = 160;
    vi.pixelsPerByte = 4;
}

/**
 * @brief      Set up for 640 pixels, Monochrome
 */
static void VMD_Mode_640_x_Mono2(void) {
    vi._dviMode = 0x0008;
    vi.xScreen = 640;vi.bytesPerLine = 80;
    vi.pixelsPerByte = 8;
}

/**
 * @brief      Set up the mode information structure according to the mode.
 *
 * @param[in]  mode  Mode to use.
 */
void VMDModeSetupInformation(uint32_t mode) {

    vi.mode = mode;
    vi.scanLineDivider = 1;                                                         // Default values incase fallback.
    vi.yScreen = 480;
    vi.enabled = false;                                                             // Disabled by default
    //
    //      Use the mode to set up information about the rendering line.
    //
    switch(mode) {
        case MODE_160_240_256:
        case MODE_160_200_256:
        case MODE_160_176_256:
            VMD_Mode_160_x_256();break;            
        case MODE_320_240_256:
        case MODE_320_200_256:
        case MODE_320_176_256:
            VMD_Mode_320_x_256();break;            
        case MODE_640_480_256:
        case MODE_640_240_256:
            VMD_Mode_640_x_256();break;            
        case MODE_640_480_16:
            VMD_Mode_640_x_16();break;            
        case MODE_640_480_MONO4:
            VMD_Mode_640_x_Mono4();break;            
        case MODE_640_480_MONO2:
            VMD_Mode_640_x_Mono2();break;            
        default:
            LOG("Mode unrecognised");
            return;
    }
    vi.yScreen = ((mode >> 9) & 0x7F) * 8;                                              // Work out vertical lines, physical vertical extent of mode.
    vi.scanLineDivider = 480 / vi.yScreen;                                              // Work out the division to get that many vertical lines.

    int displayLines = vi.scanLineDivider * vi.yScreen;                                 // Number of displayed lines out of 480
    vi._startDisplay = (480-displayLines)/2;                                            // Work out displayable top and bottom.
    vi._startBlank = vi._startDisplay + displayLines;

    int bufferSize = vi.bytesPerLine * vi.yScreen;                                      // Size of one buffer
    vi.bufferCount = vi._videoRAMSize / bufferSize;                                     // Count.
    if (vi.bufferCount > VMD_MAX_BUFFERS) vi.bufferCount = VMD_MAX_BUFFERS;             // Maximum buffer count.
    for (int i = 0;i < vi.bufferCount;i++) {                                            // Work out buffer addresses.
        vi.buffer[i] = vi._videoRAM + bufferSize * i;
    }

    vi.drawSurface = vi.displaySurface = vi.buffer[0];                                  // Initially one simple buffer.
    vi.enabled = (vi.bufferCount != 0);                                                 // Enable if buffer count > 0
}