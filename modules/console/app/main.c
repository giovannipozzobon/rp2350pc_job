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
    GFXDraw(Desktop,0,0);

    GFXCLIPRECT clip;
    clip.xLeft = 100;clip.xRight = 300;clip.yTop = 100;clip.yBottom = 400;

    GFXDraw(Move,clip.xLeft-1,clip.yTop-1);
    GFXDraw(Rect,clip.xRight+1,clip.yBottom+1);

    CONSetWindow(&clip);
    CONWrite(42);
    
    while (COMAppRunning()) { 
        int16_t k = INPGetKey();
        if (k != 0) {
            CONWrite(k);
        }     
        USBUpdate();                                                               
        INPUpdate();
        YIELD();                         
    }
	return 0;
}
