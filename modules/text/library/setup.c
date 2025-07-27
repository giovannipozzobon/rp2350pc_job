// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      setup.c
//      Purpose :   Initialisation code
//      Date :      27th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "text_module.h"
#include "text_module_local.h"

TXTWINDOW defText;

/**
 * @brief      Initialise the text window module and set the default to uninitialised.
 */
void TXTInitialise(void) {
    GFXInitialise();
    defText.isInitialised = false;
}

/**
 * @brief      Write character to default window
 *
 * @param[in]  ch    Character to write.
 */
void TXTWrite(uint16_t ch) {
    TXTWriteWindow(&defText,ch);
}

/**
 * @brief      Initialise window to default if not specified.
 *
 * @param      txt   text window structure.
 */
void TXTCheckStatus(TXTWINDOW *txt) {
    if (txt->isInitialised || vi.mode != txt->currentMode) {
        txt->isInitialised = true;
        txt->currentMode = vi.mode;
        txt->xPos = txt->yPos = 0;
        txt->width = vi.xScreen / 8;txt->height = vi.yScreen / 8;
        txt->xCursor = txt->yCursor = 0;
        txt->isCursorDrawn = false;
    }
}