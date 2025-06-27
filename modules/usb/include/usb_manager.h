// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      usb_manager.h
//      Purpose :   USB code common header files.
//      Date :      24th June 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef LOCALS
#include "pico/stdlib.h"
#include "bsp/board_api.h"
#include <ff.h>
#include "tusb.h"
#include "diskio.h"
#endif

#define USBHANDLERCOUNT     (4)                                                     // Max # report handlers supported

typedef struct _usbReport {                                                         // USB Report record passed to handler
    uint8_t     type;
    uint8_t     vid,pid;
    uint8_t     *data;
    uint16_t    length;
} USBREPORT;

typedef bool (*USBHANDLERFUNCTION)(USBREPORT *report);                              // Handler pointer type.

void USBInitialise(bool waitForFS);
void USBUpdate(void);
bool USBIsFileSystemAvailable(void);
bool USBInstallHandler(USBHANDLERFUNCTION handler);

#ifdef LOCALS
void USBHIDAppTask(void);
void USBDispatchReport(uint8_t type,uint16_t vid, uint16_t pid, uint8_t const* report, uint16_t len);
#endif

