#include "sprites_module.h"

void demo();

uint8_t vRAM[640*480];

int MAINPROGRAM(int argc,char *argv[]) {
    INPInitialise();
    GFXInitialise();
    VMDSetVideoMemory(vRAM,sizeof(vRAM));                                           // Set video ram and size
    GFXDraw(Mode,MODE_640_240_16,0);                                               // Set mode. This has 2 buffers, which will be the back and front.
    LOG("%d\n",vi.bufferCount);

    for (int i = 0; i < 80;i += 2) {                                                // Draw *something* as a background :)
        GFXDraw(RawColour,rand() & 0xFF,0);
        GFXDraw(Move,i,i);GFXDraw(Ellipse,319-i,239-i);
    }
    multicore_launch_core1(demo);

    while (COMAppRunning()) { 
        int16_t k = INPGetKey();  
        if (k != 0) LOG("Key %d",k);

        INPUpdate();
        USBUpdate();                                                                // Update USB (in this case keyboard messages)
        YIELD();                         
    }
}

void demo() {
    verticalSyncOccurred = false;                                                   // Forces wait one frame.
    while (COMAppRunning()) {
        if (verticalSyncOccurred) {
            for (int i = 0;i < 100000*100;i++) {
                vRAM[random() % (320*20)] = random();
            }
            verticalSyncOccurred = false;
        }
    }
}

