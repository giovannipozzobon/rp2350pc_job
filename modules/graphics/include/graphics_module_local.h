// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      graphics_module_local.h
//      Purpose :   Graphics module local header.
//      Date :      9th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#pragma once
#ifndef RUNTIME
#endif

//
//      This represents the current pixel drawing state.
//
struct DrawingState {
    uint32_t    currentMode;                                                        // Current mode set up for.
    int16_t     xLeft,yTop,xRight,yBottom;                                          // Clipping rectangle for drawing (inclusive)
    bool        inDrawingArea;                                                      // True if currently drawing (e.g. as move, drawing occurs)
    uint16_t    x,y;                                                                // Current position.
    uint8_t     *displayByte;                                                       // Pointer to current display byte.
    int8_t      currentShift;                                                       // The current shift to convert colour data -> bit data.

    uint16_t    shift;                                                              // Number of shifts in byte for current pixel.
    uint8_t     mask;                                                               // Mask to mask background pixels, clearing foreground pixels, doesn't shift.
    uint8_t     foreground,background;                                              // Colour pixels (already masked correctly for mode) in LSB positions.
};

extern struct DrawingState draw;