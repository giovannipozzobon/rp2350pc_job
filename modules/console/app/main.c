// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Console module test program.
//      Date :      14th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "console_module.h"
#include "console_module_local.h"
#include "input_module.h"

uint8_t vRAM[640*480];

void DrawingApplication(void);

/**
 * @brief      Console Main program
 *
 * @param[in]  argc  Argument count
 * @param      argv  Arguments
 *
 * @return     Error code or 0.
 */
int MAINPROGRAM(int argc,char *argv[]) {
    INPInitialise();
    CONInitialise();

    VMDSetVideoMemory(vRAM,sizeof(vRAM));                                           // Set video ram and size
    GFXDraw(Mode,MODE_640_480_256,0);                                               // Set mode.
    GFXDraw(Desktop,0,0);                                                           // Fill desktop background

    GFXDraw(Colour,0xFFF,0);                                                        // Draw frame
    GFXDraw(Move,4*8-1,5*8-1);GFXDraw(Rect,40*8,32*8);

    CONSetWindow(4,5,40,32);                                                        // Set window in units of 8 pixels (why all the frame draws are *8)

    while (COMAppRunning()) { 
        int16_t k = INPGetKey();                                                    // Keep sending keys to the console
        if (k != 0) {
            CONWrite(k);
        }
        USBUpdate();                                                                // Update USB (in this case keyboard messages)
        INPUpdate();                                                                // Update INP (things like autorepeat)
        YIELD();                         
    }
}


