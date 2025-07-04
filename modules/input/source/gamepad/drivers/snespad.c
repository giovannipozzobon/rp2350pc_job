// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      snespad.c
//      Purpose :   Driver for the Olimex badged SNES pad 081F:E401
//      Date :      4th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#define LOCALS
#include "input_module.h"

/**
 * @brief      Handle report for 081f:e401
 *
 * @param      r     USB report data
 */
void INPGeneric_081f_e401(USBREPORT *r) {
    // char buffer[128];
    // sprintf(buffer,"%d %04x:%04x (%2d)",r->type,r->vid,r->pid,r->length);
    // for (int i = 0;i < r->length;i++) sprintf(buffer+strlen(buffer)," %02x",r->data[i]);
    // //LOG(buffer);
}

