// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      reports.c
//      Purpose :   Handle USB Reports
//      Date :      2nd July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "common_module.h"
#include "usb_module.h"

#define LOCALS
#include "input_module.h"

/**
 * @brief      Handle USB HID reports
 *
 * @param      r     USB HID report
 *
 * @return     False as we don't consume them.
 */
static bool _INPReportHandler(USBREPORT *r) {
    char buffer[128];
    sprintf(buffer,"%d %04x:%04x (%2d)",r->type,r->vid,r->pid,r->length);
    for (int i = 0;i < r->length;i++) sprintf(buffer+strlen(buffer)," %02x",r->data[i]);
    LOG(buffer);
    return false;
}

/**
 * @brief      Initialisation of input handler.
 */
void INPInitialise(void) {
    static bool isInitialised = false;                                              // Only initialise once.
    if (isInitialised) return;
    isInitialised = true;

    COMInitialise();                                                                // Common initialise
    USBInitialise(true);                                                            // USB Initialise.
    USBInstallHandler(_INPReportHandler);                                           // Add a handler for USB HID reports.
}
