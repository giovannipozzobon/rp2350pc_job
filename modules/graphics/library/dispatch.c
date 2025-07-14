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

/**
 * @brief      Execute a graphics command
 *
 * @param[in]  cmd   command ID
 * @param[in]  x     x Coordinate
 * @param[in]  y     y Coordinate
 * 
 * @return     Return value depends.
 */
uint32_t GFXDraw(enum GFXCommand cmd,GFXDRAWPARAM x64,GFXDRAWPARAM y64) {    
    uint32_t x = (uint32_t)x64;
    uint32_t y = (uint32_t)y64;

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
            draw.foreground = x & 0xFFFF;draw.background = y & 0xFFFF;
            draw.isTransparent = false;
            if ((y & 0xFFFF) == 0xFFFF) { draw.background = 0;draw.isTransparent = true; }
            break;

        case Scaling:                                                               // Set font scaling.
            draw.xFontScale = x;draw.yFontScale = y;
            break;

        case NoClip:                                                                // Reset all clipping.
            GFXResetClipping();
            break;

        case PushClip:                                                              // Push current clip and set it
            if (draw.clipStackIndex < CLIPSTACKSIZE) {
                draw.clipStack[draw.clipStackIndex++] = draw.clip;
                draw.clip = (GFXCLIPRECT *)x64;
            }
            break;

        case PopClip:                                                               // Pop clip off stack.
            if (draw.clipStackIndex > 0) {
                draw.clip = draw.clipStack[--draw.clipStackIndex];
            }
            break;

        case SetMapper:                                                             // Set the mapping from logical to physical
            draw.mapper = (GFXMAPPER)x64;                                           // (by default logical == physical)
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
            GFXDrawLine(draw.xPrev[0],draw.yPrev[0],x,y,true);
            break;

        case Rect:                                                                  // Rectangles
        case FillRect:
            GFXPreProcess(&x,&y);
            GFXDrawRectangle(draw.xPrev[0],draw.yPrev[0],x,y,cmd == FillRect);
            GFXRawMove(x,y);
            break;

        case Ellipse:                                                               // Ellipses
        case FillEllipse:
            GFXPreProcess(&x,&y);
            GFXDrawEllipse(draw.xPrev[0],draw.yPrev[0],x,y,cmd == FillEllipse);
            GFXRawMove(x,y);
            break;

       case Triangle:                                                               // Outline triangle
            GFXPreProcess(&x,&y);
            GFXDrawOutlineTriangle(draw.xPrev[1],draw.yPrev[1],draw.xPrev[0],draw.yPrev[0],x,y);
            GFXRawMove(x,y);
            break;

        case FillTriangle:                                                          // Filled triangle
            GFXPreProcess(&x,&y);
            GFXDrawFilledTriangle(draw.xPrev[1],draw.yPrev[1],draw.xPrev[0],draw.yPrev[0],x,y);
            GFXRawMove(x,y);
            break;

        case Character:                                                             // Draw a character
            uint32_t xOrg = draw.x,yOrg = draw.y;
            x = GFXDrawCharacter(draw.x,draw.y,x);
            GFXRawMove(xOrg+(x & 0xFF),yOrg);            
            break;

        case CharExtent:                                                            // Get character extent.
            retVal = GFXGetCharacterExtent(x);                                      // Height in upper 16 bits, Width in lower1 16 bits
            break;

        case Clear:                                                                 // Clear to background
            memset(vi.drawSurface,draw.background,vi.bufferSize);
            break;

        case Desktop:                                                               // Clear to desktop
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
    if (draw.mapper != NULL) {
        (*draw.mapper)(x,y);
    }
    // Push coordinates onto previous lists.
    draw.xPrev[2] = draw.xPrev[1];draw.yPrev[2] = draw.yPrev[1];                    
    draw.xPrev[1] = draw.xPrev[0];draw.yPrev[1] = draw.yPrev[0];
    draw.xPrev[0] = draw.x;       draw.yPrev[0] = draw.y;
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