// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      input_module.h
//      Purpose :   Input Manager System Header files
//      Date :      2nd July 2025
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

void INPInitialise(void);
void INPUpdate(void);

#ifdef LOCALS

typedef struct _KeyStatus {
    uint8_t     keyID;                                                              // ID of key
    uint8_t     modifiers;                                                          // Modifier at time key is pressed.
    bool        isDown;                                                             // true when key active (down), false when inactive
    uint32_t    repeatTime;                                                         // time to repeat, if still down.
} KEYSTATUS;

void INPProcessKeyboardReport(USBREPORT *r);
void INPInitialiseStatus(void);
void INPHandleKeyEvent(uint8_t keyID,uint8_t modifiers);

#endif

