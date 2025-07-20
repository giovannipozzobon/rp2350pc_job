// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   2nd Core demo program.
//      Date :      19th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "altcore_module.h"

static void ListDirectory(void);
static void ListFile(void);
static void VerticalSyncRoutine(bool initialise);
static void decorate(void);

//
//      This demo program draws some ellipses on the screen and then "animates" a speckle bar on top of it.
//      It doesn't contain any sprite functionality, it is the interface to the 2nd core hardware, this calls
//      the functions specified (in this case "VerticalSyncRoutine") on the vertical sync.
//
uint8_t vRAM[640*480];                                                              // Actually only need 320x240x2 which is half this.
                                                                                    // Must have *two* buffers for this demo to work.

int MAINPROGRAM(int argc,char *argv[]) {
    INPInitialise();                                                                // Initialise input
    GFXInitialise();                                                                // Initialise graphics
    CORInitialise();                                                                // Initialise altcore
 
    VMDSetVideoMemory(vRAM,sizeof(vRAM));                                           // Set video ram and size
    GFXDraw(Mode,MODE_320_240_256,0);                                               // Set mode. This has 2 buffers, which will be the back and front.

    LOG("%d\n",vi.bufferCount);                                                     // Show # buffers on the log.

    CORAdd(VerticalSyncRoutine);                                                    // Call this (see below) on VSYNC
    CORStart();                                                                     // Start everything up.
    decorate();                                                                     // Draw ellipses.

    while (COMAppRunning()) {                                                       // Our "main program"
        int16_t k = INPGetKey();                                                    // Get key, log to serial and list if F or D pressed
        if (k != 0) LOG("Key %d",k);
        if (toupper(k) == 'F') ListFile();
        if (toupper(k) == 'D') ListDirectory();
        if (toupper(k) == 'M') {                                                    // M switches mode so I can test that.
            GFXDraw(Mode,MODE_640_480_16,0);
            decorate();
        }
        INPUpdate();                                                                // Update Input
        USBUpdate();                                                                // Update USB (in this case keyboard messages)
        YIELD();                                                                    // Yield for runtime.
    }
}

/**
 * @brief      Draw ellipses so we've something to see.
 */
static void decorate(void) {
    for (int i = 0; i < 80;i += 2) {                                                // Draw *something* as a background :)
        GFXDraw(RawColour,rand() & 0xFF,0);                                         // It's a series of nested ellipses
        GFXDraw(Move,i,i);GFXDraw(Ellipse,319-i,239-i);
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
        LOG("Seek result %d",error);
        LOG("Tell result %d",FSTell(handle));
        char buffer[129];
        error = FSRead(handle,buffer,128);buffer[128] = '\0';
        LOG("Read %d : [%s]",error,buffer);
        error = FSClose(handle);
    }
}

/**
 * @brief      The code executed at VSYNC when sprites are enabled. This is a simple demo.
 */
static void VerticalSyncRoutine(bool initialise) {
    if (initialise)  {                                                              // Initialise it ?    
        vi.displaySurface = vi.buffer[1];                                           // Make the display surface the second buffer.
    } else {
        static int vPos = 0;    
        vPos = (vPos+1) & 0x7F;                                                     // Cause the bar to move.
        if (vPos == 0) LOG("Alive !");                                              // So we know if core 1 is running.
        memcpy(vi.displaySurface,vi.drawSurface,vi.bufferSize);                     // Copy the draw surface to the display
        for (int i = 0;i < 10000;i++) {                                             // and draw dot pattern on top of it (this is the moving bar)
            vi.displaySurface[random() % (320*20)+vPos*320] = random();
        }
    }
}
    