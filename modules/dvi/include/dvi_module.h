// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      dvi_module.h
//      Purpose :   DVI Sytem common header files.
//      Date :      25th June 2025
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
#include "hardware/dma.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/structs/bus_ctrl.h"
#include "hardware/structs/hstx_ctrl.h"
#include "hardware/structs/hstx_fifo.h"
#include "hardware/structs/sio.h"
#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "pico/multicore.h"
#include "pico/sem.h"
#endif

typedef uint8_t *(*DVILINEACCESSOR)(uint16_t scanLine);                             // Function that gets scanline data
typedef uint8_t *(*DVIRENDERER)(uint8_t func,uint8_t *data);                        // Function that renders a line manually.

void DVIInitialise(void);
void DVISetMode(uint16_t modeInformation);
void DVISetLineAccessorFunction(DVILINEACCESSOR dlafn);

#define DVIM_INITIALISE         (0)                                                 // Initialise manual renderer
#define DVIM_GETRENDER          (1)                                                 // Get the renderer for the given data.
#define DVIM_RENDERNEXT         (2)                                                 // Create the renderer for the given data.

#ifdef LOCALS

void DVISetupRenderer(void);
uint8_t *DVI320To640Renderer(uint8_t func,uint8_t *data);

struct DVIRenderConfiguration {
    uint8_t pixelsPerByte;                                                          // Pixels per byte of video data (1,2,4 or 8)
    bool    useByteDMA;                                                             // True if using byte DMA.
    bool    useManualRendering;                                                     // True if developer controlled rendering.
    DVIRENDERER renderer;                                                           // Function used for renderering.
    uint16_t pendingModeChange;                                                     // Pending mode change.
};

extern struct DVIRenderConfiguration dviRender;

void __scratch_x("") dma_irq_handler();
void DVISetUpDMA(void);

#define DMACH_PING 0
#define DMACH_PONG 1

#define TMDS_CTRL_00 0x354u
#define TMDS_CTRL_01 0x0abu
#define TMDS_CTRL_10 0x154u
#define TMDS_CTRL_11 0x2abu

#define SYNC_V0_H0 (TMDS_CTRL_00 | (TMDS_CTRL_00 << 10) | (TMDS_CTRL_00 << 20))
#define SYNC_V0_H1 (TMDS_CTRL_01 | (TMDS_CTRL_00 << 10) | (TMDS_CTRL_00 << 20))
#define SYNC_V1_H0 (TMDS_CTRL_10 | (TMDS_CTRL_00 << 10) | (TMDS_CTRL_00 << 20))
#define SYNC_V1_H1 (TMDS_CTRL_11 | (TMDS_CTRL_00 << 10) | (TMDS_CTRL_00 << 20))

#define MODE_H_SYNC_POLARITY 0
#define MODE_H_FRONT_PORCH   16
#define MODE_H_SYNC_WIDTH    96
#define MODE_H_BACK_PORCH    48
#define MODE_H_ACTIVE_PIXELS 640

#define MODE_V_SYNC_POLARITY 0
#define MODE_V_FRONT_PORCH   10
#define MODE_V_SYNC_WIDTH    2
#define MODE_V_BACK_PORCH    33
#define MODE_V_ACTIVE_LINES  480

#define MODE_H_TOTAL_PIXELS ( \
    MODE_H_FRONT_PORCH + MODE_H_SYNC_WIDTH + \
    MODE_H_BACK_PORCH  + MODE_H_ACTIVE_PIXELS \
)
#define MODE_V_TOTAL_LINES  ( \
    MODE_V_FRONT_PORCH + MODE_V_SYNC_WIDTH + \
    MODE_V_BACK_PORCH  + MODE_V_ACTIVE_LINES \
)

#define HSTX_CMD_RAW         (0x0u << 12)
#define HSTX_CMD_RAW_REPEAT  (0x1u << 12)
#define HSTX_CMD_TMDS        (0x2u << 12)
#define HSTX_CMD_TMDS_REPEAT (0x3u << 12)
#define HSTX_CMD_NOP         (0xfu << 12)

#endif

