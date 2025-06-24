/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ff.h>

#include "pico/stdlib.h"

#include "bsp/board_api.h"
#include "tusb.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+
void led_blinking_task(void);
extern void hid_app_task(void);
void ListDirectory(void);

#if CFG_TUH_ENABLED && CFG_TUH_MAX3421
// API to read/rite MAX3421's register. Implemented by TinyUSB
extern uint8_t tuh_max3421_reg_read(uint8_t rhport, uint8_t reg, bool in_isr);
extern bool tuh_max3421_reg_write(uint8_t rhport, uint8_t reg, uint8_t data, bool in_isr);
#endif

bool is_filesystem_available(void);

/*------------- MAIN -------------*/
int main(void) {
    stdio_init_all();
    //sleep_ms(1000);

  printf("TinyUSB Host MSC HID Example\r\n");
  board_init();

  // init host stack on configured roothub port
  tuh_init(BOARD_TUH_RHPORT);

  if (board_init_after_tusb) {
    board_init_after_tusb();
  }

#if CFG_TUH_ENABLED && CFG_TUH_MAX3421
  // FeatherWing MAX3421E use MAX3421E's GPIO0 for VBUS enable
  enum { IOPINS1_ADDR  = 20u << 3, /* 0xA0 */ };
  tuh_max3421_reg_write(BOARD_TUH_RHPORT, IOPINS1_ADDR, 0x01, false);
#endif

  while (!is_filesystem_available()) {
    // tinyusb host task
    tuh_task();

    led_blinking_task();
    hid_app_task();
  }

  printf("FS Available.\n");
  ListDirectory();
  
  while (1) {
    tuh_task();
    led_blinking_task();
    hid_app_task();
  }

}

void ListDirectory(void)
 {
    DIR dir;
    char *path;
    UINT BytesWritten;
    char string[128];
    FRESULT res;
 
    path = "/"; // where you want to list
 
    res = f_opendir(&dir, path);
 
#ifdef DBG
    if (res != FR_OK)
      printf("res = %d f_opendir\n", res);
#endif
 
    if (res == FR_OK)
    {
      while(1)
      {
        FILINFO fno;
 
        res = f_readdir(&dir, &fno);
 
#ifdef DBG
        if (res != FR_OK)
          printf("res = %d f_readdir\n", res);
#endif
 
        if ((res != FR_OK) || (fno.fname[0] == 0))
          break;
 
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

//--------------------------------------------------------------------+
// TinyUSB Callbacks
//--------------------------------------------------------------------+

void tuh_mount_cb(uint8_t dev_addr) {
  // application set-up
  printf("A device with address %d is mounted\r\n", dev_addr);
}

void tuh_umount_cb(uint8_t dev_addr) {
  // application tear-down
  printf("A device with address %d is unmounted \r\n", dev_addr);
}


//--------------------------------------------------------------------+
// Blinking Task
//--------------------------------------------------------------------+
void led_blinking_task(void) {
  const uint32_t interval_ms = 200;
  static uint32_t start_ms = 0;

  static bool led_state = false;

  // Blink every interval ms
  if (board_millis() - start_ms < interval_ms) return; // not enough time
  start_ms += interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}
