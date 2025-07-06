// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      usb_module_local.h
//      Purpose :   USB code common header files.
//      Date :      24th June 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#pragma once

#ifndef RUNTIME
#include "pico/stdlib.h"
#include "bsp/board_api.h"
#include <ff.h>
#include "tusb.h"
#include "diskio.h"
#endif

#define CHECKFSAVAILABLE() if (!USBWaitForFileSystem()) return FSERR_STORAGE        // Wait FS, error if times out.

#define INPUSBKEY_TIMEOUT   (10*1000)                                               // USB Key timeout, in ms.
#define USBHANDLERCOUNT     (4)                                                     // Max # report handlers supported

void USBHIDAppTask(void);
void USBDispatchReport(uint8_t type,uint16_t vid, uint16_t pid, uint8_t *report, uint16_t len);
int32_t FSMapErrorCode(FRESULT res);
int32_t FSGetValidateHandle(int32_t handle, bool isDirectory,void **fsObjectPtr);

void FSInitialise(void);
bool FSProcessFileName(char **pFileName);
int32_t FSAllocateRecord(bool isDirectory);
void FSFreeRecord(uint32_t handle);


