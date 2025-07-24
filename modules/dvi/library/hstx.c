// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      hstx.c
//      Purpose :   HSTX display program from pico-examples, slightly modified.
//      Date :      25th June 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Heavily based on the Pico SDK Examples and Scott Shawcroft's HSTX Library
//                  Read the Pico SDK Docs for this one.
//
// *******************************************************************************************
// *******************************************************************************************

#include "dvi_module.h"
#include "dvi_module_local.h"

//
//      Holds the current DVI rendering state information.
//
struct DVIRenderConfiguration dviConfig;

/**
 * @brief      Set the current mode. 
 *
 * @param[in]  modeInformation  Mode Information.
 */
void DVISetMode(uint16_t modeInformation) {
    DVIInitialisePalette();                                                         // Default palette
    dviConfig.currentMode = modeInformation;
}

/**
 * @brief      Set up the DVI HSTX registers
 *
 */
void KEEPINRAM(DVISetupRenderer)(void) {

    // Set up HSTX to render 640 pixels per line, 256 colours.
    // We mangle the inputs to make this work.
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

    DVIRenderInitialise();                                                          // Reset the renderer.

    // Serial output config: clock period of 5 cycles, pop from command
    // expander every 5 cycles, shift the output shiftreg by 2 every cycle.
    hstx_ctrl_hw->csr =
        HSTX_CTRL_CSR_EXPAND_EN_BITS |
        5u << HSTX_CTRL_CSR_CLKDIV_LSB |
        5u << HSTX_CTRL_CSR_N_SHIFTS_LSB |
        2u << HSTX_CTRL_CSR_SHIFT_LSB |
        HSTX_CTRL_CSR_EN_BITS;
}
