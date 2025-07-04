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

#include "dvi_module.h"

#ifndef RUNTIME
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"
#endif

//
//      If this is defined, it will render the test graphic on 240 lines, and display as a 240 line display.
//      (it changes the line callback and the plotter). Very quick and dirty but it works. 
//
#define XRENDER_240

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
    #ifdef RENDER_240
    scanLine = scanLine >> 1;
    #endif
    return framebuffer + scanLine * 640;
}

static uint16_t modeInformation = 1;

/**
 * @brief      Set up the display in the given mode & draw some stuff on it.
 *
 * @param[in]  mode  The mode (see below)
 */
static void SetScreenMode(uint16_t mode) {

    modeInformation = mode;

    DVISetMode(modeInformation);                                                    // Switches mode at next top of frame                                              
    memset(framebuffer,0,640*480);                                                  // Fast screen clear

    for (int x = 0;x < 640;x++) {                                                   // Draw some lines.
        for (int y = x >> 1;y < 300;y++) {
            plotPixel(x,y,x >> 1);
        }
        for (int y = 400;y < 439;y++) {
            uint8_t p = x >> 5;
            if (y == 401 || y == 438 || (x & 0x1F) == 0) p = 0xFF;
            plotPixel(x,y,p);
        }
    }
    for (int x = 0;x < 32;x++) {                                                    // Draw a diagonal, checks pixel alignment
        plotPixel(x+120,x,0xFF);
    }
}


/**
 * @brief      Cycle through the allowed screen modes.
 */
static void CycleScreenModes(void) {
    static uint16_t modeList[] = { 1,2,4,0x8001,0x4001,8,0 };                       // Permitted modes
    static uint8_t modeIndex = 0;
    while (1) {
        sleep_ms(1500);            
        if (modeList[++modeIndex] == 0) modeIndex = 0;                              // Cycle through the modes.
        LOG("Switching to mode %x",modeList[modeIndex]);
        SetScreenMode(modeList[modeIndex]);
        __wfi();
    }
}

/**
 * @brief      Simple Demo Program
 *
 * @return     Error Code
 */
int MAINPROGRAM() {

    DVIInitialise();                                                                // Initialise the DVI system.
    DVISetLineAccessorFunction(_DVIGetDisplayLine);                                 // Set callback to access line memory.

    //
    //  Options for the mode information
    //
    //  Bit 15
    //      When set, this makes the DMA function in byte mode, not word mode. This is
    //      160 pixel across mode (only for 256 colour mode)
    //  Bit 14
    //      When set, use manual rendering of the display buffer to 640 pixels.
    //      
    //  Bits 0..3
    //      These set the rendering of data
    //          1       256 colour RRRGGGBB
    //          2       16 colour RGGB
    //          4       4 level greyscale
    //          8       2 level greyscale
    //  
    SetScreenMode(0x4001);
    
    // 
    //  Comment to run the benchmark for whatever mode, uncomment to cycle through modes.
    // 
    //CycleScreenModes();                                                             

    //
    //  A pathetic benchmark. Measures how many times it can do the time comparison in 1 second. Gives 
    //  a very rough idea of the performance hit of various drivers.
    //
    //  With no driver running at all, the benchmark is 17476268 
    //  With the interrupt driven only driver, the benchmark is 16277777 (about 6% of core time)

    uint32_t count = 0;                                                             
    uint32_t next = time_us_32();                                                   
    while (1) {                                                                     
        if (time_us_32() > next) {
            LOG("Check count = %d",count);
            next = time_us_32() + 1024*1024;
            count = 0;
        } else {
            count++;
        }
        YIELD();                                                                    // This is for the runtime library.s
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

    #ifdef RENDER_240
    y = y >> 1;                                                                     // For 240 lines.
    #endif

    switch(modeInformation & 0x0F) {
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
