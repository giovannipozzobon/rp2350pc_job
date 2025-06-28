// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   HSTX module test program
//      Date :      25th June 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "common.h"
#include "dvi_manager.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"

uint8_t framebuffer[640*480];

/**
 * @brief      This function gets the line data to display on the specified scan
 *             line.
 *
 * @param[in]  scanLine  The scan line for which data is requested
 *
 * @return     A pointer to a buffer containing that scanline, or NULL.
 *             Returning NULL means a blank line is rendered (in black)
 */
static uint8_t *_DVIGetDisplayLine(uint16_t scanLine) {
    return framebuffer + scanLine * 640;
}

uint8_t pixelsPerByte = 2;

/**
 * @brief      Simple Demo Program
 *
 * @return     Error Code
 */
int main() {
    COMInitialise();
    //
    //  Options for the pixels per byte.
    //
    //  1       256 colour RRRGGGBB
    //  2       16 colour RGGB
    //  4       4 level greyscale
    //  8       2 level greyscale
    //  
    DVIInitialise();                                                                // Initialise the DVI system.
    DVISetupRenderer(pixelsPerByte,640);                                            // Set the line renderer and width.
    DVISetLineAccessorFunction(_DVIGetDisplayLine);                                 // Set callback to access line memory.

    for (int x = 0;x < 640;x++) {
        for (int y = 0;y < 480;y++) {
            uint8_t pixel = 0;
            uint16_t yCol = (x/16+y/16) & 15;
            if (pixelsPerByte == 1) {
                if (yCol & 1) pixel |= 0xE0;
                if (yCol & 2) pixel |= 0x1C;
                if (yCol & 4) pixel |= 0x03;
            }
            if (pixelsPerByte == 2) {
                if (yCol & 1) pixel |= 0x88;
                if (yCol & 2) pixel |= 0x44;
                if (yCol & 4) pixel |= 0x22;
                if (yCol & 8) pixel |= 0x11;
            }
            if (pixelsPerByte == 4) {
                if (yCol & 1) pixel |= 0xAA;
                if (yCol & 2) pixel |= 0x55;
            }
            if (pixelsPerByte == 8) {
                if (yCol & 1) pixel |= 0xFF;
            }

            if (y >= 128 || (x & 1)) {
                framebuffer[x+y*640] = pixel;
            }
        }
    }

    while (1) {
            printf("Ping !\n");
            ASSERT(false);
            sleep_ms(500);            
            __wfi();
    }
    return 0;
}
