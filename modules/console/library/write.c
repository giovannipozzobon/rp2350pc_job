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
    switch(ch) {
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
                    CONOutputCharacter(' ');                                        // Make space for cursor
                    CONBackspace();                                                 // And undo it.
                }
            }
            break;
    }
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
 * @brief      Try to backspace one character, non destructive
 *
 * @return     true if worked, false if top left and can't.
 */
static bool CONBackspace(void) {
    if (console->x == 0 && console->y == 0) return false;                               // Cannot backspace.
    uint32_t ext = CONGetCharSize(' ');                                                 // Character size
    if (console->x == 0) {                                                              // At LHS
        console->x = (console->xRight-console->xLeft+1);                                // Right hand position
        console->y -= (ext >> 16);                                                      // Up one.
    }
    console->x -= (ext & 0xFFFF);                                                       // Back
    return true;
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
    LOG("Scroll %d",scroll);
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
