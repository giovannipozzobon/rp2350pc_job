// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      file.c
//      Purpose :   File functions
//      Date :      27th June 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#define LOCALS
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
    FIL *pFile;
    if (!FSProcessFileName(&fileName)) return FSERR_BADNAME;                        // Validate name
    int32_t newHandle = FSAllocateRecord(false);                                    // Allocate file record
    if (newHandle < 0) return newHandle;                                            // Failed for some reason (probably too many open)
    int32_t err = FSGetValidateHandle(newHandle,false,(void **)&pFile);             // Validate the handle and get the file object.
    if (err != 0) LOG("Failure !!");
    FRESULT fr = f_open(pFile,fileName,FA_READ|FA_WRITE);                           // Open for reading and writing.
    if (fr != FR_OK) {                                                              // Did it fail ?
        FSFreeRecord(newHandle);                                                    // If so don't need the new record
        return FSMapErrorCode(fr);                                                  // Return error code.
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
    FIL *pFile;
    UINT readBytes;
    int32_t err = FSGetValidateHandle(handle,false,(void **)&pFile);                // Validate the handle and get the file object.
    if (err != 0) return err;
    FRESULT fr = f_read(pFile,data,size,&readBytes);                                // Do the read.
    if (fr != FR_OK) return FSMapErrorCode(fr);                                     // Failed somehow.
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
    FIL *pFile;
    UINT writeBytes;
    int32_t err = FSGetValidateHandle(handle,false,(void **)&pFile);                // Validate the handle and get the file object.
    if (err != 0) return err;
    FRESULT fr = f_write(pFile,data,size,&writeBytes);                              // Do the write.
    if (fr != FR_OK) return FSMapErrorCode(fr);                                     // Failed somehow.
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
    FIL *pFile;
    int32_t err = FSGetValidateHandle(handle,false,(void **)&pFile);                // Validate the handle and get the file object.
    if (err != 0) return err;
    return FSMapErrorCode(f_lseek(pFile,position));                                 // Try to do the move.
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
    FIL *pFile;
    int32_t err = FSGetValidateHandle(handle,false,(void **)&pFile);                // Validate the handle and get the file object.
    if (err != 0) return err;
    int32_t pos = f_tell(pFile);                                                    // Return the position or error
    return (pos < 0) ? FSMapErrorCode(pos) : pos;
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
    FIL *pFile;
    int32_t err = FSGetValidateHandle(handle,false,(void **)&pFile);                // Validate the handle and get the file object.
    if (err != 0) return err;
    FSFreeRecord(handle);                                                           // Free up the handle.
    return FSMapErrorCode(f_close(pFile));
}
