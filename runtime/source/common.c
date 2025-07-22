// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      common.c
//      Purpose :   Common code
//      Date :      5th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include <runtime.h>

#define MAX_UPDATE_FUNCS    (8)

static COMUPDATEFUNCTION updateFunctions[MAX_UPDATE_FUNCS];                         // Update functions
static uint32_t updateFunctionCount = 0;

/**
 * @brief      Dummy initialise
 */
void COMInitialise(void) {
}

/**
 * @brief      Get elapsed time since start
 *
 * @return     time in 1khz ticks
 */
uint32_t COMTimeMS(void) {
    return SDL_GetTicks();
}

/**
 * @brief      Register an update function
 *
 * @param[in]  updateFunc  Update function to register
 */
void COMAddUpdateFunction(COMUPDATEFUNCTION updateFunc) {
    if (updateFunctionCount < MAX_UPDATE_FUNCS) {
        updateFunctions[updateFunctionCount++] = updateFunc;
    }
}

/**
 * @brief      Update the USB system
 */
void COMUpdate(void) {
    for (int i = 0;i < updateFunctionCount;i++) {
        (*updateFunctions[i])();
    }
}

/**
 * @brief      Print string on debug log.
 *
 * @param      format     Format string
 * @param[in]  <unnamed>  Data for format string.
 */
void COMPrint(char *format,...) {
    char buf[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, 128, format, args);
    printf("%s\n",buf);
    va_end(args);
}

/**
 * @brief      Report an error
 *
 * @param      msg       Error message
 * @param[in]  line      Line number
 * @param      fileName  Source file
 */
void COMError(char *msg,int line,char *fileName) {
    COMPrint("[ERROR] %s (%s:%d)",msg,fileName,line);
    exit(-1);
}
