// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      generic.c
//      Purpose :   Handle USB Generic Reports (in practice, Gamepads)
//      Date :      4th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#define LOCALS
#include "input_module.h"

#define DEVICE(v,p)         ((v)<<8|(p))
#define MAXMONITORPACKETSIZE    (16)                                                // Max size of trackable data packets.

static void INPMonitorDevice(USBREPORT *r);

static uint8_t lastPacket[MAXMONITORPACKETSIZE];                                    // Last data packet received
static bool    acceptedPacket = false;                                              // False when no packet yet received

/**
 * @brief      Handle generic reports
 *
 * @param      r     USB Generic report.
 */
void INPProcessGenericReport(USBREPORT *r) {
    //  INPMonitorDevice(r);
    switch(DEVICE(r->vid,r->pid)) {

        case DEVICE(0x081F,0xE401):                                                 // This is the SNES look keypad that plugs into the USB.
            INPGeneric_081f_e401(r);break;
    }
}

/**
 * @brief      Debugger / Packet analysis of input data from generic device.
 *
 * @param      r     USB Report
 */
static void INPMonitorDevice(USBREPORT *r) {

}

