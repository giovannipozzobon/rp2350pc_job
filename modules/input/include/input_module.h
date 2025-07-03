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
#include <ctype.h>

#include "common_module.h"
#include "usb_module.h"

void INPInitialise(void);
void INPUpdate(void);
bool *INPGetKeyboardState(void);
bool INPSetLocale(char *locale);
int16_t INPGetKey(void);
bool INPIsKeyAvailable(void);

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
uint16_t INPTranslateUSBCode(uint8_t keyID,uint8_t modifiers);
void INPInsertIntoQueue(int16_t key);
void INPResetQueue(void);

extern char *locales;
extern const uint16_t localeMapping[];

#define KBD_LEFT        (1)
#define KBD_RIGHT       (2) 
#define KBD_DOWN        (3)
#define KBD_UP          (4)
#define KBD_PAGEDOWN    (5)
#define KBD_PAGEUP      (6)
#define KBD_BACKSPACE   (8)                                                         // e.g. back and erase
#define KBD_TAB         (9) 
#define KBD_CRLF        (10)                                                        // This matches the PICO C compiler.
#define KBD_CR          (13)
#define KBD_F1          (14)                                                        // 14-25 are the function keyaas.
#define KBD_F12         (25)
#define KBD_ESCAPE      (27)

#define KBD_HOME        (28)
#define KBD_END         (29)
#define KBD_INSERT      (30)
#define KBD_DELETE      (31)                                                        // e.g. erase at cursor
#endif

