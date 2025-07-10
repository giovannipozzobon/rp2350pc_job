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
    uint16_t    x,y;                                                                // Current position.
    uint8_t     foreground,background;                                              // Colour pixels (already masked correctly for mode) in LSB positions.
    bool        inDrawingArea;                                                      // True if currently drawing (e.g. as move, drawing occurs)
    uint8_t     *currentByte;                                                       // Current byte.
    int8_t      pixelIndex;                                                         // Index in that pixel (0 = left most byte)
    int8_t      shiftsPerPixel;                                                     // How many shifts per index pixel.
    uint8_t     pixelMask;                                                          // Mask for left most pixel, so if 4 pixels per byte would be 11
};

extern struct DrawingState draw;                                                    // Current draw information.

#define CHECKUPDATE()               GFXCheckModeChange()

void GFXRawMove(int16_t x,int16_t y);
void GFXRawPlot(bool useFgr);
void GFXRawUp(void);
void GFXRawDown(void);
void GFXRawLeft(void);    
void GFXRawRight(void);

void GFXCheckModeChange(void);