// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Graphics module test program.
//      Date :      9th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

uint8_t vRAM[640*480];

int MAINPROGRAM() {

    VMDInitialise(); 
    VMDSetVideoMemory(vRAM,sizeof(vRAM));                                   // Set video ram and size

    GFXDraw(Mode,MODE_640_480_256,0);                                       // Set mode.

    int speckle = 640*480;
    speckle = 320;
    for (int x = 0;x < 160;x++) {
        for (int y = 2;y < 9;y++) {
            //vRAM[x+y*vi.bytesPerLine] = 0x11*(x / 10);
        }
    }

    draw.xLeft = 20;draw.yTop = 30;draw.xRight = 610;draw.yBottom = 440;

    static uint32_t commands[] = { Plot, Line, Rect, FillRect, Ellipse, FillEllipse, Triangle, FillTriangle,Character };
    uint32_t count = 0;

    while (COMAppRunning()) {                                                                     
        count++;
        vi.drawSurface[random()%speckle] = (random() & 1) ? 0:random();     // Knows we aren't crashing. May consider flashing LED :)
        GFXDraw(Colour,random() & 0xFF,0);
        uint32_t command;

        command = COMClockMS()/1000;        // This line does 1s for each draw type
        // command = count / 1000;          // This line shows how long it takes to draw 1000 of each, few secs for filled shapes.

        command = commands[command % 9];
        if (command != Character) {
            GFXDraw(command,random() % 640,random() % 480);
        } else {
            GFXDraw(Move,random() % 640,random() % 480);
            GFXDraw(Scaling,random()%4+1,random()%4+1);
            GFXDraw(Character,random()%96+32,0);
        }
        YIELD();                         
    }
    return 0;
}

