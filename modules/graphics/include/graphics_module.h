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
    RawColour = 1,                                                                  // Set foreground to x, background to y/transparent=-1
    Colour = 2,                                                                     // As Raw, but colours are 4 bit RGB.
    Scaling = 3,                                                                    // Set font scaling to x,y
    NoClip = 4,                                                                     // Reset clipping to full screen.
    PushClip = 5,                                                                   // Push current clip, set new clip
    PopClip = 6,                                                                    // Restore previous clip.
    SetMapper = 7,                                                                  // Set mapper function.

    Move = 20,                                                                      // Move cursor
    Plot = 21,                                                                      // Plot pixel
    Line = 22,                                                                      // Line from last position
    Rect = 23,                                                                      // Frame 
    FillRect = 24,                                                                  // Solid rectangle
    Ellipse = 25,                                                                   // Ellipse outline
    FillEllipse = 26,                                                               // Filled Ellipse
    Triangle = 27,                                                                  // Triangle outline.
    FillTriangle = 28,                                                              // Filled triangle
    Character = 29,                                                                 // Character.
    CharExtent = 30,                                                                // Character extent.
    Clear = 31,                                                                     // Clear screen
    Desktop = 32                                                                    // Clear to desktop
};

#ifdef RUNTIME
typedef uint64_t GFXDRAWPARAM;                                                      // Type of GFXDraw non command parameters.
#else
typedef uint32_t GFXDRAWPARAM;
#endif 

void GFXInitialise(void);
uint32_t GFXDraw(enum GFXCommand cmd,GFXDRAWPARAM x64,GFXDRAWPARAM y64);

typedef void *(*GFXMAPPER)(uint32_t *x,uint32_t *y);                                // Coordinate Mapper type

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
} GFXFONTSOURCE;

typedef GFXFONTSOURCE *(*GFXFONTSOURCEFUNCTION)(uint16_t code);                     // Function that gets scanline data

//
//      This is a physical clipping structure which can be applied to the display.
//
typedef struct _clipping {
    int32_t     xLeft,yTop,xRight,yBottom;                                          // Clipping rectangle for drawing (inclusive)
} GFXCLIPRECT;