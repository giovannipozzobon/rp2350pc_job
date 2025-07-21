// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      setup.c
//      Purpose :   Sprites setup
//      Date :      20th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "sprites_module.h"
#include "sprites_module_local.h"

void SPRInitialise(void) {
    VMDInitialise();                                                                // Initialise graphics
    CORInitialise();                                                                // Initialise altcore
    CORAdd(SPRVerticalSyncRoutine);                                                 // Call on VSYNC
}

void SPRVerticalSyncRoutine(bool initialise) {
    static int frameCount = 0;
    if (initialise)  {                                                              // Initialise it ?    
        vi.displaySurface = vi.buffer[1];                                           // Make the display surface the second buffer.
    } else {
        static int vPos = 0;    
        vPos = (vPos+1) & 0x7F;                                                     // Cause the bar to move.
        memcpy(vi.displaySurface,vi.drawSurface,vi.bufferSize);                     // Copy the draw surface to the display
        for (int i = 0;i < 10000;i++) {                                             // and draw dot pattern on top of it (this is the moving bar)
            vi.displaySurface[random() % (320*20)+vPos*320] = random();
        }
    }
    if (frameCount++ % 100 == 0) LOG("Frames %d",frameCount);
}


