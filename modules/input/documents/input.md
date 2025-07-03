# Input Module

## Required Modules 

- common
- usb

## Purpose

This provides a cleaner and simpler input to the HID devices plugged into the board (Keyboard, Mouse and Gamepad)

## Further Modules

None planned.

## Usage Notes

INPUpdate() needs to be called at regular intervals. This implements the keyboard repeat.

bool *INPGetKeyboardState() returns an array of booleans of size 256, one for each USB HID Keyboard code, which is true if that key is currently pressed.


## Revision

Written by Paul Robson, last revised 3 July 2025.







