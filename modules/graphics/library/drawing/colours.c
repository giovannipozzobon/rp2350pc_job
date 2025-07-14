// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      colours.c
//      Purpose :   Convert colour from RGB format to raw
//      Date :      14th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

/**
 * @brief      Convert an RGB colour to a raw one.
 *
 * @param[in]  rgb            RGB colour, 4 bits per colour, so the word is
 *                            0000rrrrggggbbbb
 * @param[in]  pixelsPerByte  Pixels in each byte
 *
 * @return     Raw colour for that mode.
 */
uint8_t GFXToRawColour(uint16_t rgb,uint8_t pixelsPerByte) {
    int r = (rgb & 0xF00) >> 8;                                                     // Convert each to 4 bit colour
    int g = (rgb & 0x0F0) >> 4;
    int b = (rgb & 0x00F);
    int sum = (r + g + b)/3;                                                        // Average brightness.
    uint8_t raw = 0;
    
    switch(pixelsPerByte) {
        case 1:                                                                      // 1 is RRRGGGBB
            raw = ((r >> 1) << 5) | ((g >> 1) << 2) | (b >> 2);
            break;
        case 2:
            raw = (r & 8) | ((g & 12) >> 1) | (b >> 3);break;
        case 4:
        case 8:
            raw = (pixelsPerByte == 4) ? sum >> 2 : sum >> 3;
            break;
    }

    return raw;
}