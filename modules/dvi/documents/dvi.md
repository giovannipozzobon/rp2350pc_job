# DVI Interface

## Required Modules 

- Common module

## Purpose

This is the low level driver for DVI output. It's primary purpose is to set up HSTX and DMA in the Pico to create a display.

This does not provide display modes per se. It sets up the HSTX to render lines in a particular format (see options below).

There is a callback function which gets the line data for each line.

## Further Modules

- A 'mode' system that provides a mode selection interface and management of things like double buffering.

## Usage Notes

DVIInitialise() sets up the DVI system - the HSTX and DMA and sets it all going.

DVISetMode() has one parameter, which describes how the data will be rendered. By default all render in 640 pixels across (the vertical resolution can be 0 - 480 and is software driven, see the data callback section.

| Bit(s) | Purpose                                                      |
| :----: | ------------------------------------------------------------ |
|   15   | When set, this forces an 8 bit DMA transfer rather than a 32 bit DMA transfer. This means that each byte in the display line is rendered four times. When set on its own, this changes the resolution to 160 (as each byte is repeated 4 times). For the bytes/line value below, this divides all of those by four, saving memory. |
|   14   |                                                              |
|  13-4  | Reserved, should be zero.                                    |
|  0-3   | Specifies the pixels per byte, as described below. These values can be 1 , 2, 4 or 8 |



### Colour Rendering Modes

|    Type    | Colour /GreyscaleDepth | Bytes/Line | Pixels/Byte |
| :--------: | :--------------------: | :--------: | :---------: |
|   Colour   |     256 (RRRGGGBB)     |    640     |      1      |
|   Colour   |       16 (RGGB)        |    320     |      2      |
| Greyscale  |        4 levels        |    160     |      4      |
| Monochrome |       Monochrome       |     80     |      8      |

Note the pixels are stored backwards, so in the 2 pixels per byte mode, the left most of the 2 pixels is stored in the lower nibble of the byte, which is slightly counter-intuitive. In an 8 pixels per byte mode, similarly, the left most pixel of the 8 is stored in the least significant bit.

### The Data Callback

To access the line data, a callback function is used - a very simple example of this is in main.c ; this is a function which takes a scanline from 0..479 (uint16_t) and returns the address of the line with the formatted data above, with colour data encoded, with 80 .. 640 bytes depending on that encoding.

It can also return NULL, which is for blank lines.

This function is set using the DVISetLineAccessorFunction()

With this system it is simple to change the vertical resolution. So if one wanted a 640x400 line, one could simply return NULL for all scanlines of 400 or more. If one wanted to halve the vertical resolution, halving the scanline with produce a value from 0..239 which would return the same memory address for two adjacent lines. It can also be used for hardware vertical scrolling.

### IMPORTANT 

This function should be a short routine, like the sample. Do not use this function to build up a buffer from other data ; the function is called from an interrupt at the end of a scan line, so if it doesn't return PDQ the display will not work.

If you want to build up scanlines it should be possible by doing it on one of the main cores and using this routine to signal "do the next one" - e.g. it is built up outside the IRQ and accessed from within it. 

## Revision

Written by Paul Robson, last revised 30 June 2025.







