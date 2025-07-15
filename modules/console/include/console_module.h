// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      console_module.h
//      Purpose :   Console module external include
//      Date :      14th July 2025
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
#include "graphics_module.h"

void CONInitialise(void);
void CONWrite(uint16_t ch);
void CONSetWindow(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);