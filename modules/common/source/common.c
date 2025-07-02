// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      common.c
//      Purpose :   Common code
//      Date :      28th June 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#define LOCALS
#include "common_module.h"
#include "stdio.h"
#include "pico/stdlib.h"

/**
 * @brief      Common initialise code.
 */
void COMInitialise(void) {
    static bool isInitialised = false;                                              // Only initialise once.
    if (isInitialised) return;
    isInitialised = true;

    stdio_init_all();
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
    printf(buf);printf("\n");
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
}

/**
 * @brief      Return the system clock
 *
 * @return     Elapsed time in milliseconds
 */
uint32_t COMClockMS(void) {
    return (uint32_t)(time_us_64() >> 10);
}