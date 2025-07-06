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

#include "input_module.h"
#include "input_module_local.h"
#include "usb_keycodes.h"

static uint16_t _INPTranslateControl(uint8_t keyID,uint8_t modifiers);

/**
 * @brief      Handle a key press event
 *
 * @param[in]  keyID      USB Key ID
 * @param[in]  modifiers  Current state of the modifiers.
 */
void INPHandleKeyEvent(uint8_t keyID,uint8_t modifiers) {

    #define REBOOT_MASK (KEY_MOD_LCTRL|KEY_MOD_LALT|KEY_MOD_RALT)                   // Reboot on CTRL Alt Alt

    if ((modifiers & REBOOT_MASK) == REBOOT_MASK) {                                 // Check for C/A/A reboot.
        watchdog_enable(1,1);                                                       // Enable the watchdog timer
        while (true) {}                                                             // Ignore it.
    }

    if (keyID >= KEY_KP1 && keyID <= KEY_KP0) {                                     // Make Keypad number keys the same as 
        keyID = keyID-KEY_KP1+KEY_1;                                                // The actual number keys
    }
    
    uint16_t key = INPTranslateUSBCode(keyID,modifiers);                            // Translate to ASCII.
    if (key == 0) {                                                                 // Didn't work, so probably a control characte.
        key = _INPTranslateControl(keyID,modifiers);
    }
    
    if (key != 0 && (modifiers & (KEY_MOD_LCTRL|KEY_MOD_RCTRL)) != 0) {             // Ctrl something.
        key &= 0x1F;                                                                // Lower 5 bits only.
    }
    INPInsertIntoQueue(key);                                                        // Insert key into keyboard queue.
}


/**
 * @brief      Translate a USB code/modifier to a control code.
 *
 * @param[in]  keyID      USB Key ID
 * @param[in]  modifiers  Current state of the modifiers.
 *
 * @return     Control code or 0 if not translatable.
 */
static uint16_t _INPTranslateControl(uint8_t keyID,uint8_t modifiers) {
    uint16_t ret;

    switch(keyID) {                                                                 // Map USB codes onto the internal equivalent.
        case KEY_LEFT:
            ret = KBD_LEFT;break;
        case KEY_RIGHT:
            ret = KBD_RIGHT;break;
        case KEY_DOWN:
            ret = KBD_DOWN;break;
        case KEY_UP:
            ret = KBD_UP;break;
        case KEY_PAGEUP:
            ret = KBD_PAGEUP;break;
        case KEY_PAGEDOWN:
            ret = KBD_PAGEDOWN;break;
        case KEY_BACKSPACE:
            ret = KBD_BACKSPACE;break;
        case KEY_TAB:
            ret = KBD_TAB;break;
        case KEY_ENTER:
            ret = KBD_CRLF;break;
        case KEY_ESC:
            ret = KBD_ESCAPE;break;
        case KEY_HOME:
            ret = KBD_HOME;break;
        case KEY_END:
            ret = KBD_END;break;
        case KEY_INSERT:
            ret = KBD_INSERT;break;
        case KEY_DELETE:
            ret = KBD_DELETE;break;
            break;

        default:
            if (keyID >= KEY_F1 && keyID <= KEY_F12) {                              // Convert function key codes.
                ret = keyID-KEY_F1+KBD_F1;
            }
            break;
    }
    return ret;
}