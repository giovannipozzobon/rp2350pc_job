// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      hstx_encoder.c
//      Purpose :   HSTX display program from pico-examples, slightly modified.
//      Date :      25th June 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Heavily based on the Pico SDK Examples and Scott Shawcroft's HSTX Library
//
// *******************************************************************************************
// *******************************************************************************************

#include "dvi_module.h"
#include "dvi_module_local.h"

//
//      Holds the current DVI rendering state information.
//
struct DVIRenderConfiguration dviConfig;

// *******************************************************************************************
// 
//                              HSTX setup routines
// 
// *******************************************************************************************

/**
 * @brief      Set up HSTX for 1 pixel per byte e.g. 256 colour mode
 */
static void dvi1PixelPerByte(void) {
    // Configure HSTX's TMDS encoder for RGB332
    hstx_ctrl_hw->expand_tmds =
            2  << HSTX_CTRL_EXPAND_TMDS_L2_NBITS_LSB |
            0  << HSTX_CTRL_EXPAND_TMDS_L2_ROT_LSB   |
            2  << HSTX_CTRL_EXPAND_TMDS_L1_NBITS_LSB |
            29 << HSTX_CTRL_EXPAND_TMDS_L1_ROT_LSB   |
            1  << HSTX_CTRL_EXPAND_TMDS_L0_NBITS_LSB |
            26 << HSTX_CTRL_EXPAND_TMDS_L0_ROT_LSB;

    // Pixels (TMDS) come in 4 8-bit chunks. Control symbols (RAW) are an
    // entire 32-bit word.
    hstx_ctrl_hw->expand_shift =
            4 << HSTX_CTRL_EXPAND_SHIFT_ENC_N_SHIFTS_LSB |
            8 << HSTX_CTRL_EXPAND_SHIFT_ENC_SHIFT_LSB |
            1 << HSTX_CTRL_EXPAND_SHIFT_RAW_N_SHIFTS_LSB |
            0 << HSTX_CTRL_EXPAND_SHIFT_RAW_SHIFT_LSB;
}

/**
 * @brief      Set up HSTX for 2 pixels per byte, e.g. 16 colour mode RGGB
 */
static void dvi2PixelsPerByte(void) {
    // Configure HSTX's TMDS encoder for RGBD
        hstx_ctrl_hw->expand_tmds =
            0 << HSTX_CTRL_EXPAND_TMDS_L2_NBITS_LSB |
            28 << HSTX_CTRL_EXPAND_TMDS_L2_ROT_LSB |
            1 << HSTX_CTRL_EXPAND_TMDS_L1_NBITS_LSB |
            27 << HSTX_CTRL_EXPAND_TMDS_L1_ROT_LSB |
            0 << HSTX_CTRL_EXPAND_TMDS_L0_NBITS_LSB |
            25 << HSTX_CTRL_EXPAND_TMDS_L0_ROT_LSB;

    // Pixels (TMDS) come in 8 4-bit chunks. Control symbols (RAW) are an
    // entire 32-bit word.
    hstx_ctrl_hw->expand_shift =
            8 << HSTX_CTRL_EXPAND_SHIFT_ENC_N_SHIFTS_LSB |
            4 << HSTX_CTRL_EXPAND_SHIFT_ENC_SHIFT_LSB |
            1 << HSTX_CTRL_EXPAND_SHIFT_RAW_N_SHIFTS_LSB |
            0 << HSTX_CTRL_EXPAND_SHIFT_RAW_SHIFT_LSB;
}

/**
 * @brief      Set up HSTX for 4 pixels per byte, e.g. 4 greyscale mode.
 */
