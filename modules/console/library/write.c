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
static bool CONOutputCharacter(uint16_t ch);
static void CONScrollUp(uint16_t scroll);
static bool CONBackspace(void);
static void CONDrawCursor(bool newState);

/**
 * @brief      Write character/command to console
 *
 * @param[in]  ch    Character or command
 */
void CONWrite(uint16_t ch) {

    if (console->_cursorDrawn) {
        CONDrawCursor(false);
    }
    switch(ch) {

        case CTL_BACKSPACE:                                                         // Backspace and erase
            CONBackspace();
            break;

        case CTL_CRLF:                                                              // Carriage return
            CONOpenContext();                                                       // Fill the bottom line with the background
            console->x = 0;                                                         // Back to LHS
            console->y += console->_ySize;
            if (console->yTop + console->y >= console->yBottom) {                   // Scrolling required.
                console->y -= console->_ySize;
                CONScrollUp(console->_ySize);
            }
            CONCloseContext();
            break;

        default:
            if (ch >= ' ' && ch <= 0x7F) {                                          // Display characters.
                if (!CONOutputCharacter(ch)) {
                    CONWrite(CTL_CRLF);
                    CONOutputCharacter(ch);
                }
            }
            break;
    }
    CONDrawCursor(true);
}

/**
 * @brief      Try to backspace one character, deleting it.
 *
 * @return     true if worked, false if top left and can't.
 */
static bool CONBackspace(void) {
    if (console->x == 0 && console->y == 0) return false;                               // Cannot backspace.

    CONOpenContext();

    if (console->x == 0) {                                                              // At LHS
        console->x = (console->xRight-console->xLeft+1);                                // Right hand position
        console->y -= console->_ySize;                                                  // Up one.
    }

    console->x -= console->_xSize;                                                      // Back one character
    GFXDraw(Colour,console->paper,0);                                                   // Erase at cursor
    GFXDraw(Move,console->xLeft+console->x,console->yTop+console->y);
    GFXDraw(FillRect,console->xLeft+console->x + console->_xSize - 1,
                    console->yTop+console->y + console->_ySize-1);
    CONCloseContext();
    return true;
}

/**
 * @brief      Draw or Erase the cursor
 *
 * @param[in]  newState  true to draw, false to erase, affects the drawing colour.
 */
static void CONDrawCursor(bool newState) {
    console->_cursorDrawn = newState;
    CONOpenContext();
    GFXDraw(Colour,newState ? console->cursor:console->paper,0);   
    GFXDraw(Move,console->xLeft+console->x,console->yTop+console->y);
    GFXDraw(Line,console->xLeft+console->x,console->yTop+console->y+console->_ySize-1);
    CONCloseContext();    
}
/**
 * @brief      Try to output character on current line.
 *
 * @param[in]  ch    Character to output
 *
 * @return     will not fit, require a CR/LF.
 */
static bool CONOutputCharacter(uint16_t ch) {
    CONOpenContext();
    bool isOk = console->x + console->xLeft + console->_xSize-1 <= console->xRight;  // Does it fit ?
    if (isOk) {
        GFXDraw(Move,console->xLeft + console->x,console->yTop + console->y);       // Output character
        GFXDraw(Character,ch,0);
        console->x += console->_xSize;                                               // Advance position.
    }
    CONCloseContext();
    return isOk;
}
/**
 * @brief      Open and initialise the drawing context for the console
 */
static void CONOpenContext(void) {
    GFXOpenContext();
    GFXDraw(Colour,console->ink,console->paper);
    GFXDraw(Move,console->xLeft,console->yTop);
    GFXDraw(SetClip,console->xRight,console->yBottom);
    uint32_t ext = GFXDraw(CharExtent,' ',0);                            
    console->_xSize = ext & 0xFFFF;console->_ySize = ext >> 16;
}

/**
 * @brief      Close the drawing context for the console and tidy up.
 */
static void CONCloseContext(void) {
    GFXCloseContext();
}

/**
 * @brief      Scroll the console window up by the given number of lines and
 *             blank the bottom row.
 *
 * @param[in]  scroll  lines to scroll
 */
static void CONScrollUp(uint16_t scroll) {
    for (int y = console->yTop;y < console->yBottom-scroll;y++) {
        uint8_t *line = vi.drawSurface + y * vi.bytesPerLine + console->xLeft/vi.pixelsPerByte;
        memcpy(line,line+scroll*vi.bytesPerLine,(console->xRight-console->xLeft)/vi.pixelsPerByte+1);
    }

    GFXDraw(Move,console->xLeft,console->yBottom-scroll);
    GFXDraw(Colour,console->paper,console->paper);
    GFXDraw(FillRect,console->xRight,console->yBottom);
}

/**
 * @brief      Clear the console window.
 */
void CONClearWindow(void) {
    CONOpenContext();
    GFXDraw(ClearWindow,0,0);
    console->_cursorDrawn = false;
    console->x = console->y = 0;
    CONCloseContext();
}