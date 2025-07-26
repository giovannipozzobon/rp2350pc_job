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

    if (console->_cursorDrawn) {                                                    // If cursor drawn, erase it.
        CONDrawCursor(false);
    }
    switch(ch) {

        case CTL_BACKSPACE:                                                         // Backspace and erase character there.
            CONBackspace();
            break;

        case CTL_CRLF:                                                              // Carriage return
            CONOpenContext();                                                       // Work on the console context
            console->x = 0;                                                         // Back to LHS
            console->y += console->_ySize;
            if (console->yTop + console->y >= console->yBottom) {                   // Scrolling required.
                console->y -= console->_ySize;                                      // So undo the move down
                CONScrollUp(console->_ySize);                                       // And scroll everything
            }
            CONCloseContext();                                                      // End work.
            break;

        default:
            if (ch >= ' ' && ch <= 0x7F) {                                          // Display characters.
                if (!CONOutputCharacter(ch)) {                                      // If output failed, which means off RHS.
                    CONWrite(CTL_CRLF);                                             // Do a CRLF and try again.
                    CONOutputCharacter(ch);
                }
            }
            break;
    }
    CONDrawCursor(true);                                                            // Put the cursor back.
}

/**
 * @brief      Try to backspace one character, deleting it.
 *
 * @return     true if worked, false if top left and can't.
 */
static bool CONBackspace(void) {
    if (console->x == 0 && console->y == 0) return false;                           // Cannot backspace, at top left

    CONOpenContext();                                                               // Start drawing.

    if (console->x == 0) {                                                          // At Left side of screen.
        console->x = (console->xRight-console->xLeft+1);                            // Right hand position
        console->y -= console->_ySize;                                              // Up one.
    }

    console->x -= console->_xSize;                                                  // Back one character
    GFXDraw(Colour,console->paper,0);                                               // Erase at cursor
    GFXDraw(Move,console->xLeft+console->x,console->yTop+console->y);
    GFXDraw(FillRect,console->xLeft+console->x + console->_xSize - 1,
                    console->yTop+console->y + console->_ySize-1);
    CONCloseContext();                                                              // End drawing.
    return true;
}

/**
 * @brief      Draw or Erase the cursor
 *
 * @param[in]  newState  true to draw, false to erase, affects the drawing colour.
 */
static void CONDrawCursor(bool newState) {
    console->_cursorDrawn = newState;                                               // Save state
    CONOpenContext();                                                               // Start drawing
    GFXDraw(Colour,newState ? console->cursor:console->paper,0);                    // Colour depends on erase/draw
    GFXDraw(Move,console->xLeft+console->x,console->yTop+console->y);               // Draw vertical line
    GFXDraw(Line,console->xLeft+console->x,console->yTop+console->y+console->_ySize-1);
    CONCloseContext();                                                              // End drawing.
}
/**
 * @brief      Try to output character on current line.
 *
 * @param[in]  ch    Character to output
 *
 * @return     will not fit, require a CR/LF.
 */
static bool CONOutputCharacter(uint16_t ch) {
    CONOpenContext();                                                               // Start drawing
    bool isOk = console->x + console->xLeft + console->_xSize-1 <= console->xRight; // Does it fit ?
    if (isOk) {
        GFXDraw(Move,console->xLeft + console->x,console->yTop + console->y);       // Output character
        GFXDraw(Character,ch,0);
        console->x += console->_xSize;                                              // Advance position.
    }
    CONCloseContext();                                                              // End drawing
    return isOk;
}
/**
 * @brief      Open and initialise the drawing context for the console
 */
static void CONOpenContext(void) {
    GFXOpenContext();                                                               // Open a new graphic context
    GFXDraw(Colour,console->ink,console->paper);                                    // Set up the fgr/bgr colours
    GFXDraw(Move,console->xLeft,console->yTop);                                     // Set the clipping windoow
    GFXDraw(SetClip,console->xRight,console->yBottom);
    uint32_t ext = GFXDraw(CharExtent,' ',0);                                       // Assuming monospaced font, so get the size of 1 character
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
 *             blank the bottom row. Context already open.
 *
 * @param[in]  scroll  lines to scroll
 */
static void CONScrollUp(uint16_t scroll) {
    for (int y = console->yTop;y < console->yBottom-scroll;y++) {                   // For each line except the scrolling are
        uint8_t *line = vi.drawSurface + y * vi.bytesPerLine +                      // Start of this line
                                                    console->xLeft/vi.pixelsPerByte;
        memcpy(line,                                                                // Copy the line up in vRam.
               line+scroll*vi.bytesPerLine,
               (console->xRight-console->xLeft)/vi.pixelsPerByte+1);
    }

    GFXDraw(Move,console->xLeft,console->yBottom-scroll);                           // Blank the bottom line.
    GFXDraw(Colour,console->paper,console->paper);
    GFXDraw(FillRect,console->xRight,console->yBottom);
}

/**
 * @brief      Clear the console window.
 */
void CONClearWindow(void) {
    CONOpenContext();                                                               // Open a console context
    GFXDraw(ClearWindow,0,0);                                                       // Clear the window
    console->_cursorDrawn = false;                                                  // Reset cursor and position
    console->x = console->y = 0;
    CONCloseContext();                                                              // End drawing.
}