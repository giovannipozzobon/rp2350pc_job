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
    Move = 0,
    Plot = 1,
    Line = 2
};

void GFXInitialise(void);
void GFXDraw(enum GFXCommand cmd,uint32_t x,uint32_t y);
void GFXPreProcess(uint32_t *x,uint32_t *y);