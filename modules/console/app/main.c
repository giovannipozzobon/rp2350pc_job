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

    GFXDraw(Colour,0x0F0,0);
    GFXDraw(Move,4*8-1,5*8-1);GFXDraw(Rect,20*8,12*8);
    CONSetWindow(4,5,20,12);
    for (int c = 0;c < 104;c++) CONWrite(c%96+32);
    
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
