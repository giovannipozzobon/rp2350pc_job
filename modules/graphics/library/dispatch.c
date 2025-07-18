// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      setup.c
//      Purpose :   Graphics module setup
//      Date :      9th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

static void GFXDrawDesktop(void);

static void *pointerData = NULL;

/**
 * @brief      Execute a graphics command. If different signature/same
 *             identifier works this may be just GFXDraw(*,#), but for now this
 *             always works.
 *
 * @param[in]  cmd   command ID
 * @param      p     Pointer being passed in.
 * @param[in]  y     y Coordinate
 *
 * @return     Return value depends.
 */
uint32_t GFXDrawP(enum GFXCommand cmd,void *p,uint32_t y) {    
    pointerData = p;
    return GFXDraw(cmd,0,y);
}

/**
 * @brief      Execute a graphics command
 *
 * @param[in]  cmd   command ID
 * @param[in]  x     x Coordinate
 * @param[in]  y     y Coordinate
 * 
 * @return     Return value depends.
 */
uint32_t GFXDraw(enum GFXCommand cmd,uint32_t x,uint32_t y) {    


    uint32_t retVal = 0;
    switch(cmd) {
        //
        //      Control functions
        //
        case Mode:                                                                  // Set screen mode
            VMDSetMode(x);
            GFXCheckModeChange();
            break;

        case RawColour:                                                             // Set Colour (raw, the physical byte value)
            draw->foreground = x & 0xFFFF;draw->background = y & 0xFFFF;
            draw->isTransparent = false;
            if ((y & 0xFFFF) == 0xFFFF) { draw->background = 0;draw->isTransparent = true; }
            break;

        case Colour:                                                                // Set Colour RGB format
            draw->foreground = GFXToRawColour(x & 0xFFF,vi.pixelsPerByte);
            draw->background = GFXToRawColour(y & 0xFFF,vi.pixelsPerByte);
            draw->isTransparent = false;
            if ((y & 0xFFFF) == 0xFFFF) { draw->background = 0;draw->isTransparent = true; }
            break;

        case Scaling:                                                               // Set font scaling.
            draw->xFontScale = x;draw->yFontScale = y;
            break;

        case SetClip:                                                               // Set current clip
            GFXPreProcess(&x,&y);
            draw->clip.xLeft = x;draw->clip.yTop = y;                               // Set the clipping rect like a rectangle draw
            draw->clip.xRight = draw->xPrev[0];draw->clip.yBottom = draw->yPrev[0];
            SORT_PAIR(draw->clip.xLeft,draw->clip.xRight);                          // Sort clip rect, as we might have reversed x/y axis.
            SORT_PAIR(draw->clip.yTop,draw->clip.yBottom);
            break;

        case NoClip:                                                                // Remove clipping
            GFXResetClipping();
            break;

        case SetMapper:                                                             // Set the mapping from logical to physical
            draw->mapper = (GFXMAPPER)pointerData;                                  // (by default logical == physical)
            break;
        //
        //      Drawing functions
        //
        case Move:                                                                  // Move to location
            GFXPreProcess(&x,&y);
            GFXRawMove(x,y);
            break;

        case Plot:                                                                  // Move to location and plot pixel.
            GFXPreProcess(&x,&y);
            GFXRawMove(x,y);
            GFXRawPlot(true);
            break;

        case Line:                                                                  // Draw a solid line.
            GFXPreProcess(&x,&y);
            GFXDrawLine(draw->xPrev[0],draw->yPrev[0],x,y,true);
            break;

        case Rect:                                                                  // Rectangles
        case FillRect:
            GFXPreProcess(&x,&y);
            GFXDrawRectangle(draw->xPrev[0],draw->yPrev[0],x,y,cmd == FillRect);
            GFXRawMove(x,y);
            break;

        case Ellipse:                                                               // Ellipses
        case FillEllipse:
            GFXPreProcess(&x,&y);
            GFXDrawEllipse(draw->xPrev[0],draw->yPrev[0],x,y,cmd == FillEllipse);
            GFXRawMove(x,y);
            break;

       case Triangle:                                                               // Outline triangle
            GFXPreProcess(&x,&y);
            GFXDrawOutlineTriangle(draw->xPrev[1],draw->yPrev[1],draw->xPrev[0],draw->yPrev[0],x,y);
            GFXRawMove(x,y);
            break;

        case FillTriangle:                                                          // Filled triangle
            GFXPreProcess(&x,&y);
            GFXDrawFilledTriangle(draw->xPrev[1],draw->yPrev[1],draw->xPrev[0],draw->yPrev[0],x,y);
            GFXRawMove(x,y);
            break;

        case Character:                                                             // Draw a character
            uint32_t xOrg = draw->x,yOrg = draw->y;
            x = GFXDrawCharacter(draw->x,draw->y,x);
            GFXRawMove(xOrg+(x & 0xFF),yOrg);            
            break;

        case CharExtent:                                                            // Get character extent.
            retVal = GFXGetCharacterExtent(x);                                      // Height in upper 16 bits, Width in lower1 16 bits
            break;

        case Clear:                                                                 // Clear whole screen to background
            memset(vi.drawSurface,draw->background,vi.bufferSize);
            break;

        case ClearWindow:                                                           // Clear the window to background
            uint32_t oldFgr = draw->foreground;draw->foreground = draw->background;
            for (int y = draw->clip.yTop;y <= draw->clip.yBottom;y++) {
                GFXDraw(Move,draw->clip.xLeft,y);
                GFXDraw(Line,draw->clip.xRight,y);
            }
            draw->foreground = oldFgr;
            break;

        case Desktop:                                                               // Clear whole screen to desktop
            GFXDrawDesktop();
            break;          
    }
    return retVal;
}

/**
 * @brief      Preprocess coordinates. Does logical->physical conversion and
 *             tracks previous coordinates.
 *
 * @param      x     address of x
 * @param      y     address of y
 */
void GFXPreProcess(int32_t *x,int32_t *y) {
    // Logical to Physical mapping.
    if (draw->mapper != NULL) {
        (*draw->mapper)(x,y);
    }
    // Push coordinates onto previous lists.
    draw->xPrev[2] = draw->xPrev[1];draw->yPrev[2] = draw->yPrev[1];                    
    draw->xPrev[1] = draw->xPrev[0];draw->yPrev[1] = draw->yPrev[0];
    draw->xPrev[0] = draw->x;       draw->yPrev[0] = draw->y;
}

static void GFXDrawDesktop(void) {
    switch(vi.pixelsPerByte) {
        case 1:                                                                     // 256 colour
        case 2:                                                                     // 16 colour
            memset(vi.drawSurface,vi.pixelsPerByte == 1 ? 0x92:0xDD,vi.bufferSize);
            break;
        case 4:                                                                     // 4 level monochrome
            memset(vi.drawSurface,0xAA,vi.bufferSize);
            break;
        case 8:
            for (int y = 0;y < vi.yScreen;y++) {
                memset(vi.drawSurface+y*vi.bytesPerLine,(y & 1) ? 0xAA:0x55,vi.bytesPerLine);
            }
            break;
    }
}

