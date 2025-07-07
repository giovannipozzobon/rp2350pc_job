#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef LOCALS
#endif

//
//      Horizontal Resolutions
//
#define     VMODE_HRES_160      (0x0000)
#define     VMODE_HRES_320      (0x0001)
#define     VMODE_HRES_640      (0x0002)
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
#define     VMODE_VRES(l)       (((l) >> 4) << 9)
//
//      Currently supported modes.
//
#define     MODE_640_480_256    (VMODE_HRES_640|VMODE_USE_COLOUR|VMODE_PPB_1|VMODE_VRES(480))
#define     MODE_640_480_16     (VMODE_HRES_640|VMODE_USE_COLOUR|VMODE_PPB_2|VMODE_VRES(480))
#define     MODE_640_480_MONO4  (VMODE_HRES_640|VMODE_PPB_4|VMODE_VRES(480))
#define     MODE_640_480_MONO1  (VMODE_HRES_640|VMODE_PPB_8|VMODE_VRES(480))

#define     MODE_320_240_256    (VMODE_HRES_320|VMODE_USE_COLOUR|VMODE_PPB_1|VMODE_VRES(240))
#define     MODE_160_240_256    (VMODE_HRES_160|VMODE_USE_COLOUR|VMODE_PPB_1|VMODE_VRES(240))
#define     MODE_320_200_256    (VMODE_HRES_320|VMODE_USE_COLOUR|VMODE_PPB_1|VMODE_VRES(200))
#define     MODE_160_200_256    (VMODE_HRES_160|VMODE_USE_COLOUR|VMODE_PPB_1|VMODE_VRES(200))

void MODInitialise(void)
void MODSetFrameBuffer(uint8_t *frameBuffer,uint32_t size);
void MODSetMode(uint32_t mode);
