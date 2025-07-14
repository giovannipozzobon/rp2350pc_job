# Graphics Module

## Required Modules

- DVI
- Modes

## Purpose

This module provides basic graphics functionality ; lines, shapes text and so on.

## Further Modules

- Console
- Sprites

## Usage Notes

The graphics functionality works in physical screen pixels by default, so coordinates are in pixel units from the top left (0,0) to the bottom right (159,319,639 , 240,480 or whatever). 

All graphics operations take place within a window which is by default the whole screen.

Graphics commands work on a sequence of coordinates, as previous commands are tracked. So, for example, to draw a filled triangle, you would Move to one point, move to a second point, and draw to the third point. If you follow this with another triangle draw command, it will use the second, third and the newest point.  This is somewhat similar to the system used on Acorn's BBC Micro.

The exception to this is the character plotter, which sets the current graphics position to the next text character along, so if you send a sequence of them you will get a straight line of text.

All interfacing is done via GFXDraw(command,x,y) which takes a command and 2 values which are 32 bit integer, and GFXDrawP(command,p,y) which is the same *but* p is of type void pointer. 

Graphic clipping is available, and is set by sending the command "SetClip" together with either NULL or the address of a GFXCLIPRECT case to a GFXDRAWPARAM.

### Notes on Commands

- Most of the commands are self explanatory, bearing in mind the above sequencing ; so to draw a filled rectangle you Move to one corner, and FillRect to the diagonally opposite corner.
- Ellipses are drawn like rectangles, the ellipse 'fills the box'
- Character uses x for the character to draw, and y should be zero.
- Mode uses x for the mode (the same as in modes_module.h), y should be zero
- RawColour and Colour use x for foreground,y for background, the lower 16 bits of each If y is 0xFFFF then transparency is on (for characters). The upper 16 bits should be zero, these are reserved for expansion. RawColour is the internal format, Colour takes a 12 bit RGB (e.g. 0000rrrrggggbbbb) value and converts it to the best available colour for the mode.
- Scaling sets the x and y scales of the pixel fonts. The defaults are 1,1 

## Coordinate Mapping

The coordinates can be mapped from anything you like ; a function defined as void xxxx(uint32_t *x,uint32_t *y) can be set using the SetMapper command, which will use GFXDrawP as you are passing a pointer.

## Improvements

- More functionality
- More fonts (interface to GEOS fonts ?)

## Revision

Written by Paul Robson, last revised 14 July 2025.
