// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      serial.c
//      Purpose :   Serial test program
//      Date :      23rd June 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "stdio.h"
#include "pico/stdlib.h"

#define LED_PIN     25  

int main() {
    stdio_init_all();
    sleep_ms(1000);
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    printf("Starting.\n");
    printf("End.\n");
    while (true) {
        printf("Hello, world !\n");
        sleep_ms(500);
        gpio_put(LED_PIN, true);        
        sleep_ms(500);
        gpio_put(LED_PIN, false);        
    }
}   