// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      input_module.h
//      Purpose :   Input Manager System Header files
//      Date :      2nd July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

#include "common_module.h"
#include "usb_module.h"

void INPInitialise(void);
void INPUpdate(void);
bool *INPGetKeyboardState(void);
bool INPSetLocale(char *locale);
int16_t INPGetKey(void);
bool INPIsKeyAvailable(void);
void INPGetMouseStatus(int16_t *px,int16_t *py,int16_t *pButtons);
INPGAMEPAD *INPReadGamepad(uint8_t player);

