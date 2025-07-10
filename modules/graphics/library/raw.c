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

struct DrawingState draw;                                                           // Current drawing state

static uint8_t pixelMasks[9] = { 0,255,15,0,3,0,0,0,1 };                            // Pixel masks for each pixels/byte setting.

/**
 * @brief      Move the cursor position to x,y
 *
 * @param[in]  x     new x position
 * @param[in]  y     new y position
 */
void GFXRawMove(int16_t x,int16_t y) {
    LOGICALTOPHYSICAL(&x,&y);                                                       // Possible mapping.
    CHECKUPDATE();
    draw.inDrawingArea = (x >= draw.xLeft && x <= draw.xRight                       // Is it currently 'on' (e.g. in the drawing window.)
                                        && y >= draw.yTop && y <= draw.yBottom);    
    draw.currentByte = vi.drawSurface + (x / vi.pixelsPerByte) + y*vi.bytesPerLine; // The byte we are currently in.
    draw.pixelIndex = x % vi.pixelsPerByte;                                         // The index position in that byte (0 = Leftmost pixel,1 = Next Leftmost)

    draw.shiftsPerPixel = 8 / vi.pixelsPerByte;                                     // Shifts for each pixel, so 2 pixels per byte would be 4,8 would be 1.
    draw.pixelMask = pixelMasks[vi.pixelsPerByte];                                  // Mask for a pixel in the left most pixel (e.g. pixelIndex = 0)
}

/**
 * @brief      Draw a pixel in the current position in the current foreground colour.
 */
void GFXRawPlot(void) {
    CHECKUPDATE();
    if (draw.inDrawingArea) {                                                       // Are we in the drawing area, e.g. the clip window
        *draw.currentByte = draw.pixelMask;
    } 
}