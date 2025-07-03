// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Input Manager main program
//      Date :      2nd July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include <stdlib.h>
 
#include "dvi_module.h"
#include "input_module.h"

static uint8_t framebuffer[640*480];

/**
 * @brief      Display line accessor
 *
 * @param[in]  scanLine  Scan line
 *
 * @return     Address of line data.
 */
static uint8_t *_DVIGetDisplayLine(uint16_t scanLine) {
    return framebuffer + scanLine * 640;
}

/**
 * @brief      Simple test program
 *
 * @param[in]  argc  Count of arguments
 * @param      argv  Arguments array
 *
 * @return     Error code.
 */
int main(int argc,char *argv[]) {

    INPInitialise();                                                                // Initialise input module.

    DVIInitialise();                                                                // Set up display - this is for mouse testing, not 
    DVISetLineAccessorFunction(_DVIGetDisplayLine);                                 // required for this module as a dependency.
    DVISetMode(1);                                                                  // 640 x 480 x 256 colours

    while (1) {                                                                     // Run USB dumping USB reports as raw data
        framebuffer[random() % (640*480)] = random();
        USBUpdate();
        INPUpdate();
    }	
    return 0;
}
