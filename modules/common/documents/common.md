# Common Support Module

## Required Modules 

None.

## Purpose

This provides logging, assertion and error functionality for all modules, and general functionality that is used across modules.

## Further Modules

N/A

## Usage Notes

There are three macros

- LOG(message) displays a message on the log (serial port at present)
- ERROR(message) displays an error message
- ASSERT(expression) displays an error message if the expression is false.

System functions are 

- uint32_t COMTimeMS(void) returns the number of milliseconds, which should not be assumed to be zero at boot. This is not a high precision timer, a resolution of about 50Hz or so is about right.


## Revision

Written by Paul Robson, last revised 14 July 2025.







