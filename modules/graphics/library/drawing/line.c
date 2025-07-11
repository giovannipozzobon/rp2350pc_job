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

static void GFXOptimisedHorizontalLine(int32_t x0, int32_t x1, int32_t y);

/**
 * @brief      Line drawing (simple Bresenham)
 *
 * @param[in]  x0             The x0 coordinate
 * @param[in]  y0             The y0 coordinate
 * @param[in]  x1             The x1 coordinate
 * @param[in]  y1             The y1 coordinate
 * @param[in]  drawLastPixel  Draw the last pixel.
 */
void GFXDrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1,bool drawLastPixel) {

    if (y0 == y1) {                                                                 // Horizontal line.
        if (y0 >= draw.yTop && y0 <= draw.yBottom && x0 >= draw.xLeft &&            // If whole line is not clipped by the window.
            x0 <= draw.xRight && x1 >= draw.xLeft && x1 <= draw.xRight) {  
                if (abs(x0-x1)/(4*vi.pixelsPerByte) >= 3) {                         // If there is sufficient to make it worth using.
                    GFXOptimisedHorizontalLine(x0,x1,y1);
                    GFXRawMove(x1,y1);
                    return;
                }
        }
    }
    int32_t dx = abs(x1 - x0);
    int32_t sx = x0 < x1 ? 1 : -1;
    int32_t dy = -abs(y1 - y0);
    int32_t sy = y0 < y1 ? 1 : -1;
    int32_t error = dx + dy;

    GFXRawMove(x0,y0);                                                              // Move drawing cursor

    while(x0 != x1 || y0 != y1) {

        GFXRawPlot(true);                                                           // Draw a pixel.
        int32_t e2 = 2 * error;
        if (e2 >= dy) {
            error = error + dy;
            if (sx < 0) {                 
                GFXRawLeft(); 
            } else { 
                GFXRawRight(); 
            }
            x0 += sx;
        }
        if (e2 <= dx) {
            error = error + dx;
            if (sy < 0) { 
                GFXRawUp(); 
            } else { 
                GFXRawDown();
            }
            y0 += sy;
        }
    }
    if (drawLastPixel) GFXRawPlot(true);                                            // Last pixel
}

static void GFXOptimisedHorizontalLine(int32_t x0, int32_t x1, int32_t y) {
    if (x0 > x1) {                                                                  // Put x0 and x1 in left to right order.
        int32_t t = x0;x0 = x1;x1 = t;
    }
    int32_t modReqd = 4 * vi.pixelsPerByte;                                         // Modulus of x0 that needs to be zero to be on a word boundary.
    while (x0 % modReqd != 0) {
        x0++;GFXRawPlot(true);GFXRawRight();
    }
    int wordsToDo = (x1 - x0) / modReqd;                                            // This is the number of words we can write.
    if (wordsToDo != 0) {
        uint32_t colour = draw.foreground;
        if (vi.pixelsPerByte == 2) colour = (colour & 0x0F) * 0x11;
        if (vi.pixelsPerByte == 4) colour = (colour & 0x03) * 0x55;
        if (vi.pixelsPerByte == 8) colour = (colour & 0x01) ? 0xFF:0x00;

        colour = colour | (colour << 8) | (colour << 16) | (colour << 24);          // Replicate each byte through the 32 bit word.
        for (int i = 0;i < wordsToDo;i++) {
            x0 += modReqd;   
            GFXRawWordRight(colour);
        }
    }

    while (x0 < x1) {                                                               // Complete it.
        x0++;GFXRawPlot(true);GFXRawRight();        
    }
}

