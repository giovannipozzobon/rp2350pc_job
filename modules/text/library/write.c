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
static void _TXTDrawCursor(TXTWINDOW *window);

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
        _TXTDrawCursor(window);
        window->isCursorDrawn = false;
    }
    GFXDraw(Colour,window->textColour,window->backColour);
    switch(ch) {
        case CTL_CLEAR:
                _TXTClearScreen(window);break;
    }
    window->xCursor++;
    _TXTDrawCursor(window);
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
            TXTWriteChar(window,x,y,' ',true);
        }
    }
}

/**
 * @brief      Draw cursor
 *
 * @param      window  window structure
 */
static void _TXTDrawCursor(TXTWINDOW *window) {
    int x = window->xCursor+1;
    int y = window->yCursor;
    if (x >= window->width) { x = 0; y++; }
    if (y < window->height) {
        x = window->xPos + x * window->xSize;
        y = window->yPos + y * window->ySize;
        GFXDraw(DrawMode,3,0);
        GFXDraw(RawColour,255,0);
        GFXDraw(Move,x,y);
        GFXDraw(FillRect,x+7,y+7);
        GFXDraw(DrawMode,0,0);
    }
}