// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      console_module.h
//      Purpose :   Console module internal include
//      Date :      14th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************


#pragma once
#ifndef RUNTIME
#endif

typedef struct _Console {
    int16_t     x,y;                        // Current cursor position.
    GFXCLIPRECT *rc;                        // Clip rect or NULL
    bool clearPending;                      // Clear is due on the first command. 
} CONCONFIG;

extern CONCONFIG *console;

void CONResetConsole(void);