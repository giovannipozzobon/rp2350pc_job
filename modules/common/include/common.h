// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      common.h
//      Purpose :   Common include files
//      Date :      28th June 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#pragma once

#define LOG   printf
#define ASSERT(x) if (!(x)) ERROR("Assert")
#define ERROR(x) printf("Error %s at (%s):%d",x,__FILE__,__LINE__)

#define COMInitialise()    stdio_init_all()
