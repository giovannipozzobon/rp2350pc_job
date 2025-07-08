// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      render.c
//      Purpose :   Graphic display renderer.
//      Date :      5th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include <runtime.h>

static DVILINEACCESSOR lineInfo = NULL;
static uint16_t currentMode = 0;

static bool initialised = false;
static Uint32 palette256[256],palette16[16],palette4[4];

static void _DVIRender256Colours(SDL_Rect *rc,uint8_t *data,uint16_t pixels);
static void _DVIRender16Colours(SDL_Rect *rc,uint8_t *data,uint16_t pixels);
static void _DVIRender4GreyScale(SDL_Rect *rc,uint8_t *data,uint16_t pixels);
static void _DVIRenderMonochrome(SDL_Rect *rc,uint8_t *data,uint16_t pixels);

/**
 * @brief      Convert an unmasked bit and width to a 8 bit value
 *
 * @param[in]  bits   unmasked bit
 * @param[in]  width  width
 *
 * @return     byte colour value
 */
static Uint8 _DVIMakeColour(Uint8 bits,Uint8 width) {
    bits = bits & ((1 << width) - 1);                                               // Mask off (so width = 3 would be 1 << 3 -1 or 7)
    bits = bits << (8 - width);                                                     // Shift up.
    return bits;
}

/**
 * @brief      Convert a RGB values to a colour.
 *
 * @param[in]  r      red bits unmasked
 * @param[in]  rBits  red width
 * @param[in]  g      red bits unmasked
 * @param[in]  gBits  green width
 * @param[in]  b      blue bits unmasked
 * @param[in]  bBits  blue width
 *
 * @return     32 bit RGB colour
 */
static Uint32 _DVIMakeRGB(Uint8 r,Uint8 rBits,Uint8 g,Uint8 gBits,Uint8 b,Uint8 bBits) {
    return SDL_MapRGB(SYSGetSurface()->format,_DVIMakeColour(r,rBits),_DVIMakeColour(g,gBits),_DVIMakeColour(b,bBits));
}
/**
 * @brief      Initialise the rendering system
 */
void DVIInitialise(void) {    
    if (initialised) return;
    initialised = true;
    for (int i = 0;i < 256;i++) {
        palette256[i] = _DVIMakeRGB(i >> 5,3,i >> 2,3,i,2);
    }
    for (int i = 0;i < 16;i++) {
        palette16[i] = _DVIMakeRGB(i >> 3,1,i >> 1,2,i,1);
    }
    for (int i = 0;i < 4;i++) {
        palette4[i] = _DVIMakeRGB(i,2,i,2,i,2);
    }
}

/**
 * @brief      Set the rendering mode. Mode Information is documented in the DVI
 *             module
 *
 * @param[in]  modeInformation  The mode information
 */
void DVISetMode(uint16_t modeInformation) {
    currentMode = modeInformation;    
    //printf("Current Mode %d\n",currentMode);
}

/**
 * @brief      Set the Line accessor function
 *
 * @param[in]  dlafn  Get the line accessor function.
 */
void DVISetLineAccessorFunction(DVILINEACCESSOR dlafn) {
    lineInfo = dlafn;
}

/**
 * @brief      Render the display
 *
 * @param      surface  The surface to render it on
 */
void RNDRender(SDL_Surface *surface) {  
    uint8_t *pr,*pg,*pb,r,g,b;
    SDL_Rect rc;
    rc.x = rc.y = 8;rc.w = AS_SCALEX*640;rc.h = AS_SCALEY*480;                      // Black background, so we don't have to render black.
    SYSRectangle(&rc,0);
    if (lineInfo == NULL || currentMode == 0) return;                               // No renderer/mode.
    for (int y = 0;y < 480;y++) {
        rc.w = AS_SCALEX;rc.h = AS_SCALEY;                                          // Get the drawing rectangle.
        rc.x = 8;rc.y = y*rc.h+8;                                                   
        uint8_t *data = (*lineInfo)(y);                                             // Get the drawing data.
        if (data != NULL) {
            switch(currentMode) { 
                case 1:                                                             // Mode 1 is 640x480x256
                    _DVIRender256Colours(&rc,data,640);break;
                case 0x4001:                                                        // Mode $4001 is 320x480x256
                    rc.w = rc.w * 2;_DVIRender256Colours(&rc,data,320);break;
                case 0x8001:                                                        // Mode $8001 is 160x480x256
                    rc.w = rc.w * 4;_DVIRender256Colours(&rc,data,160);break;
                case 2:                                                             // Mode 2 is 640x480x16
                    _DVIRender16Colours(&rc,data,640);break;
                case 4:                                                             // Mode 4 is 640x480x4 greyscales.
                    _DVIRender4GreyScale(&rc,data,640);break;
                case 8:                                                             // Mode 8 is 640x480x1 monochrome
                    _DVIRenderMonochrome(&rc,data,640);break;
            }
        }
    } 
}

/**
 * @brief      256 colour renderer
 *
 * @param      rc      Pixel drawing rectangle
 * @param      data    Colour Data
 * @param[in]  pixels  Pixel Count
 */
static void _DVIRender256Colours(SDL_Rect *rc,uint8_t *data,uint16_t pixels) {
    SDL_Surface *surface = SYSGetSurface();
    while (pixels-- > 0) {
        if (*data != 0) {
            SDL_FillRect(surface,rc,palette256[*data]);
        }
        rc->x += rc->w;
        data++;
    }
}

/**
 * @brief      16 colour renderer
 *
 * @param      rc      Pixel drawing rectangle
 * @param      data    Colour Data
 * @param[in]  pixels  Pixel Count
 */
static void _DVIRender16Colours(SDL_Rect *rc,uint8_t *data,uint16_t pixels) {
    SDL_Surface *surface = SYSGetSurface();
    while (pixels > 0) {
        if (((*data) & 0x0F) != 0) {
            SDL_FillRect(surface,rc,palette16[(*data) & 0x0F]);
        }
        rc->x += rc->w;;pixels--;
        if (((*data) >> 4) != 0) {
            SDL_FillRect(surface,rc,palette16[(*data) >> 4]);
        }
        rc->x += rc->w;pixels--;
        data++;
    }
}

/**
 * @brief      4 Greyscale renderer
 *
 * @param      rc      Pixel drawing rectangle
 * @param      data    Pixel Data
 * @param[in]  pixels  Pixel Count
 */
static void _DVIRender4GreyScale(SDL_Rect *rc,uint8_t *data,uint16_t pixels) {
    SDL_Surface *surface = SYSGetSurface();
    while (pixels > 0) {
        uint8_t byte = *data++;
        pixels = pixels - 4;
        for (int p = 0;p < 4;p++) {
            if ((byte & 3) != 0) {
                SDL_FillRect(surface,rc,palette4[byte & 3]);
            }            
            rc->x += rc->w;
            byte = byte >> 2;
        }
    }
}

/**
 * @brief      Monochrome renderer
 *
 * @param      rc      Pixel drawing rectangle
 * @param      data    Pixel Data
 * @param[in]  pixels  Pixel Count
 */
static void _DVIRenderMonochrome(SDL_Rect *rc,uint8_t *data,uint16_t pixels) {
    SDL_Surface *surface = SYSGetSurface();
    while (pixels > 0) {
        uint8_t byte = *data++;
        pixels = pixels - 8;
        for (int p = 0;p < 8;p++) {
            if ((byte & 1) != 0) {
                SDL_FillRect(surface,rc,palette4[3]);
            }            
            rc->x += rc->w;
            byte = byte >> 1;
        }
    }
}