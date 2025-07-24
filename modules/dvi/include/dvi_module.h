// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      dvi_module.h
//      Purpose :   DVI Sytem common header files.
//      Date :      25th June 2025
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

typedef uint8_t *(*DVILINEACCESSOR)(uint16_t scanLine);                             // Function that gets scanline data

void DVIInitialise(void);
void DVISetMode(uint16_t modeInformation);
void DVISetLineAccessorFunction(DVILINEACCESSOR dlafn);                                                    
uint32_t DVIGetScanLineTime(void);

extern bool verticalSyncOccurred;

//
//      Modes available at present. The parameters are the horizontal width, the number of colours supported, and variable/fixed palette.
//
#define DVIR_COUNT          (3)

#define DVIR_160_256_V      (0)
#define DVIR_320_256_V      (1)
#define DVIR_640_256_F      (2)
