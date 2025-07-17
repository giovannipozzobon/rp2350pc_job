# TODO list

- finish builder.py
- add DVI callback on DVI on core 1. 
- create sprite with double buffer doing something very simple.
## Console
Demo DVI callback for multicore. These can be called by YIELD and in the second core.

## Input
Implement function keys.

## Audio ?

## Sprite module
- Initially requires 2 buffers, drawBuffer copied to displayBuffer and rendered on top.

## Others
- python script with skeleton for runtimes CMakeLists.txt
- builder to allow libraries that aren't in the dependency list (e.g. input and usb in console, for testing keyboard.)
