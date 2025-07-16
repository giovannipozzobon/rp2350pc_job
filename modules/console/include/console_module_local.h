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
    int16_t     x,y;                                                                // Current cursor position, offset from top left.
    int16_t     xLeft,yTop,xRight,yBottom;                                          // Rectangle used for console.
    int16_t     ink,paper,cursor;                                                   // Colours used in drawing.
    bool        _cursorDrawn;                                                       // True if cursor drawn.
    int16_t     _xSize,_ySize;                                                      // Character size, current context (assumes monospaced)
} CONCONFIG;

extern CONCONFIG *console;

void CONResetConsole(void);
void CONClearWindow(void);