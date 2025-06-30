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

#include "common_manager.h"
#include "dvi_manager.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"

uint8_t framebuffer[640*480];

static void plotPixel(uint16_t x,uint16_t y,uint8_t colour);
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

uint8_t modeInformation = 1;

/**
 * @brief      Simple Demo Program
 *
 * @return     Error Code
 */
int main() {
    COMInitialise();
    //
    //  Options for the mode information
    //
    //  Bits 0..3
    //  
    //      1       256 colour RRRGGGBB
    //      2       16 colour RGGB
    //      4       4 level greyscale
    //      8       2 level greyscale
    //  
    DVIInitialise();                                                                // Initialise the DVI system.
    DVISetLineAccessorFunction(_DVIGetDisplayLine);                                 // Set callback to access line memory.

    DVISetupRenderer(modeInformation);                                              // Set the line renderer and width.

    for (int x = 0;x < 640;x++) {
        for (int y = x >> 2;y < 300;y++) {
            plotPixel(x,y,x >> 1);
        }
        for (int y = 400;y < 440;y++) {
            uint8_t p = x >> 5;
            if (y == 400 || y == 439 || (x & 0x1F) == 0) p = 0xFF;
            plotPixel(x,y,p);
        }
    }
    for (int x = 0;x < 32;x++) {
        plotPixel(x+120,x,0xFF);
    }

    while (1) {
            LOG("Ping !");
            sleep_ms(500);            
            __wfi();
    }
    return 0;
}

/**
 * @brief      Single Pixel Plotter.
 *
 * @param[in]  x       x coordinate
 * @param[in]  y       y coordinate
 * @param[in]  colour  pixel colour.
 */
static void plotPixel(uint16_t x,uint16_t y,uint8_t colour) {
    uint8_t *address,mask,shift;
    if (x >= 640 || y >= 480) return;
    switch(modeInformation) {
        case 1:
            framebuffer[x+y*640] = colour;
            break;
        case 2:
            address = framebuffer + (x >> 1) + y * 640;
            shift = ((x & 1) == 0) ? 0 : 4;
            mask = 0xF0 >> shift;colour = (colour & 0x0F) << shift;
            *address = ((*address) & mask) | colour;
            break;
        case 4:
            address = framebuffer + (x >> 2) + y * 640;
            shift = (x & 3) * 2;
            mask = (0x03 << shift) ^ 0xFF;
            colour = (colour & 3) << shift;
            *address = ((*address) & mask) | colour;
            break;
        case 8:
            address = framebuffer + (x >> 3) + y * 640;
            shift = (x & 7);
            *address &= ((0x01 << shift) ^ 0xFF);
            if ((colour & 1) != 0) {
                *address |= (0x01 << shift);
            }
            break;
    }
}

