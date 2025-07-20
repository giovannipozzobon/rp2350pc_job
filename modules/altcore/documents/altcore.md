# AltCore module

## Required Modules 

- Common module
- DVI module
- Modes module

## Purpose

The altcore module is a low level bridge to the multicore system. On a Pico, core 1 is dedicated to screen manipulation, core 0 runs all the other modules, and the video interrupt.

It allows starting of the second core and arbitrary functions can be called on Vertical Sync. The second core can be stopped manually ; it also stops if the modes change.

This along with DVI and USB has its own personal implementation in runtime ; it is not run on a second core, but called when the system yields to redraw.

## Further Modules

- Modules to do graphical effects, most obviously sprites.

## Usage Notes

Perhaps best explained by reference to the demo main/app.c

The program initialises everything, goes to 320x240x256 mode (which allows for two buffers), adds "VerticalSyncRoutine" to the handlers, draws some stuff on the screen, then goes into a standard loop where keystrokes are echoed, you can list a file or a directory, and switch mode (for testing purposes)

### VerticalSyncRoutine

Handlers are called with a boolean parameter.

If true, the handler is initialised.  In this example, the displaySurface and drawSurface, which are usually the same thing, are seperated (without error checking, naughty !)

So there are at least two buffers here. Things are drawn on the first on, but the second one is displayed.

If false, the routine is called at VerticalSync (e.g. top of screen)

The first thing it does is copy the drawSurface to the displaySurface (the memcpy), which is done by DMA on this hardware (I think).

Double buffering would normally swap them over, but I want to show overlayed 'animated graphics' on this. At this point the display is just the ellipses.

I then draw a pile of dots on the screen at a position determined by vPos which is incremented and wrapped every frame (this is why the bar moves down and loops !), which gives the moving speckly bar effect.

It's a predecessor of a sprite routine ; the difference is rather than draw sprites, we're just drawing *something* to show they work.

Experimentally there seems to be no limit on how long this takes, if you draw an insane number of pixels it still works, but it moves more slowly, as the drawing takes more than one frame to complete. 

As with anything of this type it may cause flickering once you start updating displaySurface when it's being drawn (out of Vertical Blank), but how much one can actually do remains to be seen.

## Revision

Written by Paul Robson, last revised 20 July 2025.

