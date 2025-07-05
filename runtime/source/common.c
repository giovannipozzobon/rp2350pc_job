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

/**
 * @brief      Get elapsed time since start
 *
 * @return     time in 1khz ticks
 */
uint32_t COMClockMS(void) {
    return SDL_GetTicks();
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
    exit(-1);
}
