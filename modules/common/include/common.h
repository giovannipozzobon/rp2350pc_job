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

#define LOG   COMPrint
#define ASSERT(x) if (!(x)) ERROR("Assert")
#define ERROR(x) COMError(x,__LINE__,__FILE__)

void COMInitialise(void);
void COMPrint(char *format,...);
void COMError(char *msg,int line,char *fileName);
