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

#include "usb_manager.h"

#include "pico/stdlib.h"
#include "bsp/board_api.h"
#include "ff.h"

static void LedBlinkingTask(void);
static void ListDirectory(void);

/**
 * @brief      Handle USB Report
 *
 * @param      r     USB Report
 *
 * @return     false, as the report is not consumed.
 */
bool _ReportHandler(USBREPORT *r) {
    printf("%d %04x:%04x (%2d)",r->type,r->vid,r->pid,r->length);
    for (int i = 0;i < r->length;i++) printf(" %02x",r->data[i]);
    printf("\n");
    return false;
}

/**
 * @brief      Example main program
 *
 * @return     Error code
 */
int main(void) {
    stdio_init_all();
    printf("TinyUSB Host MSC HID Example\r\n");

    USBInitialise(true);                                                            // Set up, and wait for the USB Key
    USBInstallHandler(_ReportHandler);                                              // Add a handler for USB HID reports.

    printf("USBKey now available.\n");
    
    ListDirectory();  
    while (1) {
        USBUpdate();
        LedBlinkingTask();
    }
}



/**
 * @brief      List the root directory
 */
static void ListDirectory(void) {
    DIR dir;
    char *path;
    UINT BytesWritten;
    char string[128];
    FRESULT res; 
    path = "/"; 

    res = f_opendir(&dir, path); 
    if (res != FR_OK) printf("res = %d f_opendir\n", res);
 
    if (res == FR_OK) {
        while(1) {
            FILINFO fno;
            res = f_readdir(&dir, &fno);
            if (res != FR_OK) printf("res = %d f_readdir\n", res); 
            if ((res != FR_OK) || (fno.fname[0] == 0)) break;
            sprintf(string, "%c%c%c%c %10d %s/%s",
                            ((fno.fattrib & AM_DIR) ? 'D' : '-'),
                            ((fno.fattrib & AM_RDO) ? 'R' : '-'),
                            ((fno.fattrib & AM_SYS) ? 'S' : '-'),
                            ((fno.fattrib & AM_HID) ? 'H' : '-'),
                            (int)fno.fsize, path, fno.fname);
            puts(string);
        }
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
