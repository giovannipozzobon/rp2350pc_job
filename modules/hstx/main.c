// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   HSTX module test program
//      Date :      25th June 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "dvi_system.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"

uint8_t framebuffer[640*480];
uint8_t pixelsPerByte = 1;

int main() {
    stdio_init_all();
    DVISetup(pixelsPerByte,framebuffer);
    for (int x = 0;x < 640;x++) {
        for (int y = 0;y < 480;y++) {
            uint8_t pixel = 0;
            uint16_t yCol = (x/16+y/16) & 15;
            if (pixelsPerByte == 1) {
                if (yCol & 1) pixel |= 0xE0;
                if (yCol & 2) pixel |= 0x1C;
                if (yCol & 4) pixel |= 0x03;
            }
            if (pixelsPerByte == 2) {
                if (yCol & 1) pixel |= 0x88;
                if (yCol & 2) pixel |= 0x44;
                if (yCol & 4) pixel |= 0x22;
            }
            if (pixelsPerByte == 8) {
                if (yCol & 1) pixel |= 0xFF;
            }

            if (y >= 128 || (x & 1)) {
                framebuffer[x+y*640] = pixel;
            }
        }
    }
    while (1) {
            printf("Ping !\n");
            sleep_ms(500);
        __wfi();
    }
    return 0;
}
