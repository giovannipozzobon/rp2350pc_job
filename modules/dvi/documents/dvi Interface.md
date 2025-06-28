# DVI Interface

## Required Modules 

- Common module

## Purpose

This is the low level driver for DVI output. It's primary purpose is to set up HSTX and DMA in the Pico to create a display.

The low level display is defined by 2 values. 

- The "pixels per byte" value, which is currently, 8, 4 , 2 and 1 - a mixture of colour displays and greyscale displays.
- The display width in pixels. This currently only supports 640 pixels, but I hope to support 320 pixels when I can figure out how/if the HSTX can do this.

There is a callback function which gets the line data for each line.

## Further Modules

- A 'mode' system that provides a mode selection interface and management of things like double buffering.

## Usage Notes

DVIInitialise() sets up the DVI system - the HSTX and DMA and sets it all going.

DVISetup() has two parameters 

- The first one is the format (see table below), which is a choice between more colours or less memory. The hardware is perfectly capable of doing 640x480x256 colours, but this will cost over half the SRAM in the RP2350.
- The second is the horizontal pixel render with, which is currently always 640.

|    Type    | Colour /GreyscaleDepth | Bytes/Line | Pixels/Byte |
| :--------: | :--------------------: | :--------: | :---------: |
|   Colour   |     256 (RRRGGGBB)     |    640     |      1      |
|   Colour   |       16 (RGGB)        |    320     |      2      |
| Greyscale  |        4 levels        |    160     |      4      |
| Monochrome |       Monochrome       |     80     |      8      |

Note that in the two intermediate values the pixels are interlaced e.g.

|    Type     | Format |  7   |  6   |  5   |  4   |  3   |  2   |  1   |  0   |
| :---------: | :----: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: |
|  16 Colour  |  RGGB  |  R1  |  R0  | GH1  | GH0  | GL1  | GL0  |  B1  |  B0  |
| 4 Greyscale |   LD   |  H3  |  H2  |  H1  |  H0  |  L3  |  L2  |  L1  |  L0  |

H is high, L is low. 

### The Data Callback

To access the line data, a callback function is used - a very simple example of this is in main.c ; this is a function which takes a scanline from 0..479 (uint16_t) and returns the address of the line with the formatted data above, with colour data encoded, with 80 .. 640 bytes depending on that encoding.

It can also return NULL, which is for blank lines.

This function is set using the DVISetLineAccessorFunction()

With this system it is simple to change the vertical resolution. So if one wanted a 640x400 line, one could simply return NULL for all scanlines of 400 or more. If one wanted to halve the vertical resolution, halving the scanline with produce a value from 0..239 which would return the same memory address for two adjacent lines. It can also be used for hardware vertical scrolling.

### IMPORTANT 

This function should be a short routine, like the sample. Do not use this function to build up a buffer from other data ; the function is called from an interrupt at the end of a scan line, so if it doesn't return PDQ the display will not work.

If you want to build up scanlines it should be possible by doing it on one of the main cores and using this routine to signal "do the next one" - e.g. it is built up outside the IRQ and accessed from within it. 

## Revision

Written by Paul Robson, last revised 28 June 2025.







