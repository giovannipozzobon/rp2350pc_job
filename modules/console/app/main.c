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


    GFXDraw(Move,100-1,100-1);GFXDraw(Rect,300+1,400+1);

    CONSetWindow(100,100,300,400);
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
