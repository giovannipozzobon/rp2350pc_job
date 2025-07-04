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

#define min(a,b) (((a) < (b)) ? (a):(b))

static void INPMonitorDevice(USBREPORT *r);

static uint8_t lastPacket[MAXMONITORPACKETSIZE];                                    // Last data packet received
static uint8_t lastPacketSize;
static bool    acceptedPacket = false;                                              // False when no packet yet received

GAMEPAD gp;                                                                         // Current gamepad status

/**
 * @brief      Handle generic reports
 *
 * @param      r     USB Generic report.
 */
void INPProcessGenericReport(USBREPORT *r) {

    //INPMonitorDevice(r);                                                          // When uncommented debug info about byte changes will be on log.

    switch(DEVICE(r->vid,r->pid)) {

        case DEVICE(0x081F,0xE401):                                                 // This is the SNES look keypad that plugs into the USB.
            INPGeneric_081f_e401(r);break;
    }
}

/**
 * @brief      Read a gamepad
 *
 * @param[in]  player  Player number (indexed from zero)
 *
 * @return     Address of gamepad structure.
 */
GAMEPAD *INPReadGamepad(uint8_t player) {
    if (player != 0 || !gp.known) return NULL;                                      // Only 1 gamepad at present, or none attached.
    return &gp;
}

/**
 * @brief      Debugger / Packet analysis of input data from generic device.
 *
 * @param      r     USB Report
 */
static void INPMonitorDevice(USBREPORT *r) {
    if (acceptedPacket) {                                                           // Need something to compare against.
        int compSize = min(r->length,min(lastPacketSize,MAXMONITORPACKETSIZE));     // How much data to compare.
        for (int i = 0;i < compSize;i++) {                                          // Scan for changed data.
            if (r->data[i] != lastPacket[i]) {
                LOG("Offset:%d Was:$%02x [%08b] Now:$%02x [%08b]",i,lastPacket[i],lastPacket[i],r->data[i],r->data[i]);
            }
        }
    }
    lastPacketSize = min(r->length,MAXMONITORPACKETSIZE);                           // Update the last packet record, so we can compare it :)
    acceptedPacket = true; 
    for (int i = 0;i < lastPacketSize;i++) {
        lastPacket[i] = r->data[i];
    }
}

