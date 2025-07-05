// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      common_module.h
//      Purpose :   Common include files
//      Date :      28th June 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#pragma once

#include <stdint.h>
#include <stdio.h>

#ifdef DEBUG
#define LOG   COMPrint
#define ASSERT(x) if (!(x)) ERROR("Assert")
#define ERROR(x) COMError(x,__LINE__,__FILE__)
#else
#define LOG(...)    {}
#define ASSERT(x)   {}
#define ERROR(x)    {}
#endif

void COMInitialise(void);
void COMPrint(char *format,...);
void COMError(char *msg,int line,char *fileName);
uint32_t COMClockMS(void);

#ifdef LOCALS
#ifndef RUNTIME
#include "pico/stdlib.h"
#endif
#endif

#ifdef RUNTIME
#define MAINPROGRAM 
#define YIELD()     SYSYield()
#else
#define MAINPROGRAM main
#define YIELD()     {}
#endif