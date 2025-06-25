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
 * @brief      Handle USB Report
 *
 * @param[in]  type    Type (G)eneric (M)ouse (K)eyboard
 * @param[in]  vid     Vendor ID (if Generic)
 * @param[in]  pid     Product ID (if Generic)
 * @param      report  Report data
 * @param[in]  len     Length of report data
 */
void USBReportHandler(uint8_t type,uint16_t vid, uint16_t pid, uint8_t const* report, uint16_t len) {
    printf("%d %04x:%04x (%2d)",type,vid,pid,len);
    for (int i = 0;i < len;i++) printf(" %02x",report[i]);
    printf("\n");
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
