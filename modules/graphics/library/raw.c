// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      raw.c
//      Purpose :   Graphics module raw drawers
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
 * @brief      Move the cursor position to x,y.
 *
 * @param[in]  x     new x position
 * @param[in]  y     new y position
 */
void GFXRawMove(int32_t x,int32_t y) {
    CHECKUPDATE();                                                                  // Possible mode change.
    draw.x = x;draw.y = y;
    draw.inDrawingArea = (x >= draw.xLeft && x <= draw.xRight                       // Is it currently 'on' (e.g. in the drawing window.)
                                        && y >= draw.yTop && y <= draw.yBottom);    
    draw.currentByte = vi.drawSurface + (x / vi.pixelsPerByte) + y*vi.bytesPerLine; // The byte we are currently in.
    draw.shiftsPerPixel = 8 / vi.pixelsPerByte;                                     // Shifts for each pixel, so 2 pixels per byte would be 4,8 would be 1.
    draw.pixelIndex = (x % vi.pixelsPerByte) * draw.shiftsPerPixel;                 // The index position in that byte this is counting overall shifts.
    draw.pixelMask = pixelMasks[vi.pixelsPerByte];                                  // Mask for a pixel in the left most pixel (e.g. pixelIndex = 0)
}

/**
 * @brief      Draw a pixel in the current position in the current foreground colour.
 */
void GFXRawPlot(bool useFgr) {
    if (draw.inDrawingArea) {                                                       // Are we in the drawing area, e.g. the clip window
        if (vi.pixelsPerByte == 1) {                                                // Optimise for 1 pixel = 1 byte.
            *draw.currentByte = useFgr ? draw.foreground:draw.background;
        } else {                                                                    // Multi-pixels per byte.                   
            *draw.currentByte = (*draw.currentByte)
                    & (~(draw.pixelMask << draw.pixelIndex))                        // Mask out the pixel mask shifted
                    | (((useFgr ? draw.foreground : draw.background) & draw.pixelMask) << draw.pixelIndex);   
        }
    } 
}

/**
 * @brief      Move up. 
 */
void GFXRawUp(void) {
    draw.currentByte += vi.bytesPerLine;
    draw.y--;
    draw.inDrawingArea = (draw.y >= draw.yTop && draw.y <= draw.yBottom);
}

/**
 * @brief      Move down
 */
void GFXRawDown(void) {    
    draw.currentByte += vi.bytesPerLine;
    draw.y++;
    draw.inDrawingArea = (draw.y >= draw.yTop && draw.y <= draw.yBottom);
}

/**
 * @brief      Move left
 */
void GFXRawLeft(void) {    
    draw.x--;                                                                       // Track horizontal position. 
    draw.pixelIndex -= draw.shiftsPerPixel;                                         // Shift left, which is moving right.
    if (draw.pixelIndex < 0) {                                                      // Done the whole byte.
        draw.pixelIndex += 8;                                                       // Back to rh pixel
        draw.currentByte--;                                                         // In the next byte left.
    }
    draw.inDrawingArea = (draw.x >= draw.xLeft && draw.x <= draw.xRight);           // Check in/out window.
}

/**
 * @brief      Move right
 */
void GFXRawRight(void) {   
    draw.x++;                                                                       // Track horizontal position. 
    draw.pixelIndex += draw.shiftsPerPixel;                                         // Shift left, which is moving right.
    if (draw.pixelIndex == 8) {                                                     // Done the whole byte.
        draw.pixelIndex = 0;                                                        // Back to lh pixel
        draw.currentByte++;                                                         // In the next byte right.
    }
    draw.inDrawingArea = (draw.x >= draw.xLeft && draw.x <= draw.xRight);           // Check in/out window.
}