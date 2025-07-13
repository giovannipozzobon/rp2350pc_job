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

/**
 * @brief      Execute a graphics command
 *
 * @param[in]  cmd   command ID
 * @param[in]  x     x Coordinate
 * @param[in]  y     y Coordinate
 */

void GFXDraw(enum GFXCommand cmd,int32_t x,int32_t y) {
    switch(cmd) {
        case Mode:                                                                  // Set screen mode
            VMDSetMode(x);
            GFXCheckModeChange();
            break;
        case Colour:                                                                // Set Colour
            draw.foreground = x & 0xFFFF;draw.background = y & 0xFFFF;
            draw.isTransparent = false;
            if ((y & 0xFFFF) == 0xFFFF) { draw.background = 0;draw.isTransparent = true; }
            break;
        case Scaling:                                                               // Set font scaling.
            draw.xFontScale = x;draw.yFontScale = y;
            break;
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
    }
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
    draw.xPrev[2] = draw.xPrev[1];draw.yPrev[2] = draw.yPrev[1];                    // Push coordinates onto previous lists.
    draw.xPrev[1] = draw.xPrev[0];draw.yPrev[1] = draw.yPrev[0];
    draw.xPrev[0] = draw.x;       draw.yPrev[0] = draw.y;
}

