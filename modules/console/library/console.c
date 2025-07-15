// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      console.c
//      Purpose :   Console configuration
//      Date :      14th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "console_module.h"
#include "console_module_local.h"

CONCONFIG *console;
CONCONFIG defaultConsole;

/**
 * @brief      Initialise the console system.
 */
void CONInitialise(void) {
    GFXInitialise();                                                                // Initialise graphics subsystem and chain.
    console = &defaultConsole;
    CONResetConsole();
    console->clearPending = true;
}

/**
 * @brief      Reset the current console
 */
void CONResetConsole(void) {
    console->x = console->y = 0;
    console->xLeft = console->yTop = 0;
    console->xRight = vi.xScreen-1;console->yBottom = vi.yScreen-1;
}

#define SORT_PAIR(c1,c2)            if (c1 > c2) { int32_t t = c1;c1 = c2;c2 = t; }

/**
 * @brief      Set the console window
 *
 * @param[in]  x1    coordinates of one corner
 * @param[in]  y1   
 * @param[in]  x2    coordinates of other corner
 * @param[in]  y2    
 */
void CONSetWindow(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2) {
    console->xLeft = x1;console->xRight = x2;
    console->yTop = y1;console->yBottom = y2;
    SORT_PAIR(console->xLeft,console->xRight);
    SORT_PAIR(console->yTop,console->yBottom);
}