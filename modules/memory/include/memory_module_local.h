// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      memory_module_local.h
//      Purpose :   Memory manager internal header
//      Date :      2nd August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************


#pragma once

#define MAXBLOCKS   (256)                                                           // Fixed # of blocks for now.

typedef struct _MemoryUsageTracker {
    uint8_t     *baseAddress;                                                       // Base address of memory.
    uint32_t    totalSize;                                                          // Total size in bytes
    uint32_t    blockCount;                                                         // Count of blocks here.
    uint32_t    blockSize;                                                          // Size of each blocks.
    bool        blockUsed[MAXBLOCKS];                                               // Usage flag for each block.
} MEMORYTRACKER;

void MEMInitialiseBlock(MEMORYTRACKER *tracker,uint8_t *address,uint32_t size);
bool MEMBlockInUse(MEMORYTRACKER *tracker,uint32_t blockID);
void MEMSetUsedFlag(MEMORYTRACKER *tracker,uint32_t blockID,bool inUse);

#ifndef RUNTIME
#endif
