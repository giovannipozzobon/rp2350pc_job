// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      process.c
//      Purpose :   Converts the USB keyboard/Modifiers to ASCII.
//      Date :      3rd July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "common_module.h"
#include "usb_module.h"

#define LOCALS
#include "input_module.h"
#include "usb_keycodes.h"

/**
 * @brief      Handle a key press event
 *
 * @param[in]  keyID      USB Key ID
 * @param[in]  modifiers  Current state of the modifiers.
 */
void INPHandleKeyEvent(uint8_t keyID,uint8_t modifiers) {

    if (keyID >= KEY_KP1 && keyID <= KEY_KP0) {                                     // Make Keypad number keys.
        keyID = keyID-KEY_KP1+KEY_1;        
    }
    
    LOG("Key event %d %x",keyID,modifiers);
}