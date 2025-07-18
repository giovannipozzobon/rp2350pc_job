// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      psram.c
//      Purpose :   PSRAM test program
//      Date :      17th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "stdio.h"
#include "pico/stdlib.h"
#include "rp2_psram.h"

int main() {
    stdio_init_all();
    sleep_ms(1000);
    printf("Starting.\n");
    size_t psramSize = psram_init(8);
    printf("%d %x\n",psramSize,psramSize);
    printf("End.\n");
    while (true) {
        printf("Hello, world !\n");
        sleep_ms(500);
        sleep_ms(500);
    }
}   