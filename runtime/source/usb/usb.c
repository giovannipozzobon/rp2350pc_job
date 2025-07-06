// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      usb.c
//      Purpose :   USB functions
//      Date :      6th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include <runtime.h>

/**
 * @brief      Initialise the USB system.
 */
void USBInitialise(void) {

}

/**
 * @brief      Update the USB system
 */
void USBUpdate(void) {

}

/**
 * @brief      Add a new packet handler
 *
 * @param[in]  handler  packet handler
 *
 * @return     true if successful.
 */
bool USBInstallHandler(USBHANDLERFUNCTION handler) {
    
}

/**
 * @brief      Dispatch packet to handlers
 *
 * @param      r     Packet to dispatch.
 */
void USBDispatchPacket(USBREPORT *r) {
    printf("%c %04x:%04x ",r->type,r->vid,r->pid);
    for (int i = 0;i < r->length;i++) printf("%02x ",r->data[i]);
    printf("(%d)\n",r->length);
}