// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      palette.c
//      Purpose :   Palette initialisation
//      Date :      24th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "dvi_module.h"
#include "dvi_module_local.h"

uint8_t DVIPalette[256];

/**
 * @brief      Initialise the palette
 */
void KEEPINRAM(DVIInitialisePalette)(void) {
    for (int i = 0;i < 256;i++) {
        DVIPalette[i] = i ^ 0xFF;
    }
}