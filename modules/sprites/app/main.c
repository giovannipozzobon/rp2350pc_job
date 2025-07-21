// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Sprites demo program.
//      Date :      19th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "sprites_module.h"
#include "sprites_module_local.h"
#include "dvi_module.h"
static void ListDirectory(void);
static void ListFile(void);
static void VerticalSyncRoutine(bool initialise);
static void decorate(void);

uint8_t vRAM[320*240*2];                                                            // Actually only need 320x240x2 which is half this.
                                                                                    // Must have *two* buffers for this demo to work.


int MAINPROGRAM(int argc,char *argv[]) {
    INPInitialise();                                                                // Initialise input
    GFXInitialise();
    VMDSetVideoMemory(vRAM,sizeof(vRAM));                                           // Set video ram and size
    SPRInitialise();

    GFXDraw(Mode,MODE_320_240_256,0);                                               // Set mode. This has 2 buffers, which will be the back and front.
    decorate();                                                                     // Draw ellipses.

    CORStart();                                                                     // Start everything up.

    while (COMAppRunning()) {                                                       // Our "main program"
        int16_t k = INPGetKey();                                                    // Get key, log to serial and list if F or D pressed
        if (k != 0) LOG("Key %d",k);
        if (toupper(k) == 'F') ListFile();
        if (toupper(k) == 'D') ListDirectory();
        if (toupper(k) == 'M') {                                                    // M switches mode so I can test that.
            GFXDraw(Mode,MODE_160_240_256,0);
            decorate();
        }
        if (toupper(k) == 'S') DVISetupDMA();
        INPUpdate();                                                                // Update Input
        USBUpdate();                                                                // Update USB (in this case keyboard messages)
        YIELD();                                                                    // Yield for runtime.
    }
}

/**
 * @brief      Draw rectangles so we've something to see.
 */
static void decorate(void) {
    for (int i = 0; i < 120;i += 2) {                                               // Draw *something* as a background :)
        GFXDraw(RawColour,rand() & 0xFF,0);                                      
        GFXDraw(Move,i,i);GFXDraw(Rect,319-i,239-i);
    }
}

/**
 * @brief      List the root directory
 */
static void ListDirectory(void) {
    char *path = ""; 
    int32_t error,handle = FSOpenDirectory(path);
    if (handle >= 0) {
        FSOBJECTINFO fInfo;
        while (error = FSReadDirectory(handle,&fInfo),error == 0) {
            LOG("%c %-8d %s",fInfo.isDirectory ? 'D':'.',fInfo.size,fInfo.name);
        }
        if (error != FSERR_EOF) LOG("Read error : %d",error);
        FSCloseDirectory(handle);        
    }
}

/**
 * @brief      List part of a file on the USB key.
 */
static void ListFile(void) {
    int32_t error,handle = FSOpen("loops.bsc");
    if (handle == 0) {
        error = FSSeek(handle,12);
        char buffer[129];
        error = FSRead(handle,buffer,128);buffer[128] = '\0';
        LOG("Read %d : [%s]",error,buffer);
        error = FSClose(handle);
    }
}

