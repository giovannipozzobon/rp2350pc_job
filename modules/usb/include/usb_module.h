// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      usb_module.h
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

#include "common_module.h"

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

void USBInitialise(bool waitForFS);                                                 // USB prototypes
void USBUpdate(void);
bool USBIsFileSystemAvailable(void);
bool USBInstallHandler(USBHANDLERFUNCTION handler);

#define FS_MAXFILENAMESIZE (128)                                                    // Max size of a filename

typedef struct _fsobjectInfo {                                                      // Structure to return dictionary search object
    bool isDirectory;
    uint32_t size;
    char name[FS_MAXFILENAMESIZE+1];
} FSOBJECTINFO;

int32_t FSCreate(char *fileName);                                                   // File & Directory prototypes.
int32_t FSDelete(char *fileName);
int32_t FSCreateDirectory(char *dirName);
int32_t FSDeleteDirectory(char *dirName);

int32_t FSOpenDirectory(char *dirName);
int32_t FSReadDirectory(int handle,FSOBJECTINFO *fso);
int32_t FSCloseDirectory(int handle);

int32_t FSOpen(char *fileName);
int32_t FSRead(int32_t handle,void *data,int32_t size);
int32_t FSWrite(int32_t handle,void *data,int32_t size);
int32_t FSSeek(int32_t handle,int32_t position);
int32_t FSTell(int32_t handle);
int32_t FSClose(int32_t handle);

#define FSERR_BADNAME       (-1)                                                    // Bad file name.
#define FSERR_SYSTEM        (-2)                                                    // System error.
#define FSERR_EXIST         (-3)                                                    // File/Path/Directory bad
#define FSERR_PROTECTED     (-4)                                                    // File or Volume locked/read only.
#define FSERR_BADHANDLE     (-5)                                                    // Bad Handle (out of range/ not open)
#define FSERR_TYPE          (-6)                                                    // File handle on Directory func or vice versa.
#define FSERR_EOF           (-7)                                                    // End of Read Directory/File

#ifdef LOCALS

#define USBHANDLERCOUNT     (4)                                                     // Max # report handlers supported

void USBHIDAppTask(void);
void USBDispatchReport(uint8_t type,uint16_t vid, uint16_t pid, uint8_t *report, uint16_t len);
int32_t FSMapErrorCode(FRESULT res);
int32_t FSGetValidateHandle(int32_t handle, bool isDirectory,void **fsObjectPtr);

void FSInitialise(void);
bool FSProcessFileName(char **pFileName);
int32_t FSAllocateRecord(bool isDirectory);
void FSFreeRecord(uint32_t handle);

#endif

