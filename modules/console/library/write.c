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
static uint32_t CONGetCharSize(uint16_t ch);
static void CONScrollUp(uint16_t scroll);
static bool CONBackspace(void);
static void CONDrawCursor(bool newState);
static void CONMoveCursor(uint16_t ch);

/**
 * @brief      Write character/command to console
 *
 * @param[in]  ch    Character or command
 */
void CONWrite(uint16_t ch) {

    if (console->clearPending) {                                                    // Clear is delayed until we actually do something, not at 
        console->clearPending = false;                                              // initialisation or rectangle setting.
        CONWrite(CTL_CLEAR);  
    }
    if (console->cursorDrawn) {
        CONDrawCursor(false);
    }
    switch(ch) {

        case CTL_BACKSPACE:                                                         // Backspace and erase
            CONBackspace();
            break;

        case CTL_LEFT:                                                              // Move cursor.
        case CTL_RIGHT:
        case CTL_UP:
        case CTL_DOWN:
            CONMoveCursor(ch);
            break;

        case CTL_CRLF:                                                              // Carriage return
            console->x = 0;                                                         // Back to LHS
            uint32_t ext = CONGetCharSize('W') >> 16;                               // Move down
            console->y += ext;
            if (console->yTop + console->y >= console->yBottom) {                   // Scrolling required.
                console->y -= ext;
                CONScrollUp(ext);
            }
            break;

        case CTL_CLEAR:                                                             // Clear screen, home cursor.
            CONOpenContext();
            CONWrite(CTL_HOME);     
            GFXDraw(ClearWindow,0,0);
            console->cursorDrawn = false;
            CONCloseContext();
            break;

        case CTL_HOME:                                                              // Home cursor.
            console->x = console->y = 0;
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
 * @brief      Get character size.
 *
 * @param[in]  ch    Character
 *
 * @return     Size in CharExtent format (hi/lo words)
 */
static uint32_t CONGetCharSize(uint16_t ch) {
    CONOpenContext();                   
    uint32_t ext = GFXDraw(CharExtent,ch,0);                            
    CONCloseContext();
    return ext;
}

/**
 * @brief      Handle cursor movement.
 *
 * @param[in]  ch    CTL_xx for movement.
 */
static void CONMoveCursor(uint16_t ch) {
    CONOpenContext();
    uint32_t ext = CONGetCharSize(' ');                                                 // Character size
    uint16_t xSize = ext & 0xFFFF,ySize = ext >> 16;
    switch(ch) {
        case CTL_LEFT:
            if (console->x > 0) {
                console->x = console->x - xSize;
            }
            break;
        case CTL_RIGHT:
            if (console->x + xSize + console->xLeft <= console->xRight) {
                console->x += xSize;
            }
            break;
        case CTL_UP:
            if (console->y > 0) {
                console->y = console->y - ySize;
            }
            break;
        case CTL_DOWN:
            if (console->y + ySize + console->yTop <= console->yBottom) {
                console->y += ySize;
            }
            break;
    }
    CONCloseContext();
}
/**
 * @brief      Try to backspace one character, non destructive
 *
 * @return     true if worked, false if top left and can't.
 */
static bool CONBackspace(void) {
    if (console->x == 0 && console->y == 0) return false;                               // Cannot backspace.
    CONOpenContext();
    uint32_t ext = CONGetCharSize(' ');                                                 // Character size
    if (console->x == 0) {                                                              // At LHS
        console->x = (console->xRight-console->xLeft+1);                                // Right hand position
        console->y -= (ext >> 16);                                                      // Up one.
    }
    console->x -= (ext & 0xFFFF);                                                       // Back
    GFXDraw(Colour,console->paper,0);                                                   // Erase at cursor
    GFXDraw(Move,console->xLeft+console->x,console->yTop+console->y);
    GFXDraw(FillRect,console->xLeft+console->x + (ext & 0xFFFF) - 1,
                                console->yTop+console->y + (ext >> 16)-1);
    CONCloseContext();
    return true;
}

/**
 * @brief      Draw or Erase the cursor
 *
 * @param[in]  newState  true to draw, false to erase, affects the drawing colour.
 */
static void CONDrawCursor(bool newState) {
    console->cursorDrawn = newState;
    CONOpenContext();
    uint32_t ext = GFXDraw(CharExtent,' ',0);                         
    GFXDraw(Colour,newState ? console->cursor:console->paper,0);   
    GFXDraw(Move,console->xLeft+console->x,console->yTop+console->y);
    GFXDraw(Line,console->xLeft+console->x,console->yTop+console->y+(ext>>16)-1);
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
    uint32_t ext = GFXDraw(CharExtent,ch,0);
    bool isOk = console->x + console->xLeft + (ext & 0xFFFF) - 1 <= console->xRight;// Does it fit ?
    if (isOk) {
        GFXDraw(Move,console->xLeft + console->x,console->yTop + console->y);       // Output character
        GFXDraw(Character,ch,0);
        console->x += (ext & 0xFFFF);                                               // Advance position.
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

    GFXOpenContext();                                                               // Fill the bottom line with the background
    GFXDraw(Move,console->xLeft,console->yBottom-scroll);
    GFXDraw(Colour,console->paper,console->paper);
    GFXDraw(FillRect,console->xRight,console->yBottom);
    GFXCloseContext();
}
