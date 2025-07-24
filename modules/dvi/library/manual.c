// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      manual.c
//      Purpose :   Default manual renderer (expands 320 to 640)
//      Date :      2nd July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "dvi_module.h"
#include "dvi_module_local.h"

static DVIRenderBuffer dviRender[2];                                                // We need 2 buffers - one line is being painted, one rendered.
static uint8_t mostRecentlyUsed = 0;

/**
 * @brief      Initialise the renderer.
 */
void KEEPINRAM(DVIRenderInitialise)(void) {
    dviRender[0].source = dviRender[1].source = NULL;                               // Neither represents an actual line.
    memset(dviRender[0].render,0xE0,640);                                           // For testing, so we can see what isn't rendered.
    memset(dviRender[1].render,0x18,640);                                     
}

/**
 * @brief      Get the line that should already have been rendered.
 *
 * @param      data  The address of the data that should be rendered
 *
 * @return     The rendered data.
 */
uint8_t *KEEPINRAM(DVIGetRenderedLine)(uint8_t *data) {
    uint8_t *retVal = NULL;
    if (dviRender[0].source == data) {                                              // This is a bit long winded but we can't guarantee
        retVal = dviRender[0].render;                                               // The renders will always be in order.
        mostRecentlyUsed = 0;
    }
    if (dviRender[1].source == data) {
        retVal = dviRender[1].render;
        mostRecentlyUsed = 1;
    }
    return retVal;
}

/**
 * @brief      Render a line into a buffer.
 *
 * @param      data  The scanline data to render.
 */
void KEEPINRAM(DVIRenderOneLine)(uint8_t *data) {
    if (dviRender[0].source != data && dviRender[1].source != data) {       // If not already rendered
        uint8_t n = 1 - mostRecentlyUsed;                                   // Use *this* buffer - not the most recently used.
        dviRender[n].source = data;                                         // Remember what it is rendering for getRender
        ASMRender160_256(dviRender[n].render,data,DVIPalette);
    }
}
