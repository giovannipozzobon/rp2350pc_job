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
 
#include "common_module.h"
#include "usb_module.h"
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

    COMInitialise();                                                                // Common initialise
    DVIInitialise();                                                                // Set up display
    DVISetLineAccessorFunction(_DVIGetDisplayLine);                              
    DVISetMode(1);                                                                  // 640 x 480 x 256 colours

    USBInitialise(true);                                                            // Set up, and wait for the USB Key
    INPInitialise();

    while (1) {                                                                     // Run USB dumping USB reports as raw data
        framebuffer[random() % (640*480)] = random();
        USBUpdate();
    }	
    return 0;
}
