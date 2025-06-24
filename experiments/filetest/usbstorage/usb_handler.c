// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      usb_handler.c
//      Purpose :   USB Interfaces
//      Date :      24th June 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#define LOCALS
#include "usb_manager.h"

static bool isInitialised = false;

/**
 * @brief      Initialise the USB system.
 */
void USBInitialise(void) {

    if (isInitialised) return;                                                      // Can only do this once.
    isInitialised = true;

    board_init();                                                                   // Most of this code comes from the tinyUSB examples
    tuh_init(BOARD_TUH_RHPORT);                                                     // init host stack on configured roothub port
    //sleep_ms(1000);                                                               // Doesn't seem to be needed any more.

    if (board_init_after_tusb) {                                                    // Your guess is as good as mine ... if it ain't broke ...
        board_init_after_tusb();
    }

    while (!USBIsFileSystemAvailable()) {                                           // Wait for USB Key. Could have a timeout here.
        USBUpdate();    
    }
}

/**
 * @brief      Update the USB System and any associated code.
 */
void USBUpdate(void) {
    tuh_task();
    USBHIDAppTask();
}


/**
 * @brief      Mount callback
 *
 * @param[in]  dev_addr  The device address
 */
void tuh_mount_cb(uint8_t dev_addr) {
    printf("A device with address %d is mounted\r\n", dev_addr);
}

/**
 * @brief      Unmount callback
 *
 * @param[in]  dev_addr  The device address
 */
void tuh_umount_cb(uint8_t dev_addr) {
    printf("A device with address %d is unmounted \r\n", dev_addr);
}
