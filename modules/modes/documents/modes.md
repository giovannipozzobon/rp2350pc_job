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

One is the generation of the horizontal line, which is the same as the DVI Module ; currently there is 640 pixels either as 1 byte per pixel (RRRGGGBB) 2 bytes per pixel(RGGB) 4 byte per pixel grey scale and 8 pixels per byte monochrome.

The second is the vertical resolution. This can be anything from 16 to 480 in steps of 16 ; the scanlines are adjusted to fit on the screen, and the display is vertically centred. So a 320x192 display would be 384 display lines (2 scan lines per line) with 48 blank lines above and below.

The third is the buffering. After initialisation the address and size of the frame buffer is provided, along with the mode. The module works out how many buffers you have on this mode.  relatively short.

There is an accessible structure "vi" which exposes certain constants. Some of these are private, and are declared with an underscore prefix and should not be used. This is document in modes_module.h

The API is very simple

- VMDInitialise() initialises everything.
- VMDSetMemory() specifies the location and size of VRAM
- VMDSetMode() specifies the mode.

## Mode Word

The mode word is a 32 bit unsigned integer.

| Bits  | Contents                                        |
| :---: | ----------------------------------------------- |
| 16-31 | Reserved (zero)                                 |
| 9-15  | Vertical Resolution (x 8)                       |
|   8   | Set if colour, Clear if monochrome/greyscale    |
|  4-7  | Pixels per Byte (1,2,4 or 8)                    |
|  0-3  | Horizontal Resolution 0 = 160, 1 = 320, 3 = 640 |

## Double Buffering

Double buffering is possible at this point using the VSync callback in DVI and by manupulating displaySurface and drawSurface, but it has been reserved for Sprites.

## Revision

Written by Paul Robson, last revised 9 July 2025.
