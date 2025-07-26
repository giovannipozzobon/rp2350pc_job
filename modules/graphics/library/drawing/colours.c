// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      colours.c
//      Purpose :   Convert colour from RGB format to raw
//      Date :      26th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

static uint8_t rggbPalette[16];
static bool isInitialised = false;

/**
 * @brief      Convert a colour to a raw one.
 *
 * @param[in]  rgb            If 0-15 represents an RGGB palette colour,
 *                            otherwise use the RRRGGGBB palette. Assumes the
 *                            palette is the default.
 * @param[in]  pixelsPerByte  Pixels in each byte
 *
 * @return     Raw colour for that mode.
 */
uint8_t GFXToRawColour(uint16_t rgb,uint8_t pixelsPerByte) {
    static uint8_t greenPalette[4] = { 0,0x08,0x10,0x1C };
    if (!isInitialised) {                                                           // Create the mapping first time used.
        isInitialised = true;
        for (int c = 0;c < 16;c++) {
            rggbPalette[c] = greenPalette[(c & 0x06) >> 1];                         // Green component
            rggbPalette[c] += ((c & 0x08) ? 0xE0:0x00);                             // Red component
            rggbPalette[c] += ((c & 0x01) ? 0x03:0x00);                             // Blue component
        }
    }
    uint8_t colour = 0;
    switch (pixelsPerByte) {
        case 1:                                                                     // 256 colour
            colour = (rgb >= 16) ? rgb:rggbPalette[rgb];break;
        case 2:                                                                     // 16 colour
            colour = rgb;break;
        case 4:                                                                     // 4 colour monochrome
            colour = rgb & 3;break;                                                 
        case 8:                                                                     // 2 colour monochrome
            colour = rgb & 1;break;

        }
    return colour;
}
