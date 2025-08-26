// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      tracker.c
//      Purpose :   Memory usage tracker
//      Date :      2nd August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "memory_module.h"
#include "memory_module_local.h"


/**
 * @brief      Initialise the block tracker structure.
 *
 * @param      tracker  The structure to initialise
 * @param      address  Address of the memory block
 * @param[in]  size     Size of the memory block.
 */
void MEMInitialiseBlock(MEMORYTRACKER *tracker,uint8_t *address,uint32_t size) {
    tracker->baseAddress = address;                                                 // Set memory location and range
    tracker->totalSize = size;
    tracker->blockSize = (size / MAXBLOCKS) & 0xFFFFFFFC;                           // Size of one block. Force to word size.
    tracker->blockCount = size / tracker->blockSize;                                // Number of complete blocks available.
    for (int i = 0;i < tracker->blockCount;i++) MEMSetUsedFlag(tracker,i,false);    // Mark all blocks unused.
}

/**
 * @brief      Check if a memory block is in use.
 *
 * @param      tracker  Usage tracker
 * @param[in]  blockID  Block ID
 *
 * @return     true if block in use.
 */
bool MEMBlockInUse(MEMORYTRACKER *tracker,uint32_t blockID) {
    return tracker->blockUsed[blockID];
}

/**
 * @brief      Set a block in the usage tracker's state
 *
 * @param      tracker  Usage tracker
 * @param[in]  blockID  Block ID
 * @param[in]  inUse    True if in use, e.g. allocated.
 */
void MEMSetUsedFlag(MEMORYTRACKER *tracker,uint32_t blockID,bool inUse) {
    tracker->blockUsed[blockID] = inUse;
}
