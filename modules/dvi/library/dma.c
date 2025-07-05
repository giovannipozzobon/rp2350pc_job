// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      dma.c
//      Purpose :   DMA Code
//      Date :      25th June 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Heavily based on the Pico SDK Examples and Scott Shawcroft's HSTX Library
//
// *******************************************************************************************
// *******************************************************************************************

#define LOCALS
#include "dvi_module.h"

// *******************************************************************************************
// 
//                                          HSTX command lists
//                                          
//      Lists are padded with NOPs to be >= HSTX FIFO size, to avoid DMA rapidly
//      pingponging and tripping up the IRQs.
//      
// *******************************************************************************************

static uint32_t vblank_line_vsync_off[] = {
    HSTX_CMD_RAW_REPEAT | MODE_H_FRONT_PORCH,
    SYNC_V1_H1,
    HSTX_CMD_RAW_REPEAT | MODE_H_SYNC_WIDTH,
    SYNC_V1_H0,
    HSTX_CMD_RAW_REPEAT | (MODE_H_BACK_PORCH + MODE_H_ACTIVE_PIXELS),
    SYNC_V1_H1,
    HSTX_CMD_NOP
};

static uint32_t vblank_line_vsync_on[] = {
    HSTX_CMD_RAW_REPEAT | MODE_H_FRONT_PORCH,
    SYNC_V0_H1,
    HSTX_CMD_RAW_REPEAT | MODE_H_SYNC_WIDTH,
    SYNC_V0_H0,
    HSTX_CMD_RAW_REPEAT | (MODE_H_BACK_PORCH + MODE_H_ACTIVE_PIXELS),
    SYNC_V0_H1,
    HSTX_CMD_NOP
};

static uint32_t vactive_line[] = {
    HSTX_CMD_RAW_REPEAT | MODE_H_FRONT_PORCH,
    SYNC_V1_H1,
    HSTX_CMD_NOP,
    HSTX_CMD_RAW_REPEAT | MODE_H_SYNC_WIDTH,
    SYNC_V1_H0,
    HSTX_CMD_NOP,
    HSTX_CMD_RAW_REPEAT | MODE_H_BACK_PORCH,
    SYNC_V1_H1,
    HSTX_CMD_TMDS       | MODE_H_ACTIVE_PIXELS
};

// *******************************************************************************************
// 
//                                          DMA logic
// 
// *******************************************************************************************

static dma_channel_config cPing,cPong;

//  First we ping. Then we pong. Then... we ping again.
static bool dma_pong = false;

//  A ping and a pong are cued up initially, so the first time we enter this
//  handler it is to cue up the second ping after the first ping has completed.
//  This is the third scanline overall (-> =2 because zero-based).
static uint v_scanline = 2;

//  During the vertical active period, we take two IRQs per scanline: one to
//  post the command list, and another to post the pixels.
static bool vactive_cmdlist_posted = false;

//  Blank line used for Blank lines
uint8_t blankLine[640] = {0} ;

//  Line Data access function
static DVILINEACCESSOR lineAccessFunction = NULL;

/**
 * @brief      Set the line data accessor function
 *
 * @param[in]  dlafn  function to access line.
 */
void DVISetLineAccessorFunction(DVILINEACCESSOR dlafn) {
    lineAccessFunction = dlafn;
}
/**
 * @brief      IRQ Handle for DMA used in DVI
 *
 * @param[in]  <unnamed>  None
 */
