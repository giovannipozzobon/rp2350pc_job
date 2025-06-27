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

int32_t FSCreate(char *fileName);
int32_t FSDelete(char *fileName);
int32_t FSCreateDirectory(char *dirName);
int32_t FSDeleteDirectory(char *dirName);

#define FSERR_BADNAME       (-1)                                                    // Bad file name.
#define FSERR_SYSTEM        (-2)                                                    // System error.
#define FSERR_EXIST         (-3)                                                    // File/Path/Directory bad
#define FSERR_PROTECTED     (-4)                                                    // File or Volume locked/read only.

#ifdef LOCALS

#define USBHANDLERCOUNT     (4)                                                     // Max # report handlers supported

void USBHIDAppTask(void);
void USBDispatchReport(uint8_t type,uint16_t vid, uint16_t pid, uint8_t *report, uint16_t len);
int32_t FSMapErrorCode(FRESULT res);

void FSInitialise(void);
bool FSProcessFileName(char **pFileName);
int32_t FSAllocateRecord(bool isDirectory);
void FSFreeRecord(uint32_t handle);

#endif

