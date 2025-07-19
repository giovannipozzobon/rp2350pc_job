// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      altcore_module.h
//      Purpose :   External definition for altcore
//      Date :      19th July 2025
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

#include "common_module.h"
#include "usb_module.h"
#include "dvi_module.h"
#include "modes_module.h"
#include "graphics_module.h"
#include "input_module.h"

typedef void (*CORVSYNCHANDLER)(bool initialise); 

void CORInitialise(void);
void CORStart(void);
void CORStop(void);
void CORAdd(CORVSYNCHANDLER handler);

void VerticalSyncRoutine(bool initialise);
