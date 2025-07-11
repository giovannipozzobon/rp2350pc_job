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

void GFXDraw(enum GFXCommand cmd,uint32_t x,uint32_t y) {
    switch(cmd) {
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
    }
}

/**
 * @brief      Preprocess coordinates. Does logical->physical conversion and
 *             tracks previous coordinates.
 *
 * @param      x     address of x
 * @param      y     address of y
 */
void GFXPreProcess(uint32_t *x,uint32_t *y) {
    // Logical to Physical mapping.
    draw.xPrev[2] = draw.xPrev[1];draw.yPrev[2] = draw.yPrev[1];                    // Push coordinates onto previous lists.
    draw.xPrev[1] = draw.xPrev[0];draw.yPrev[1] = draw.yPrev[0];
    draw.xPrev[0] = draw.x;       draw.yPrev[0] = draw.y;
}