// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      status.c
//      Purpose :   Tracks key presses and autorepeat.
//      Date :      3rd July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "common_module.h"
#include "usb_module.h"

#define LOCALS
#include "input_module.h"

#define MAXKEYDOWN      (8)                                                         // Max # of simultaneous keys.

static uint8_t lastKeyPressed;                                                      // Index of last key pressed.
static KEYSTATUS keys[MAXKEYDOWN];                                                  // Keys currently down.

static uint16_t repeatDelay = 700;                                                  // ms before delay starts.
static uint16_t repeatRate = 100;                                                   // ms for each repeat

/**
 * @brief      Initialise the status manager.
 */
void INPInitialiseStatus(void) {
    lastKeyPressed = 0;
    for (int i = 0;i < MAXKEYDOWN;i++) keys[i].isDown = false;                      // No keys currently down.
}

/**
 * @brief      Process a keyboard report, converts into keyboard events
 *
 * @param      r     Keyboard report.
 */
void INPProcessKeyboardReport(USBREPORT *r) {
    //
    //      Check if any keys currently down have been released.
    //
    for (int i = 0;i < MAXKEYDOWN;i++) {                                            // For each key thought to be down.
        if (keys[i].isDown) {
            bool stillDown = false;                                                 // Check if it is still down.
            for (int j = 2;j < r->length;j++) {
                if (r->data[j] == keys[i].keyID) stillDown = true;
            }
            if (!stillDown) {                                                       // Key released.
                keys[i].isDown = false;                                             // Mark released.
            }
        }
    }
    //
    //      Check for any new key presses.
    //
    for (int j = 2;j < r->length;j++) {                                             // Look for keys that are down.
        if (r->data[j] != 0) {
            bool isKnown = false;                                                   // Is there a record for it already.
            for (int i = 0;i < MAXKEYDOWN;i++) {
                if (keys[i].isDown && keys[i].keyID == r->data[j]) isKnown = true;
            }
            if (!isKnown) {                                                         // New key down.
                uint8_t emptyRec = 0xFF;                                            // Find a slot for it.
                for (int i = 0;i < MAXKEYDOWN;i++) {
                    if (!keys[i].isDown) emptyRec = i;
                }
                if (emptyRec != 0xFF) {                                             // Not pressing 8 keys at once :)
                    keys[emptyRec].keyID = r->data[j];
                    keys[emptyRec].isDown = true;
                    keys[emptyRec].modifiers = r->data[0];
                    keys[emptyRec].repeatTime = COMClockMS() + repeatDelay;
                    lastKeyPressed = emptyRec;                
                    INPHandleKeyEvent(keys[emptyRec].keyID,keys[emptyRec].modifiers);
                }
            }
        }
    }
}

/**
 * @brief      Update timed events, implements the keyboard repeat.
 */
void INPUpdate(void) {    
    //
    //      Check for repeats.
    //
    if (keys[lastKeyPressed].isDown && COMClockMS() > keys[lastKeyPressed].repeatTime) {
            INPHandleKeyEvent(keys[lastKeyPressed].keyID,keys[lastKeyPressed].modifiers);
            keys[lastKeyPressed].repeatTime = COMClockMS() + repeatRate;
    }
}
