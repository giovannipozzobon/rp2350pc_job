// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      graphics_module.h
//      Purpose :   Graphics module header.
//      Date :      9th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "common_module.h"
#include "dvi_module.h"
#include "modes_module.h"

enum GFXCommand {
    Mode = 0,                                                                       // Set Graphics Mode to x
    Colour = 1,                                                                     // Set foreground to x, background to y or transparent = -1
    Scaling = 2,                                                                    // Set font scaling to x,y
    NoClip = 3,                                                                     // Reset clipping to full screen.
    PushClip = 4,                                                                   // Push current clip, set new clip
    PopClip = 5,                                                                    // Restore previous clip.

    Move = 20,                                                                      // Move cursor
    Plot = 21,                                                                      // Plot pixel
    Line = 22,                                                                      // Line from last position
    Rect = 23,                                                                      // Frame 
    FillRect = 24,                                                                  // Solid rectangle
    Ellipse = 25,                                                                   // Ellipse outline
    FillEllipse = 26,                                                               // Filled Ellipse
    Triangle = 27,                                                                  // Triangle outline.
    FillTriangle = 28,                                                              // Filled triangle
    Character = 29                                                                  // Character.
};



void GFXInitialise(void);
void GFXDraw(enum GFXCommand cmd,int32_t x,int32_t y);

typedef uint8_t *(*GFXMAPPER)(uint32_t *x,uint32_t *y);                             // Coordinate Mapper type

//
//      The rendering information for a single monochrome character. initially we just support simple 8x8 fonts, but this
//      should be expandable to any monochrome font.
//
typedef struct FontSource {
    uint8_t     *pixelData;                                                         // Pointer to the pixel data for this data.
    uint8_t     bytesPerLine;                                                       // How many 8 bit bytes per horizontal line of character
    uint8_t     height;                                                             // Height of character in pixels
    uint8_t     width;                                                              // Width of character in pixels (e.g. spacing)
    int8_t      xOffset,yOffset;                                                    // The offset of this character from the draw point (descenders etc)
} FONTSOURCE;

typedef FONTSOURCE *(*FONTSOURCEFUNCTION)(uint16_t code);                           // Function that gets scanline data

//
//      This is a physical clipping structure which can be applied to the display.
//
struct Clipping {
    int32_t     xLeft,yTop,xRight,yBottom;                                          // Clipping rectangle for drawing (inclusive)
};