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

/**
 * @brief      A simple demo mapper. All it does is halve all the coordinates,
 *             shifting the drawing into the upper left quarter of the screen.
 *             The clipping rect still works, but only top/left sides because
 *             that's still on the physical system.
 *
 * @param      x     ptr to x to map
 * @param      y     ptr to y to map.
 */
void _DemoMapper(uint32_t *x,uint32_t *y) {
    *x = (*x) >> 1;
    *y = (*y) >> 1;
}

/**
 * @brief      Main program
 *
 * @return     0 if no error.
 */
int MAINPROGRAM() {

    VMDInitialise(); 
    VMDSetVideoMemory(vRAM,sizeof(vRAM));                                           // Set video ram and size

    GFXDraw(Mode,MODE_640_480_256,0);                                               // Set mode.

    int speckle = 640*480;
    speckle = 320;

    GFXDraw(Desktop,0,0);                                                           // Clear to blue

    GFXCLIPRECT clip;                                                               // Set drawing clip
    clip.xLeft = 20;clip.yTop = 30;clip.xRight = 610;clip.yBottom = 440;
    GFXDraw(PushClip,(GFXDRAWPARAM)&clip,0);

    uint32_t size = GFXDraw(CharExtent,'W',0);
    LOG("%x\n",size);

    //
    //      To see the mapper working uncomment this.
    //
    // GFXDraw(SetMapper,(GFXDRAWPARAM)_DemoMapper,0);                               // Set coordinate mapper.


    //
    //      Demo code. Cycles through lots of drawing with the various drawing commands.
    //
    static uint32_t commands[] = { Plot, Line, Rect, FillRect, Ellipse, FillEllipse, Triangle, FillTriangle,Character };
    uint32_t count = 0;

    while (COMAppRunning()) {                                                                     
        count++;
        vi.drawSurface[random()%speckle] = (random() & 1) ? 0:random();             // Knows we aren't crashing. May consider flashing LED :)
        GFXDraw(Colour,random() & 0xFF,0);
        uint32_t command;

        command = COMTimeMS()/1000;         // This line does 1s for each draw type
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

