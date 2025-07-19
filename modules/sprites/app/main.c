// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Sprite demo program.
//      Date :      19th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "sprites_module.h"

static void ListDirectory(void);
static void ListFile(void);

uint8_t vRAM[640*480];

int MAINPROGRAM(int argc,char *argv[]) {
    GFXInitialise();
    VMDSetVideoMemory(vRAM,sizeof(vRAM));                                           // Set video ram and size
    GFXDraw(Mode,MODE_640_240_16,0);                                               // Set mode. This has 2 buffers, which will be the back and front.
    LOG("%d\n",vi.bufferCount);
    INPInitialise();

    for (int i = 0; i < 80;i += 2) {                                                // Draw *something* as a background :)
        GFXDraw(RawColour,rand() & 0xFF,0);
        GFXDraw(Move,i,i);GFXDraw(Ellipse,319-i,239-i);
    }

    SPRStart();

    while (COMAppRunning()) { 
        int16_t k = INPGetKey();  
        if (k != 0) LOG("Key %d",k);
        if (toupper(k) == 'F') ListFile();
        if (toupper(k) == 'D') ListDirectory();
        INPUpdate();
        USBUpdate();                                                                // Update USB (in this case keyboard messages)
        YIELD();                         
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
        //LOG("Seek result %d",error);
        //LOG("Tell result %d",FSTell(handle));
        char buffer[129];
        error = FSRead(handle,buffer,128);buffer[128] = '\0';
        LOG("Read %d : [%s]",error,buffer);
        error = FSClose(handle);
    }
}