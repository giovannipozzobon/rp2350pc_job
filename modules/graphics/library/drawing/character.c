// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      character.c
//      Purpose :   Display a single character.
//      Date :      12th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

static void _GFXDrawRenderOneRow(uint32_t x,uint32_t y,FONTSOURCE *fs);

static FONTSOURCEFUNCTION currentFont = GFXGetSystemCharacter;                          // Current function for getting font information.

/**
 * @brief      Draw a single character
 *
 * @param[in]  x     Position of character left
 * @param[in]  y     Position of character top
 * @param[in]  code  ASCII or Uniocode
 *
 * @return     Width of character in pixels (low byte, mask off)
 */
uint32_t GFXDrawCharacter(uint32_t x,uint32_t y,uint32_t code) {

    FONTSOURCE *fs = (*currentFont)(code);                                              // Get character info and exit if not known.
    if (fs == NULL) return 0;
    x += fs->xOffset;y += fs->yOffset;                                                  // Characters can be offsete
    for (int yRow = 0;yRow < fs->height;yRow++) {                                       // For each scanline.
        for (int yScalar = 0;yScalar < draw.yFontScale;yScalar++) {                     // For each scalar
            if (y >= draw.yTop || y <= draw.yBottom && x <= draw.xRight) {              // If in window vertically and not off RHS.
                _GFXDrawRenderOneRow(x,y,fs);
            }
            y++;                                                                        // Next line down.
        }
        fs->pixelData += fs->bytesPerLine;                                              // Go to data for next row.
    }
    return fs->width * draw.xFontScale;                                                 // Return the vertical width.
}


/**
 * @brief      Render one row of the character
 *
 * @param[in]  x     position
 * @param[in]  y     position
 * @param      fs    Font Character information.
 */
static void _GFXDrawRenderOneRow(uint32_t x,uint32_t y,FONTSOURCE *fs) {    
    GFXRawMove(x,y);
    uint8_t *source = fs->pixelData;                                                    // Where the pixels are coming from.
    uint8_t current = *source++;

    for (uint32_t xPixel = 0;xPixel < fs->width;xPixel++) {                             // For each pixel.
        for (uint32_t yScalar = 0;yScalar < draw.xFontScale;yScalar++) {                // Do by scalar.
            if (current & 0x80) {                                                       // Foreground if set
                GFXRawPlot(true);
            } else {                                                                    // Background if clear and not transparent.
                if (!draw.isTransparent) GFXRawPlot(false);
            }
            GFXRawRight();
        }
        current <<= 1;                                                                  // Next bit in source data.
        if ((xPixel & 7) == 7) current = *source++;                                     // If the last bit then shift to next byte.
    }
}
    // uint8_t     *pixelData;                                                         // Pointer to the pixel data for this data.
    // uint8_t     bytesPerLine;                                                       // How many 8 bit bytes per horizontal line of character
    // uint8_t     height;                                                             // Height of character in pixels
    // uint8_t     width;                                                              // Width of character in pixels (e.g. spacing)
    // int8_t      xOffset,yOffset;                                                    // The offset of this character from the draw point (descenders etc)
