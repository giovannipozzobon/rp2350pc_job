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