void __scratch_x("") dma_irq_handler() {
    // dma_pong indicates the channel that just finished, which is the one
    // we're about to reload.
    uint ch_num = dma_pong ? DMACH_PONG : DMACH_PING;

    // So if bit 15 of mode is set, and we are in the pixel rendering region, use DMA Byte size, not Word size
    channel_config_set_transfer_data_size(dma_pong ? &cPong:&cPing,(vactive_cmdlist_posted && dviConfig.useByteDMA) ? DMA_SIZE_8 : DMA_SIZE_32);
    dma_channel_set_config(ch_num, dma_pong ? &cPong:&cPing,false);

    dma_channel_hw_t *ch = &dma_hw->ch[ch_num];
    dma_hw->intr = 1u << ch_num;
    dma_pong = !dma_pong;

    //
    //      Vertical sync part. If a mode change is pending it is done here.
    //
    if (v_scanline >= MODE_V_FRONT_PORCH && v_scanline < (MODE_V_FRONT_PORCH + MODE_V_SYNC_WIDTH)) {
        if (dviConfig.pendingModeChange != 0) {                             
            DVISetupRenderer();
        }
        ch->read_addr = (uintptr_t)vblank_line_vsync_on;
        ch->transfer_count = count_of(vblank_line_vsync_on);
    //
    //      Vertical blank period.
    //
    } else if (v_scanline < MODE_V_FRONT_PORCH + MODE_V_SYNC_WIDTH + MODE_V_BACK_PORCH) {
        ch->read_addr = (uintptr_t)vblank_line_vsync_off;
        ch->transfer_count = count_of(vblank_line_vsync_off);
    //
    //      In video generation, the preamble control before the actual video data.
    //
    } else if (!vactive_cmdlist_posted) {
        ch->read_addr = (uintptr_t)vactive_line;
        ch->transfer_count = count_of(vactive_line);
        vactive_cmdlist_posted = true;
    //
    //      Render using DMA
    //
    } else {
        uint8_t *scanLineData = blankLine;                                          // Default is do blank lines.
        uint16_t scanLine = v_scanline - (MODE_V_TOTAL_LINES-MODE_V_ACTIVE_LINES);  // Scanline to render (0-479)
        if (lineAccessFunction != NULL) {                                           // If we have a line access callback
            scanLineData = (*lineAccessFunction)(scanLine);                         // Get the scanline data for the current line.
            if (scanLineData == NULL) {                                             // If NULL, render a blank line.
                scanLineData = blankLine;
            } else {                                                                // If not NULL, and using manual rendering, retrieve
                if (dviConfig.useManualRendering) {                                 // the manual renderer. 
                    scanLineData = (*dviConfig.renderer)(DVIM_GETRENDER,scanLineData);
                }
            }
        }

        ch->read_addr = (uintptr_t)scanLineData;                                    // Start the DMA transfer
        ch->transfer_count = MODE_H_ACTIVE_PIXELS / sizeof(uint32_t) / dviConfig.pixelsPerByte / 1;

        if (dviConfig.useManualRendering && lineAccessFunction != NULL) {           // If manual rendering, we want to get the next line.
            scanLineData = (*lineAccessFunction)((scanLine+1) % 480);               // So retrieve the next line data.
            if (scanLineData != NULL) {                                             // If it isn't blank, render it.
                (*dviConfig.renderer)(DVIM_RENDERNEXT,scanLineData);
            }
        }
        vactive_cmdlist_posted = false;
    }

    if (!vactive_cmdlist_posted) {
        v_scanline = (v_scanline + 1) % MODE_V_TOTAL_LINES;
    }
}

/**
 * @brief      Set up the DMA System to display video.
 */
void DVISetUpDMA(void) {
    // Both channels are set up identically, to transfer a whole scanline and
    // then chain to the opposite channel. Each time a channel finishes, we
    // reconfigure the one that just finished, meanwhile the opposite channel
    // is already making progress.
    cPing = dma_channel_get_default_config(DMACH_PING);
    channel_config_set_chain_to(&cPing, DMACH_PONG);
    channel_config_set_dreq(&cPing, DREQ_HSTX);
    dma_channel_configure(
        DMACH_PING,
        &cPing,
        &hstx_fifo_hw->fifo,
        vblank_line_vsync_off,
        count_of(vblank_line_vsync_off),
        false
    );

    cPong = dma_channel_get_default_config(DMACH_PONG);
    channel_config_set_chain_to(&cPong, DMACH_PING);
    channel_config_set_dreq(&cPong, DREQ_HSTX);
    dma_channel_configure(
        DMACH_PONG,
        &cPong,
        &hstx_fifo_hw->fifo,
        vblank_line_vsync_off,
        count_of(vblank_line_vsync_off),
        false
    );

    //dma_channel_set_config(DMACH_PONG,&c,false);

    dma_hw->ints0 = (1u << DMACH_PING) | (1u << DMACH_PONG);
    dma_hw->inte0 = (1u << DMACH_PING) | (1u << DMACH_PONG);
    irq_set_exclusive_handler(DMA_IRQ_0, dma_irq_handler);
    irq_set_enabled(DMA_IRQ_0, true);

    bus_ctrl_hw->priority = BUSCTRL_BUS_PRIORITY_DMA_W_BITS | BUSCTRL_BUS_PRIORITY_DMA_R_BITS;

    dma_channel_start(DMACH_PING);
}

