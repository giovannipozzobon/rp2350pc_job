// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Input Manager main program
//      Date :      2nd July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "common_module.h"
#include "usb_module.h"

bool _ReportHandler(USBREPORT *r) {
    char buffer[128];
    sprintf(buffer,"%d %04x:%04x (%2d)",r->type,r->vid,r->pid,r->length);
    for (int i = 0;i < r->length;i++) sprintf(buffer+strlen(buffer)," %02x",r->data[i]);
    LOG(buffer);
    return false;
}

int main(int argc,char *argv[]) {
    COMInitialise();
    USBInitialise(true);                                                            // Set up, and wait for the USB Key
    USBInstallHandler(_ReportHandler);                                              // Add a handler for USB HID reports.
    while (1) {                                                                     // Run USB dumping USB reports as raw data
        USBUpdate();
    }	
    return 0;
}
