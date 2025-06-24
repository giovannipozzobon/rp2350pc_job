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

#define LOCALS
#include "usb_manager.h"

static void LedBlinkingTask(void);
static void ListDirectory(void);

/**
 * @brief      Example main program
 *
 * @return     Error code
 */
int main(void) {
    stdio_init_all();
    printf("TinyUSB Host MSC HID Example\r\n");
    USBInitialise();
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
