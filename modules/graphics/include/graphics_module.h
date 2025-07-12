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
    Move = 0,                                                                       // Move cursor
    Plot = 1,                                                                       // Plot pixel
    Line = 2,                                                                       // Line from last position
    Rect = 3,                                                                       // Frame from last position.
    FillRect = 4,                                                                   // Solid rectangle
    Ellipse = 5,                                                                    // Ellipse outline
    FillEllipse = 6,                                                                // Filled Ellipse
    FillTriangle = 7                                                                // Filled triangle
};

void GFXInitialise(void);
void GFXDraw(enum GFXCommand cmd,int32_t x,int32_t y);
void GFXPreProcess(int32_t *x,int32_t *y);