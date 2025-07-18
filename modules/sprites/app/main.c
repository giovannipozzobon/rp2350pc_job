#include "sprites_module.h"

uint8_t vRAM[320*240*2];

int MAINPROGRAM(int argc,char *argv[]) {
    USBInitialise();
    GFXInitialise();

    VMDSetVideoMemory(vRAM,sizeof(vRAM));                                           // Set video ram and size
    GFXDraw(Mode,MODE_320_240_256,0);                                               // Set mode.
    GFXDraw(Desktop,0,0);                                                           // Fill desktop background

    GFXDraw(Colour,0xFFF,0);                                                        // Draw frame
    GFXDraw(Move,4*8-1,5*8-1);GFXDraw(Rect,40*8,32*8);

    LOG("%d\n",vi.bufferCount);
    while (COMAppRunning()) { 
        USBUpdate();                                                                // Update USB (in this case keyboard messages)
        YIELD();                         
    }
}

