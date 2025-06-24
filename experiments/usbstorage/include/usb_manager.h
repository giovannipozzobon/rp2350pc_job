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

#ifdef LOCALS
#include <ff.h>
#include "pico/stdlib.h"
#include "bsp/board_api.h"
#include "tusb.h"
#include "diskio.h"
#endif

void USBInitialise(void);
void USBUpdate(void);
bool USBIsFileSystemAvailable(void);

#ifdef LOCALS
void USBHIDAppTask(void);
#endif

