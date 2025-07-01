// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Test program for USB Interface
//      Date :      24th June 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "common_module.h"
#include "usb_module.h"

#include "pico/stdlib.h"
#include "bsp/board_api.h"

static void LedBlinkingTask(void);
static void ListDirectory(void);
static void ListFile(void);

/**
 * @brief      Handle USB Report
 *
 * @param      r     USB Report
 *
 * @return     false, as the report is not consumed.
 */
bool _ReportHandler(USBREPORT *r) {
    char buffer[128];
    sprintf(buffer,"%d %04x:%04x (%2d)",r->type,r->vid,r->pid,r->length);
    for (int i = 0;i < r->length;i++) sprintf(buffer+strlen(buffer)," %02x",r->data[i]);
    LOG(buffer);
    return false;
}

/**
 * @brief      Example main program
 *
 * @return     Error code
 */
int main(void) {
    stdio_init_all();
    LOG("TinyUSB Host MSC HID Example");

    USBInitialise(true);                                                            // Set up, and wait for the USB Key
    USBInstallHandler(_ReportHandler);                                              // Add a handler for USB HID reports.
    ListDirectory();                                                                // List the directory
    ListFile();                                                                     // List the file.
    while (1) {                                                                     // Run USB dumping USB reports as raw data
        USBUpdate();
        LedBlinkingTask();
    }
}


/**
 * @brief      List the root directory
 */
static void ListDirectory(void) {
    char *path = "/"; 
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
        char buffer[129];
        error = FSRead(handle,buffer,128);buffer[128] = '\0';
        LOG("Read %d : [%s]",error,buffer);
        error = FSClose(handle);
    }
}

/**
 * @brief      LED Blinking task
 */
static void LedBlinkingTask(void) {
    const uint32_t interval_ms = 200;
    static uint32_t start_ms = 0;
    static bool led_state = false;
    if (board_millis() - start_ms < interval_ms) return; 
    start_ms += interval_ms;
    board_led_write(led_state);
    led_state = 1 - led_state; // toggle
}
