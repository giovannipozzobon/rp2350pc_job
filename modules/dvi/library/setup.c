// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      setup.c
//      Purpose :   DVI Initialisation
//      Date :      17th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "dvi_module.h"
#include "dvi_module_local.h"

static void DVIInitialiseMain(void);

/**
 * @brief      Initialise the DVI system, HSTX and DMA.
 */

void DVIInitialise(void) {
    static bool isInitialised = false;                                              // Only initialise once.
    if (isInitialised) return;
    isInitialised = true;
    multicore_launch_core1(DVIInitialiseMain);                                      // So we're running DVI stuff on core1.
    //DVIInitialiseMain();
}

/**
 * @brief      This does the actual initialisation.
 */
static void DVIInitialiseMain(void) {
    COMInitialise();                                                                // Initialise common.

    dviConfig.renderer = NULL;
    dviConfig.verticalSync = NULL;

    hstx_ctrl_hw->csr = 0;

    // Serial output config: clock period of 5 cycles, pop from command
    // expander every 5 cycles, shift the output shiftreg by 2 every cycle.
    // 
    // Note: this needs to be done here, as well as after the mode switch, otherwise it doesn't work.
    // 
    hstx_ctrl_hw->csr =
        HSTX_CTRL_CSR_EXPAND_EN_BITS |
        5u << HSTX_CTRL_CSR_CLKDIV_LSB |
        5u << HSTX_CTRL_CSR_N_SHIFTS_LSB |
        2u << HSTX_CTRL_CSR_SHIFT_LSB |
        HSTX_CTRL_CSR_EN_BITS;

    // Note we are leaving the HSTX clock at the SDK default of 125 MHz; since
    // we shift out two bits per HSTX clock cycle, this gives us an output of
    // 250 Mbps, which is very close to the bit clock for 480p 60Hz (252 MHz).
    // If we want the exact rate then we'll have to reconfigure PLLs.

    // HSTX outputs 0 through 7 appear on GPIO 12 through 19.
    // Pinout on Pico DVI sock:
    //
    //   GP12 D0+  GP13 D0-
    //   GP14 CK+  GP15 CK-
    //   GP16 D2+  GP17 D2-
    //   GP18 D1+  GP19 D1-

    // Assign clock pair to two neighbouring pins:
    hstx_ctrl_hw->bit[2] = HSTX_CTRL_BIT0_CLK_BITS;
    hstx_ctrl_hw->bit[3] = HSTX_CTRL_BIT0_CLK_BITS | HSTX_CTRL_BIT0_INV_BITS;
    for (uint lane = 0; lane < 3; ++lane) {
        // For each TMDS lane, assign it to the correct GPIO pair based on the
        // desired pinout:
        static const int lane_to_output_bit[3] = {0, 6, 4};
        int bit = lane_to_output_bit[lane];
        // Output even bits during first half of each HSTX cycle, and odd bits
        // during second half. The shifter advances by two bits each cycle.
        uint32_t lane_data_sel_bits =
            (lane * 10    ) << HSTX_CTRL_BIT0_SEL_P_LSB |
            (lane * 10 + 1) << HSTX_CTRL_BIT0_SEL_N_LSB;
        // The two halves of each pair get identical data, but one pin is inverted.
        hstx_ctrl_hw->bit[bit    ] = lane_data_sel_bits;
        hstx_ctrl_hw->bit[bit + 1] = lane_data_sel_bits | HSTX_CTRL_BIT0_INV_BITS;
    }

    for (int i = 12; i <= 19; ++i) {
        gpio_set_function(i, 0); // HSTX
    }
    DVISetUpDMA();

    while (true) {}
}
