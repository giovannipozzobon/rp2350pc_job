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

#define LOCALS
#include "common_module.h"
#include "dvi_module.h"

// *******************************************************************************************
// 
//                          Manual Renderer (320 - 640 bytes)
// 
// *******************************************************************************************

uint8_t *DVIManualRenderer(uint8_t func,uint8_t *data) {

    uint8_t *retVal = NULL;

    switch(func) {
        case DVIM_INITIALISE:
            break;
        case DVIM_GETRENDER:
            retVal = data;
            break;
        case DVIM_RENDERNEXT:
            break;
    }
    return retVal;
}

// *******************************************************************************************
//
//      We do not know what order the renders are coming in, or whether they are duplicates, 
//      so with each buffer we store the address of the data that is rendered.
//      
//      The simplest behaviour of the renderer (which is pointless) is simply to return the
//      provided data on DVIM_GETRENDER and ignore everything else.
//      
// *******************************************************************************************
