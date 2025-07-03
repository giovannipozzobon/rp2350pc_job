// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      support.c
//      Purpose :   File I/O Support routines
//      Date :      27th June 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#define LOCALS
#include "usb_module.h"

#define MAXFILESDIRS    (8)                                                         // Max # of files/directories open.

//
//      Each represents a file or directory.
//
struct _filedirRecord {
    enum { Unused, File, Directory } type;                                          // What each record holds.
    union {
        FIL     file;                                                               // FatFS file structure
        DIR     directory;                                                          // Directory object structure.
    } ff;
} fsObject[MAXFILESDIRS];

/**
 * @brief      Initialise the file system wrapper.
 */
void FSInitialise(void) {
    for (int i = 0;i < MAXFILESDIRS;i++) fsObject[i].type = Unused;                 // Set all types to unused.
}

/**
 * @brief      Validate a handle and convert it to the appropriate form (DIR or
 *             FIL) as a void.
 *
 * @param[in]  handle       File or Directory Handle
 * @param[in]  isDirectory  if true, expecting a directory, if false, expecting
 *                          a file.
 * @param      fsObjectPtr  pointer to a pointer to store the FIL or DIR
 *                          reference.
 *
 * @return     Error Code or 0
 */
int32_t FSGetValidateHandle(int32_t handle, bool isDirectory,void **fsObjectPtr) {
    if (handle < 0 || handle >= MAXFILESDIRS) return FSERR_BADHANDLE;               // Handle out of range.
    if (fsObject[handle].type == Unused) return FSERR_BADHANDLE;                    // Not actually in use
    if (fsObject[handle].type != (isDirectory ? Directory:File)) return FSERR_TYPE; // Wrong type of handle.
    *fsObjectPtr = (void *)&(fsObject[handle].ff);                                  // Return pointer to file/directory part.
    return 0;
}

/**
 * @brief      Map a FATFS Error code onto a USB Module one.
 *
 * @param[in]  res   result of FATFS operation
 *
 * @return     Error code (or 0 if no error)
 */
int32_t FSMapErrorCode(FRESULT res) {
    int code = FSERR_SYSTEM;                                                        // Default for all the odd stuff.
    switch(res) {
        case FR_OK:
            code = 0;break;
        case FR_NO_FILE:             
        case FR_NO_PATH:             
        case FR_EXIST:               
            code = FSERR_EXIST;break;

        case FR_INVALID_NAME:        
            code = FSERR_BADNAME;break;

        case FR_DENIED:              
        case FR_WRITE_PROTECTED:     
        case FR_LOCKED:
            code = FSERR_PROTECTED;break;
        }
    return code;
}

/**
 * @brief      Preprocess a file or directory name
 *
 * @param      pFileName  pointer to the file name string.
 *
 * @return     true if valid.
 */
bool FSProcessFileName(char **pFileName) {
    return true;
}

/**
 * @brief      Allocate a record to be used to hold a file or directory object
 *
 * @param[in]  isDirectory  true for directory, false for file
 *
 * @return     handle of the record to used or -1 if none available.
 */
int32_t FSAllocateRecord(bool isDirectory) {
    for (int32_t i = 0;i < MAXFILESDIRS;i++) {
        if (fsObject[i].type == Unused) {
            fsObject[i].type = isDirectory ? Directory : File;
            return i;
        }
    }
    return -1;
}

/**
 * @brief      Free a record in the file/directory structure.
 *
 * @param[in]  handle  The handle
 */
void FSFreeRecord(uint32_t handle) {
    fsObject[handle].type = Unused;
}

