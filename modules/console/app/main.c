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
    CONInitialise(); 
    VMDSetVideoMemory(vRAM,sizeof(vRAM));                                           // Set video ram and size
    GFXDraw(Mode,MODE_640_480_256,0);                                               // Set mode.
    GFXDraw(Desktop,0,0);

    GFXCLIPRECT clip;
    clip.xLeft = 100;clip.xRight = 300;clip.yTop = 100;clip.yBottom = 400;
    GFXDraw(SetClip,(GFXDRAWPARAM)&clip,0);
    GFXDraw(Colour,0xFF0,0x00F);
    GFXDraw(ClearWindow,0,0);

    while (COMAppRunning()) {                                                                     
        GFXDraw(Colour,random() & 0xFF,0);
        GFXDraw(Line,random() % 640,random() % 480);
        YIELD();                         
    }
	return 0;
}
