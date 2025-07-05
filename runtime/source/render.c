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
static uint8_t currentMode = 0;

/**
 * @brief      Initialise the rendering system
 */
void DVIInitialise(void) {    
}

/**
 * @brief      Set the rendering mode. Mode Information is documented in the DVI
 *             module
 *
 * @param[in]  modeInformation  The mode information
 */
void DVISetMode(uint16_t modeInformation) {
    currentMode = modeInformation;    
    printf("Current Mode %d\n",currentMode);
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
    rc.x = rc.y = 8;rc.w = AS_SCALE*640;rc.h = AS_SCALE*480;                        // Black background, so we don't have to render black.
    SYSRectangle(&rc,0);
    if (lineInfo == NULL || currentMode == 0) return;                               // No renderer/mode.
    for (int y = 0;y < 480;y++) {
        rc.w = AS_SCALE;rc.h = AS_SCALE;
        rc.x = 8;rc.y = y*rc.h+8;        
        SYSRectangle(&rc,0xFFF);
    } 
}
