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

void USBInitialise(void);
void USBUpdate(void);
bool USBIsFileSystemAvailable(void);
void USBReportHandler(uint8_t type,uint16_t vid, uint16_t pid, uint8_t const* report, uint16_t len);

#ifdef LOCALS
void USBHIDAppTask(void);
#endif

