// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      write.c
//      Purpose :   Console writing
//      Date :      15th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "console_module.h"
#include "console_module_local.h"

static void CONOpenContext(void);
static void CONCloseContext(void);
/**
 * @brief      Write character/command to console
 *
 * @param[in]  ch    Character or command
 */
void CONWrite(uint16_t ch) {
    LOG("Write %d",ch);

    if (console->clearPending) {                                                    // Clear is delayed until we actually do something, not at 
        console->clearPending = false;                                              // initialisation or rectangle setting.
        CONWrite(CTL_CLEAR);  
    }


    switch(ch) {
        case CTL_CLEAR:                                                             // Clear screen, home cursor.
            CONOpenContext();
            CONWrite(CTL_HOME);     
            GFXDraw(ClearWindow,0,0);
            CONCloseContext();
            break;

        case CTL_HOME:                                                              // Home cursor.
            console->x = console->y = 0;
            break;

        default:
            break;
    }
}

/**
 * @brief      Open and initialise the drawing context for the console
 */
static void CONOpenContext(void) {
    GFXOpenContext();
    GFXDraw(Colour,0xFF0,0x00F);
    GFXDraw(Move,console->xLeft,console->yTop);
    GFXDraw(SetClip,console->xRight,console->yBottom);
}

/**
 * @brief      Close the drawing context for the console and tidy up.
 */
static void CONCloseContext(void) {
    GFXCloseContext();
}