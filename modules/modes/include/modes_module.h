// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      modes_module.h
//      Purpose :   Modes module (external)
//      Date :      7th July 2025
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
#include "dvi_module.h"

#define VMD_MAX_BUFFERS         (4)                                                 // Max # supported buffers.

typedef struct _VideoInformation {
    uint32_t    mode;                                                               // The current mode.
    uint8_t     *drawSurface;                                                       // Framebuffer to draw on.
    uint8_t     *displaySurface;                                                    // Framebuffer to display.
    uint16_t    xScreen,yScreen;                                                    // Screen pixel size.
    uint16_t    bytesPerLine;                                                       // Bytes used per line.
    uint8_t     pixelsPerByte;                                                      // Pixels in each byte.
    uint16_t    scanLineDivider;                                                    // Scan line division value.
    //
    // [PRIVATE]  _x should not be used as these may change. 
    // 
    uint32_t    _dviMode;                                                           // Mode set in the DVI library.
    uint16_t    _startDisplay,_startBlank;                                          // Scanline start of display, start of post display blank.
    uint8_t     bufferCount;                                                        // Number of buffers
    uint8_t     *buffers[VMD_MAX_BUFFERS];                                          // Buffers.
} VIDINFO;

extern VIDINFO vi;

void VMDInitialise(void);
void VMDSetVideoMemory(uint8_t *memory,uint32_t size);
void VMDSetMode(uint32_t mode);
//
//      Horizontal Resolutions
//
#define     VMODE_HRES_160      (0x0000)
#define     VMODE_HRES_320      (0x0001)
#define     VMODE_HRES_640      (0x0003)
//
//      Pixels per Byte
//
#define     VMODE_PPB_1         (0x0010)
#define     VMODE_PPB_2         (0x0020)
#define     VMODE_PPB_4         (0x0040)
#define     VMODE_PPB_8         (0x0080)
//
//      Colour enabled flag
//
#define     VMODE_USE_COLOUR    (0x0100)
//
//      Variable vertical resolution
//
#define     VMODE_VRES(l)       (((l) >> 3) << 9)
//
//      Currently supported modes.
//
#define     MODE_640_480_256    (VMODE_HRES_640|VMODE_USE_COLOUR|VMODE_PPB_1|VMODE_VRES(480))
#define     MODE_640_240_256    (VMODE_HRES_640|VMODE_USE_COLOUR|VMODE_PPB_1|VMODE_VRES(240))
#define     MODE_640_480_16     (VMODE_HRES_640|VMODE_USE_COLOUR|VMODE_PPB_2|VMODE_VRES(480))
#define     MODE_640_480_MONO4  (VMODE_HRES_640|VMODE_PPB_4|VMODE_VRES(480))
#define     MODE_640_480_MONO2  (VMODE_HRES_640|VMODE_PPB_8|VMODE_VRES(480))

#define     MODE_320_240_256    (VMODE_HRES_320|VMODE_USE_COLOUR|VMODE_PPB_1|VMODE_VRES(240))
#define     MODE_160_240_256    (VMODE_HRES_160|VMODE_USE_COLOUR|VMODE_PPB_1|VMODE_VRES(240))
#define     MODE_320_200_256    (VMODE_HRES_320|VMODE_USE_COLOUR|VMODE_PPB_1|VMODE_VRES(200))
#define     MODE_160_200_256    (VMODE_HRES_160|VMODE_USE_COLOUR|VMODE_PPB_1|VMODE_VRES(200))
#define     MODE_320_176_256    (VMODE_HRES_320|VMODE_USE_COLOUR|VMODE_PPB_1|VMODE_VRES(176))
#define     MODE_160_176_256    (VMODE_HRES_160|VMODE_USE_COLOUR|VMODE_PPB_1|VMODE_VRES(176))

void MODInitialise(void);
void MODSetFrameBuffer(uint8_t *frameBuffer,uint32_t size);
void MODSetMode(uint32_t mode);
