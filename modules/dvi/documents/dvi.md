# DVI Interface

## Required Modules 

- Common module

## Purpose

This is the low level driver for DVI output. It's primary purpose is to set up HSTX and DMA in the Pico to create a display.

This does not provide display modes per se. It sets up the HSTX to render lines in a particular format (see options below). 

It runs on core 1. 

There is a callback function which gets the line data for each line. 

## Further Modules

- A 'mode' system that provides a mode selection interface and management of things like double buffering.

## Usage Notes

DVIInitialise() sets up the DVI system - the HSTX and DMA and sets it all going.

DVISetMode() has one parameter, which describes how the data will be rendered. These are listed in the include files.

The normal parameters are the number of pixels displayed horizontally, the colour depth supported, and whether the palette is fixed or variable.

### The Data Callback

To access the line data, a callback function is used - a very simple example of this is in main.c ; this is a function which takes a scanline from 0..479 (uint16_t) and returns the address of the line with the formatted data above, with colour data encoded, with 80 .. 640 bytes depending on that encoding.

It can also return NULL, which is for blank lines.

This function is set using the DVISetLineAccessorFunction()

With this system it is simple to change the vertical resolution. So if one wanted a 640x400 line, one could simply return NULL for all scanlines of 400 or more. If one wanted to halve the vertical resolution, halving the scanline with produce a value from 0..239 which would return the same memory address for two adjacent lines. It can also be used for hardware vertical scrolling.

# IMPORTANT 

- The data callback functions should be a short routine, like the sample. 
- Data callback functions should be be declared KEEP_IN_RAM (this is __not_in_flash_func()) otherwise they will potentially be unloaded, which will crash the display. If it loses sync it seems to stay lost.

## Revision

Written by Paul Robson, last revised 24 July 2025.







