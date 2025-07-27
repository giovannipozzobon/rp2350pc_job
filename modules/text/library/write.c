// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      write.c
//      Purpose :   Writing to window.
//      Date :      27th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "text_module.h"
#include "text_module_local.h"

static void _TXTClearScreen(TXTWINDOW *window);
static void _TXTDrawCursor(TXTWINDOW *window,uint16_t colour);

/**
 * @brief      Write character to arbitrary window
 *
 * @param      window  Address of structure defining text window.
 * @param[in]  ch      Character to write.
 */
void TXTWriteWindow(TXTWINDOW *window,uint16_t ch) {
    TXTCheckStatus(window);
    GFXOpenContext();
    if (window->isCursorDrawn) {
        _TXTDrawCursor(window,window->backColour);
        window->isCursorDrawn = false;
    }
    GFXDraw(Colour,window->textColour,window->backColour);
    switch(ch) {
        case CTL_CLEAR:
                _TXTClearScreen(window);break;
    }
    _TXTDrawCursor(window,window->cursorColour);
    window->isCursorDrawn = true;
    GFXCloseContext();
}

/**
 * @brief      Read a character from the state manager
 *
 * @param      window  Window Structure
 * @param[in]  x       x coordinate
 * @param[in]  y       y coordinate
 *
 * @return     0 if not available, or character at that coordinate
 */
uint16_t TXTReadChar(TXTWINDOW *window,uint16_t x,uint16_t y) {
    if (window->stateManager == NULL) return 0;                                     // No state manager.
    return (*(TXTSTATEMANAGER)window->stateManager)(window,x,y,0);                  // Call state manager with character 0.
}

/**
 * @brief      Write a character, notifying the state manager optionally
 *
 * @param      window       Window structure
 * @param[in]  x            x coordinate
 * @param[in]  y            y coordinate
 * @param[in]  ch           character
 * @param[in]  updateState  true to update
 */
void TXTWriteChar(TXTWINDOW *window,uint16_t x,uint16_t y,uint16_t ch,bool updateState) {
    GFXDraw(Move,window->xPos+x*window->xSize,window->yPos+y*window->ySize);        // Draw the character
    GFXDraw(Character,ch,0);   
    if (window->stateManager != NULL && updateState) {                              // If updating and state manager in use, update it.
        (*(TXTSTATEMANAGER)window->stateManager)(window,x,y,ch);
    }
}

/**
 * @brief      Clear the screen
 *
 * @param      window  Window structure
 */
static void _TXTClearScreen(TXTWINDOW *window) {
    for (int x = 0;x < window->width;x++) {
        for (int y = 0;y < window->height;y++) {
            TXTWriteChar(window,x,y,x+y+32,true);
        }
    }
}

/**
 * @brief      Draw cursor
 *
 * @param      window  window structure
 * @param[in]  colour  colour to draw.
 */
static void _TXTDrawCursor(TXTWINDOW *window,uint16_t colour) {
}