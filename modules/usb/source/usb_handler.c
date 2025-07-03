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
#include "usb_module.h"

static USBHANDLERFUNCTION usbReportHandlers[USBHANDLERCOUNT];

/**
 * @brief      Initialise the USB system.
 *
 * @param[in]  waitForFS  If true, wait for the USB Key to stabilise. If false,
 */
void USBInitialise(bool waitForFS) {

    static bool isInitialised = false;                                              // Only initialise once.
    if (isInitialised) return;
    isInitialised = true;

    COMInitialise();                                                                // Initialise common code.

    FSInitialise();                                                                 // Initialise the file wrapper (not a module, part of USB)
    for (int i = 0;i < USBHANDLERCOUNT;i++) usbReportHandlers[i] = NULL;            // Remove all report handlers
    board_init();                                                                   // Most of this code comes from the tinyUSB examples
    tuh_init(BOARD_TUH_RHPORT);                                                     // init host stack on configured roothub port
    //sleep_ms(1000);                                                               // Doesn't seem to be needed any more.

    if (board_init_after_tusb) {                                                    // Your guess is as good as mine ... if it ain't broke ...
        board_init_after_tusb();
    }

    if (waitForFS) {
        while (!USBIsFileSystemAvailable()) {                                       // Wait for USB Key. Could have a timeout here.
            USBUpdate();    
        }
    }
}

/**
 * @brief      Install a USB Report Handler
 *
 * @param[in]  handler  The handler function
 *
 * @return     true if successfully installed. 
 */
bool USBInstallHandler(USBHANDLERFUNCTION handler) {
    for (int i = 0;i < USBHANDLERCOUNT;i++) {
        if (usbReportHandlers[i] == NULL) {
            usbReportHandlers[i] = handler;
            return true;
        }
    }
    return false;
}
/**
 * @brief      Dispatch a report to handler or handlers.
 *
 * @param[in]  type    Report type 'K' 'M' 'G'
 * @param[in]  vid     Vendor ID
 * @param[in]  pid     Product ID
 * @param      report  Data in report
 * @param[in]  len     Length of the report in bytes.
 */
void USBDispatchReport(uint8_t type,uint16_t vid, uint16_t pid, uint8_t *report, uint16_t len) {
    bool consumed = false;
    USBREPORT r;                                                                    // Create a report record
    r.type = type;r.vid = vid;r.pid = pid;r.data = report;r.length = len; 
    for (int i = 0;i < USBHANDLERCOUNT;i++) {
        if (!consumed && usbReportHandlers[i] != NULL) {                            // If not consumed, and handler found.
            consumed = (*usbReportHandlers[i])(&r);                                 // Call the handler, and do no more if the handler consumes it.
        }
    }
}

/**
 * @brief      Update the USB System and any associated code.
 */
void USBUpdate(void) {
    static int nextUpdate = 0;
    if (COMClockMS() > nextUpdate) {                                                // Limits USB update rate to 25Hz, which is fast enough to update
        nextUpdate = COMClockMS()+40;                                               // the mouse. Doing it flat out crashes the DVI Driver.
        tuh_task();                                                                 // I think it's a reentrancy problem.
        USBHIDAppTask();
    }
}

/**
 * @brief      Mount callback
 *
 * @param[in]  dev_addr  The device address
 */
void tuh_mount_cb(uint8_t dev_addr) {
    LOG("A device with address %d is mounted", dev_addr);
}

/**
 * @brief      Unmount callback
 *
 * @param[in]  dev_addr  The device address
 */
void tuh_umount_cb(uint8_t dev_addr) {
    LOG("A device with address %d is unmounted", dev_addr);
}
