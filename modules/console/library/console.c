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
}

/**
 * @brief      Reset the current console
 */
void CONResetConsole(void) {
    console->x = console->y = 0;
    console->rc = NULL;
    console->clearPending = true;
}

/**
 * @brief      Set the clipping window.
 *
 * @param      rc    The rectangle
 */
void CONSetWindow(GFXCLIPRECT *rc) {
    console->rc = rc;
}