// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Runtime main program.
//      Date :      4th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "runtime.h"

bool isAppRunning = true;

#define FRAME_RATE  (50)

static int nextUpdateTime = 0;

/**
 * @brief      Is the app still running (for simulator)
 *
 * @return     true if the app is still running.
 */
bool COMAppRunning(void) {
    return isAppRunning;
}


/**
 * @brief      Yield, body executed at 50Hz
 *
 * @return     true if 50Hz tick occurred.
 */
bool SYSYield(void) {
    if (COMTimeMS() >= nextUpdateTime) {                                            // So do this to limit the repaint rate to 50Hz.
        nextUpdateTime = COMTimeMS()+1000/FRAME_RATE;
        if (SYSPollUpdate() == 0) isAppRunning = false;
        return true;
    }
    return false;
}

/**
 * @brief      Main program.
 *
 * @param[in]  argc  The count of arguments
 * @param      argv  The arguments array
 *
 * @return     { description_of_the_return_value }
 */

void MainApplication(void);

int main(int argc,char *argv[]) {
    SYSOpen(false);                                                                 // Start SDL and Mouse/Controller/Sound that use it
    MainApplication();                                                              // Run the program
    SYSClose();                                                                     // Close down
    return(0);
}
