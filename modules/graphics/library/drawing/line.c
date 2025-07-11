// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      line.c
//      Purpose :   Simple Bresenham Line drawer.
//      Date :      11th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

/**
 * @brief      Line drawing (simple Bresenham)
 *
 * @param[in]  x0             The x0 coordinate
 * @param[in]  y0             The y0 coordinate
 * @param[in]  x1             The x1 coordinate
 * @param[in]  y1             The y1 coordinate
 * @param[in]  drawLastPixel  Draw the last pixel.
 */
void GFXDrawLine(int x0, int y0, int x1, int y1,bool drawLastPixel) {

    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int error = dx + dy;

    GFXRawMove(x0,y0);                                                              // Move drawing cursor

    while(x0 != x1 || y0 != y1) {

        GFXRawPlot(true);                                                           // Draw a pixel.
        int e2 = 2 * error;
        if (e2 >= dy) {
            error = error + dy;
            if (sx < 0) {                 
                GFXRawLeft(); 
                x0--;
            } else { 
                GFXRawRight(); 
                x0++;
            }
        }
        if (e2 <= dx) {
            error = error + dx;
            if (sy < 0) { 
                GFXRawUp(); 
                y0--;
            } else { 
                GFXRawDown();
                y0++;
            }
        }
    }
    if (drawLastPixel) GFXRawPlot(true);                                            // Last pixel
}

