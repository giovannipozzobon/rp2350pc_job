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
typedef uint8_t *(*DVIRENDERER)(uint8_t func,uint8_t *data);                        // Function that renders a line manually.

void DVIInitialise(void);
void DVISetMode(uint16_t modeInformation);
void DVISetLineAccessorFunction(DVILINEACCESSOR dlafn);

#define DVIM_INITIALISE         (0)                                                 // Initialise manual renderer
#define DVIM_GETRENDER          (1)                                                 // Get the renderer for the given data.
#define DVIM_RENDERNEXT         (2)                                                 // Create the renderer for the given data.

typedef struct _DVIRenderBuffer {                                                   // A single buffer for render
    uint8_t *source;                                                                // Source address used to render data
    uint8_t render[640];                                                            // The rendered result.
} DVIRenderBuffer;

extern DVIRenderBuffer dviRender[2];                                                // Render buffer, there are two.

