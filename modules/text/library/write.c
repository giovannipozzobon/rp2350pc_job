// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      write.c
//      Purpose :   Writing to window.
//      Date :      27th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "text_module.h"
#include "text_module_local.h"


/**
 * @brief      Write character to arbitrary window
 *
 * @param      window  Address of structure defining text window.
 * @param[in]  ch      Character to write.
 */
void TXTWriteWindow(TXTWINDOW *window,uint16_t ch) {
    TXTCheckStatus(window);
}