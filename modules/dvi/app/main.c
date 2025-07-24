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
#include "usb_module.h"

//
//      If this is defined, it will render the test graphic on 240 lines, and display as a 240 line display.
//      (it changes the line callback and the plotter). Very quick and dirty but it works. 
//
#define XRENDER_240                                                                 // Ugly hack - removing X means it will do 240 pixels.

uint8_t framebuffer[640*480];                                                      

static void plotPixel(uint16_t x,uint16_t y,uint8_t colour);
/**
 * @brief      This function gets the line data to display on the specified scan
 *             line, which runs from 0-479.
 *             
 *             This is the only function required to get DVI up and running.
 *
 * @param[in]  scanLine  The scan line for which data is requested
 *
 * @return     A pointer to a buffer containing that scanline, or NULL.
 *             Returning NULL means a blank line is rendered (in black)
 */
static uint8_t *KEEPINRAM(_DVIGetDisplayLine)(uint16_t scanLine) {
    return framebuffer + scanLine * 640;                                            // Address in memory.
}

static uint16_t modeInformation = 1;                                                // A lazy global, the current mode.

/**
 * @brief      Set up the display in the given mode & draw some stuff on it.
 *
 * @param[in]  mode  The mode (see include file)
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
    static uint8_t modeIndex = 0;
    while (COMAppRunning()) {                                                       // Until exit (runtime)
        uint32_t next = COMTimeMS()+1500;                                           // Wait 1500ms
        while (COMTimeMS() < next) { YIELD(); }         
        modeIndex = (modeIndex + 1) % DVIR_COUNT;
        LOG("Switching to mode %x",modeIndex);
        SetScreenMode(modeIndex);                                                       // And change it.
    }
}

/**
 * @brief      Simple Demo Program
 *
 * @return     Error Code
 */
int MAINPROGRAM() {
    USBInitialise();
    DVIInitialise();                                                                // Initialise the DVI system.
    DVISetLineAccessorFunction(_DVIGetDisplayLine);                                 // Set callback to access line memory.

    SetScreenMode(2);
    
    // 
    //  Comment to run the benchmark for whatever mode, uncomment to cycle through modes.
    // 
    // CycleScreenModes();return(0);

    //
    //  A pathetic benchmark. Measures how many times it can do the time comparison in 1 second. Gives 
    //  a very rough idea of the performance hit of various drivers.
    //
    //  With no driver running at all, the benchmark is 17476268 
    //  With the interrupt driven only driver, the benchmark is 16277777 (about 6% of core time)

    uint32_t count = 0;                                                             
    uint32_t next = COMTimeMS();  
    while (COMAppRunning()) {                                                   // While not complete                                                 
        if (COMTimeMS() > next) {
            next = COMTimeMS()+1000;
            LOG("us per scan %d",DVIGetScanLineTime());
        }
        YIELD();                                                                // This is for the runtime library.s
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
    framebuffer[x+y*640] = colour;
}
