// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      dvi_module_local.h
//      Purpose :   DVI Sytem common header files.
//      Date :      25th June 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#pragma once

#ifndef RUNTIME
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

#define MAX_VSYNC_HANDLER       (8)                                                 // Max number of vsync handlers

void DVISetupRenderer(void);
uint8_t *DVI320To640Renderer(uint8_t func,uint8_t *data);

void ASMRender160_256(uint8_t *target,uint8_t *data,uint8_t *palette);           
void ASMRender320_256(uint8_t *target,uint8_t *data,uint8_t *palette);           
void ASMRender640_256(uint8_t *target,uint8_t *data,uint8_t *palette);           

struct DVIRenderConfiguration {
    uint16_t pendingModeChange;                                                     // Pending mode change.
    uint16_t currentMode;                                                           // Current mode.
};

typedef struct _DVIRenderBuffer {                                                   // A single buffer for render
    uint8_t *source;                                                                // Source address used to render data
    uint8_t render[640];                                                            // The rendered result.
} DVIRenderBuffer;

void DVISetupDMA(void);
void DVIInitialiseMain(void);
uint32_t DVIGetScanLineTime(void);
void DVIInitialisePalette(void);
void DVIRenderInitialise(void);
void DVIRenderOneLine(uint8_t *data);
uint8_t *DVIGetRenderedLine(uint8_t *data);
void DVISetupHSTX(void);

extern struct DVIRenderConfiguration dviConfig;
extern uint8_t DVIPalette[256];

void __scratch_x("") dma_irq_handler();

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
