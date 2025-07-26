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

uint8_t *psRAM = (uint8_t *)PSRAM_LOCATION;

int main() {
    stdio_init_all();
    sleep_ms(1000);
    printf("Starting.\n");
    size_t psramSize = psram_init(8);    
    printf("%d $%x\n",psramSize,psramSize);
    for (int i = 0;i < 8;i++) {
        psRAM[i] = i * 3 + 12;
    }
    for (int i = 0;i < 8;i++) {
        printf("%d\n",psRAM[i]);
    }

    printf("End.\n");
    while (true) {
        printf("Hello, world !\n");
        sleep_ms(500);
        sleep_ms(500);
    }
}   