// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      input_module_local.h
//      Purpose :   Input Manager System Header files
//      Date :      2nd July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#pragma once

#ifdef RUNTIME
#define USEFLASH
#else
#include "pico/stdlib.h"
#define USEFLASH __in_flash()
#endif


extern INPGAMEPAD gp;

typedef struct _KeyStatus {
    uint8_t     keyID;                                                              // ID of key
    uint8_t     modifiers;                                                          // Modifier at time key is pressed.
    bool        isDown;                                                             // true when key active (down), false when inactive
    uint32_t    repeatTime;                                                         // time to repeat, if still down.
} INPKEYSTATUS;

void INPProcessKeyboardReport(USBREPORT *r);
void INPProcessMouseReport(USBREPORT *r);
void INPProcessGenericReport(USBREPORT *r);
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

//
//      Generic drivers (mostly gamepad ?)
//
void INPGeneric_081f_e401(USBREPORT *r);

