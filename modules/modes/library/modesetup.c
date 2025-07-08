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
}

/**
 * @brief      Set up for 160 pixels, 256 colours.
 */
static void VMD_Mode_160_x_256(void) {
    vi._dviMode = 0x8001;
    vi.xScreen = 160;vi.bytesPerLine = 160;
}

/**
 * @brief      Set up the mode information structure according to the mode.
 *
 * @param[in]  mode  Mode to use.
 */
void VMDModeSetupInformation(uint32_t mode) {

    vi.mode = mode;

    switch(mode) {
        case MODE_160_240_256:
            VMD_Mode_160_x_256();break;            
        case MODE_320_240_256:
            VMD_Mode_320_x_256();break;            
        default:
            LOG("Mode unrecognised");
    }


    vi.scanLineDivider = 1;
    if (vi._dviMode == 0x4001) {
        vi.scanLineDivider = 2;  
    } 
    if (vi._dviMode == 0x8001) {
        vi.xScreen = vi.bytesPerLine = 160;
    }
    vi.yScreen = 480 / vi.scanLineDivider;


}