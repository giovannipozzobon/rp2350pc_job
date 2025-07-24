# Modes Module

## Required Modules

- common
- dvi

## Purpose

The purpose of the 'modes' modules is to provide a set of standardised modes built on top of the low level DVI interface, complete with double buffering and similar.

## Further Modules

Graphics modules, Sprite module.

## Usage Notes

There are three components to a mode. 

One is the generation of the horizontal line, which is the same as the DVI Module.

The second is the vertical resolution. This can be anything from 16 to 480 in steps of 16 ; the scanlines are adjusted to fit on the screen, and the display is vertically centred. So a 320x192 display would be 384 display lines (2 scan lines per line) with 48 blank lines above and below.

The third is the buffering. After initialisation the address and size of the frame buffer is provided, along with the mode. The module works out how many buffers you have on this mode.  relatively short.

There is an accessible structure "vi" which exposes certain constants. Some of these are private, and are declared with an underscore prefix and should not be used. This is document in modes_module.h

The API is very simple

- VMDInitialise() initialises everything.
- VMDSetMemory() specifies the location and size of VRAM
- VMDSetMode() specifies the mode.

## Mode Word

The mode word is a 32 bit unsigned integer.

| Bits  | Contents                                            |
| :---: | --------------------------------------------------- |
| 21-31 | Reserved (zero)                                     |
| 14-20 | Vertical Resolution (x 8)                           |
| 11-13 | Horizontal Pixel Size (000 = 1 001 = 320 011 = 640) |
| 8-10  | Pixels per byte (000= 1)                            |
|   7   | Set if colour, Clear if monochrome/greyscale        |
|   6   | Set if variable palette                             |
|  0-5  | DVI driver mode                                     |

## Double Buffering

Double buffering is possible at this point using the VSync callback in DVI and by manupulating displaySurface and drawSurface, but it has been reserved for Sprites.

## Revision

Written by Paul Robson, last revised 24 July 2025.
