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
    switch(r->type) {                                                               // Figure out what to do with it.
        case 'K':
            INPProcessKeyboardReport(r);break;
        case 'M':
            INPProcessMouseReport(r);break;    }
    return false;
}

/**
 * @brief      Initialisation of input handler.
 */
void INPInitialise(void) {
    static bool isInitialised = false;                                              // Only initialise once.
    if (isInitialised) return;
    isInitialised = true;
    INPInitialiseStatus();                                                          // Initialise keyboard status.
    INPResetQueue();                                                                // Reset the queue.
    COMInitialise();                                                                // Common initialise
    INPSetLocale("us");                                                             // Default US locale.                    
    USBInitialise(true);                                                            // USB Initialise.
    USBInstallHandler(_INPReportHandler);                                           // Add a handler for USB HID reports.
}


// char buffer[128];
// sprintf(buffer,"%d %04x:%04x (%2d)",r->type,r->vid,r->pid,r->length);
// for (int i = 0;i < r->length;i++) sprintf(buffer+strlen(buffer)," %02x",r->data[i]);
// LOG(buffer);
