// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      filedirect.c
//      Purpose :   Operations on complete files/directories (e.g. no open/close)
//      Date :      27th June 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#define LOCALS
#include "common_module.h"
#include "usb_module.h"

/**
 * @brief      Create a new file, deleting the old one if it exists.
 *
 * @param      fileName  File Name to create
 *
 * @return     error code or 0
 */
int32_t FSCreate(char *fileName) {
    FIL fil;
    if (!FSProcessFileName(&fileName)) return FSERR_BADNAME;
    FSDelete(fileName);                                                             // Delete any existing file.
    FRESULT res = f_open(&fil,fileName,FA_CREATE_ALWAYS);                           // Try to create it.
    if (res != FR_OK) return FSMapErrorCode(res);                                   // We couldn't for some reason.
    f_close(&fil);                                                                  // Close it straight away.
    return 0;
}

/**
 * @brief      Delete a file if it exists.
 *             
 * @param      fileName  File Name to delete
 *
 * @return     0 
 */
int32_t FSDelete(char *fileName) {
    if (!FSProcessFileName(&fileName)) return FSERR_BADNAME;
    f_unlink(fileName);
    return 0;
}

/**
 * @brief      Create a directory, ignored if it already exists.
 *
 * @param      dirName  Directory to create
 *
 * @return     error code or 0.
 */
int32_t FSCreateDirectory(char *dirName) {
    if (!FSProcessFileName(&dirName)) return FSERR_BADNAME;
    FRESULT res = f_mkdir(dirName);
    if (res  == FR_EXIST) return 0;                                                     // Ignore exist errors.
    return FSMapErrorCode(res);
}

/**
 * @brief      Delete a directory if it exists
 *
 * @param      dirName  Directory to delete
 *
 * @return     error code or 0.
 */
int32_t FSDeleteDirectory(char *dirName) {
    if (!FSProcessFileName(&dirName)) return FSERR_BADNAME;
    f_unlink(dirName);
    return 0;
}

