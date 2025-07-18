// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      file.c
//      Purpose :   File functions
//      Date :      6th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "runtime.h"
#include "usb_module.h"

/**
 * @brief      Open file. The file must already exist, opens in read or write
 *             mode with the file position at the start of the file.
 *
 * @param      fileName  File to open
 *
 * @return     Error code (-ve) Handle (+ve or zero)
 */
int32_t FSOpen(char *fileName) {
    CHECKFSAVAILABLE();                                                             // Storage available ?
    FILE **ppFile;
    if (!FSProcessFileName(&fileName)) return FSERR_BADNAME;                        // Validate name
    int32_t newHandle = FSAllocateRecord(false);                                    // Allocate file record
    if (newHandle < 0) return newHandle;                                            // Failed for some reason (probably too many open)
    int32_t err = FSGetValidateHandle(newHandle,false,(void **)&ppFile);            // Validate the handle and get the file object.
    if (err != 0) LOG("Failure !!");
    *ppFile = fopen(fileName,"rb+");                                                // Open for reading and writing.
    if (*ppFile == NULL) {                                                          // Did it fail ?
        FSFreeRecord(newHandle);                                                    // If so don't need the new record
        return FSMapErrorCode();                                                    // Return error code.
    }
    return newHandle;
}

/**
 * @brief      Read some data from the file
 *
 * @param[in]  handle  File Handle
 * @param      data    Buffer for data
 * @param[in]  size    Amount of data to read in.
 *
 * @return     -ve error code, +ve amount of data actually read in.
 */
int32_t FSRead(int32_t handle,void *data,int32_t size) {
    CHECKFSAVAILABLE();                                                             // Storage available ?
    FILE **ppFile;
    int readBytes;
    int32_t err = FSGetValidateHandle(handle,false,(void **)&ppFile);               // Validate the handle and get the file object.
    if (err != 0) return err;
    readBytes = fread(data,1,size,*ppFile);                                         // Do the read.
    if (readBytes < 0) return FSMapErrorCode();                                     // Failed somehow.
    return (int32_t)readBytes;                                                      // Return the number of bytes read.
}

/**
 * @brief      Write some data to the file
 *
 * @param[in]  handle  File Handle
 * @param      data    Buffer for data
 * @param[in]  size    Amount of data to write out
 *
 * @return     -ve error code, +ve amount of data actually written out.
 */
int32_t FSWrite(int32_t handle,void *data,int32_t size) {
    CHECKFSAVAILABLE();                                                             // Storage available ?
    FILE **ppFile;
    int writeBytes;
    int32_t err = FSGetValidateHandle(handle,false,(void **)&ppFile);               // Validate the handle and get the file object.
    if (err != 0) return err;
    writeBytes = fwrite(data,1,size,*ppFile);                                       // Do the read.
    if (writeBytes < 0) return FSMapErrorCode();                                    // Failed somehow.
    return (int32_t)writeBytes;                                                     // Return the number of bytes written.
}

/**
 * @brief      Move the current file position.
 *
 * @param[in]  handle    File handle
 * @param[in]  position  New position in file.
 *
 * @return     0 or error if -ve
 */
int32_t FSSeek(int32_t handle,int32_t position) {
    CHECKFSAVAILABLE();                                                             // Storage available ?
    FILE **ppFile;
    int32_t err = FSGetValidateHandle(handle,false,(void **)&ppFile);               // Validate the handle and get the file object.
    if (err != 0) return err;
    fseek(*ppFile,position,SEEK_SET);
    return FSMapErrorCode();                                                        // Try to do the move.
}

/**
 * @brief      Get the current file position.
 *
 * @param[in]  handle  File handle
 *
 * @return     Position in the file, error if -ve.
 */
int32_t FSTell(int32_t handle) {
    CHECKFSAVAILABLE();                                                             // Storage available ?
    FILE **ppFile;
    int32_t err = FSGetValidateHandle(handle,false,(void **)&ppFile);               // Validate the handle and get the file object.
    if (err != 0) return err;
    int32_t pos = ftell(*ppFile);                                                   // Return the position or error
    return (pos < 0) ? FSMapErrorCode() : pos;
}

/**
 * @brief      Close the file
 *
 * @param[in]  handle  File Handle
 *
 * @return     0 or error if -ve
 */
int32_t FSClose(int32_t handle) {
    CHECKFSAVAILABLE();                                                             // Storage available ?
    FILE **ppFile;
    int32_t err = FSGetValidateHandle(handle,false,(void **)&ppFile);               // Validate the handle and get the file object.
    if (err != 0) return err;
    FSFreeRecord(handle);                                                           // Free up the handle.
    fclose(*ppFile);
    return FSMapErrorCode();
}
