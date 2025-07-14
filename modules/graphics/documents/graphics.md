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

All interfacing is done via GFXDraw(command,x,y) which takes a command and 2 values which are usually, not always, coordinates. (see graphics_module.h)

Graphic clipping is available, and operates as a stack. When you call command PushStack it saves the current clip on an internal stack, which is restored by command PopStack. If you want to operate it purely as an update/single window then call command NoClip then PushStack as the stack has a finite size.

### Notes on Commands

- Most of the commands are self explanatory, bearing in mind the above sequencing ; so to draw a filled rectangle you Move to one corner, and FillRect to the diagonally opposite corner.
- Ellipses are drawn like rectangles, the ellipse 'fills the box'
- Character uses x for the character to draw, and y should be zero.
- Mode uses x for the mode (the same as in modes_module.h), y should be zero
- Colour uses x for foreground,y for background, the lower 16 bits of each If y is 0xFFFF then transparency is on (for characters). The upper 16 bits should be zero, these are reserved for expansion.
- Scaling sets the x and y scales of the pixel fonts. The defaults are 1,1 
- NoClip resets the whole clipping system and its internal stack, which is manipulated by PushStack and PopStack. PushStack takes a GFXCLIPRECT structure cast to a uint32_t

## Coordinate Mapping

The coordinates can be mapped from anything you like ; a function defined as void xxxx(uint32_t *x,uint32_t *y) can be set using the SetMapper command (function address in x, cast as uint32_t), and all coordinates will be passed through this.

Unlike clipping there is one mapping per screen. 

## Improvements

- More functionality
- More fonts (interface to GEOS fonts ?)

## Revision

Written by Paul Robson, last revised 14 July 2025.
