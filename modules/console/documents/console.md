# Console Module

## Required Modules

- DVI
- Modes
- Graphics

## Purpose

This module provides a simple text console that can be written to. It is currently a singleton, but could be extended to multiple consoles.

## Further Modules

- ScreenEditor (maybe)

## Usage Notes

The example application shows how it works. It has the usb and input modules included so it can be typed into, which is not a requirement. It sets the mode and writes a blank desktop, and then draws a frame.

CONSetWindow(x1,y1,x2,y2) takes 2 coordinate pairs which are 8 pixel cells, not absolute pixels, and these define the window the console operates in. The colours can be defined before using CONSetColour(ink,paper,cursor)

CONWrite(c) sends character to the console window. Apart from the standard character set 32-127, it also understands backspace (8) carriage return/line feed (10).

It does not provide screen editor functionality, merely a simple serial console. It is relatively straightforward to modify them demo app to store the characters in a buffer as you go.

## Improvements

It's actually been simplified, originally it did allow moving of the cursor around and similar, but I decided this was probably overkill. Creating a screen editor along the lines of Commodore microcomputers would be perfectly feasible, but it's really designed for just typing commands in. I would ideally like BASIC to be a proper editor, so the line numbers can almost vanish entirely (they will probably exist for backwards compatibility)

## Revision

Written by Paul Robson, last revised 16 July 2025.