static void dvi4PixelsPerByte(void) {
        uint8_t color_depth = 2;
        uint8_t rot = 24 + color_depth;
        hstx_ctrl_hw->expand_tmds =
            (color_depth - 1) << HSTX_CTRL_EXPAND_TMDS_L2_NBITS_LSB |
                rot << HSTX_CTRL_EXPAND_TMDS_L2_ROT_LSB |
                    (color_depth - 1) << HSTX_CTRL_EXPAND_TMDS_L1_NBITS_LSB |
                rot << HSTX_CTRL_EXPAND_TMDS_L1_ROT_LSB |
                    (color_depth - 1) << HSTX_CTRL_EXPAND_TMDS_L0_NBITS_LSB |
                rot << HSTX_CTRL_EXPAND_TMDS_L0_ROT_LSB;

    // Pixels (TMDS) come in 16 2-bit chunks. Control symbols (RAW) are an
    // entire 32-bit word.
    hstx_ctrl_hw->expand_shift =
            16 << HSTX_CTRL_EXPAND_SHIFT_ENC_N_SHIFTS_LSB |
            2 << HSTX_CTRL_EXPAND_SHIFT_ENC_SHIFT_LSB |
            1 << HSTX_CTRL_EXPAND_SHIFT_RAW_N_SHIFTS_LSB |
            0 << HSTX_CTRL_EXPAND_SHIFT_RAW_SHIFT_LSB;
}

/**
 * @brief      Set up HSTX for 8 pixels per byte, e.g. 2 colour mode.
 */
static void dvi8PixelsPerByte(void) {
    // Configure HSTX's TMDS encoder for RGBD
    uint8_t color_depth = 1;
    uint8_t rot = 24 + color_depth;
    hstx_ctrl_hw->expand_tmds =
            (color_depth - 1) << HSTX_CTRL_EXPAND_TMDS_L2_NBITS_LSB |
                rot << HSTX_CTRL_EXPAND_TMDS_L2_ROT_LSB |
                    (color_depth - 1) << HSTX_CTRL_EXPAND_TMDS_L1_NBITS_LSB |
                rot << HSTX_CTRL_EXPAND_TMDS_L1_ROT_LSB |
                    (color_depth - 1) << HSTX_CTRL_EXPAND_TMDS_L0_NBITS_LSB |
                rot << HSTX_CTRL_EXPAND_TMDS_L0_ROT_LSB;

    // Pixels (TMDS) come in 8 1-bit chunks. Control symbols (RAW) are an
    // entire 32-bit word.
    hstx_ctrl_hw->expand_shift =
            32 << HSTX_CTRL_EXPAND_SHIFT_ENC_N_SHIFTS_LSB |
            1 << HSTX_CTRL_EXPAND_SHIFT_ENC_SHIFT_LSB |
            1 << HSTX_CTRL_EXPAND_SHIFT_RAW_N_SHIFTS_LSB |
            0 << HSTX_CTRL_EXPAND_SHIFT_RAW_SHIFT_LSB;
}

/**
 * @brief      Set the current mode. This actually *doesn't* set the current
 *             mode, it stores it to be changed at the next vsync.
 *
 * @param[in]  modeInformation  Mode Information.
 */
void DVISetMode(uint16_t modeInformation) {
    dviConfig.pendingModeChange = modeInformation;
}

/**
 * @brief      Set up the DVI HSTX registers
 *
 */
void DVISetupRenderer(void) {
    dviConfig.pixelsPerByte = dviConfig.pendingModeChange & 0x0F;
    dviConfig.useByteDMA = ((dviConfig.pendingModeChange) & 0x8000) != 0;
    dviConfig.useManualRendering = ((dviConfig.pendingModeChange) & 0x4000) != 0;

    if (dviConfig.useManualRendering && dviConfig.renderer == NULL) {               // Use default manual rendering ?
        dviConfig.renderer = DVI320To640Renderer;
    }

    if (dviConfig.useManualRendering) {                                             // Initialise the manual renderer.
        (*dviConfig.renderer)(DVIM_INITIALISE,NULL);       
    }

    dviConfig.pendingModeChange = 0;

    switch(dviConfig.pixelsPerByte) {
        case 1:
            dvi1PixelPerByte();break;
        case 2:
            dvi2PixelsPerByte();break;
        case 4:
            dvi4PixelsPerByte();break;
        case 8:
            dvi8PixelsPerByte();break;
    }

    // Serial output config: clock period of 5 cycles, pop from command
    // expander every 5 cycles, shift the output shiftreg by 2 every cycle.
    hstx_ctrl_hw->csr =
        HSTX_CTRL_CSR_EXPAND_EN_BITS |
        5u << HSTX_CTRL_CSR_CLKDIV_LSB |
        5u << HSTX_CTRL_CSR_N_SHIFTS_LSB |
        2u << HSTX_CTRL_CSR_SHIFT_LSB |
        HSTX_CTRL_CSR_EN_BITS;
}
