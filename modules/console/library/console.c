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
    console->_cursorDrawn = false;
}

/**
 * @brief      Reset the current console
 */
void CONResetConsole(void) {
    console->x = 0;console->y = 0;
    console->xLeft = vi.xScreen-1; 
    console->yBottom = vi.yScreen-1;
    console->ink = 0x0F0;console->paper = 0x000;
    console->cursor = 0xFF0;
    console->xRight = vi.xScreen-1;console->yBottom = vi.yScreen-1;
}

#define SORT_PAIR(c1,c2)            if (c1 > c2) { int32_t t = c1;c1 = c2;c2 = t; }

/**
 * @brief      Set the console window. These coordinates are in units of 8
 *             pixels (e.g. x1 = 32 means pixel 256)
 *
 * @param[in]  x1    text coordinates of one corner
 * @param[in]  y1    
 * @param[in]  x2    text coordinates of other corner
 * @param[in]  y2    
 */
void CONSetWindow(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2) {
    console->xLeft = x1*8;
    console->yTop = y1*8;
    console->xRight = (x2 == 0) ? vi.xScreen-1 : x2*8-1;
    console->yBottom = (y2 == 0) ? vi.yScreen-1 : y2*8-1;
    SORT_PAIR(console->xLeft,console->xRight);
    SORT_PAIR(console->yTop,console->yBottom);
    CONClearWindow();
}

/**
 * @brief      Set the colours (all 12 bit RGB)
 *
 * @param[in]  ink     ink, the text colour
 * @param[in]  paper   paper, the background colour
 * @param[in]  cursor  the cursor colour
 */
void CONSetColours(uint16_t ink,uint16_t paper,uint16_t cursor) {
    console->ink = ink;console->paper = paper;console->cursor = cursor;
}
