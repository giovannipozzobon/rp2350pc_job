// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Text module test program.
//      Date :      26th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "text_module.h"
#include "input_module.h"

uint8_t vRAM[640*480];                                                              // May consider triple buffering.

int MAINPROGRAM(int argc,char *argv[]) {
    INPInitialise();                                                                // Initialise input
    TXTInitialise();
    VMDSetVideoMemory(vRAM,sizeof(vRAM));                                           // Set video ram and size, clear desktop.
    GFXDraw(Mode,MODE_640_480_256,0);       
    GFXDraw(Desktop,0,0);
    TXTWrite('*');
    while (COMAppRunning()) {                                                       // Our "main program"
        int16_t k = INPGetKey();                                                    // Get key, log to serial and list if F or D pressed
        if (k != 0) LOG("Key %d",k);
        COMUpdate();                                                                // Update stuff.
        YIELD();                                                                    // Yield for runtime.
    }
	return 0;
}
