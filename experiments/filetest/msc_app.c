/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdint.h>
#include "tusb.h"
#include <inttypes.h>
#include "ff.h"
#include "diskio.h"

static FATFS msc_fatfs_volumes[CFG_TUH_DEVICE_MAX];
static volatile bool msc_volume_busy[CFG_TUH_DEVICE_MAX];
static scsi_inquiry_resp_t msc_inquiry_resp;
bool msc_inquiry_complete = false;

/**
 * @brief      Handle a new USB key device
 *
 * @param[in]  dev_addr  dev address
 * @param      cb_data   cb data
 *
 * @return     True if file system mounted now.
 */
bool inquiry_complete_cb(uint8_t dev_addr, tuh_msc_complete_data_t const *cb_data) {
    if (cb_data->csw->status != 0) {
        printf("MSC SCSI inquiry failed\n");
        return false;
    }

    msc_cbw_t const* cbw = cb_data->cbw;
    msc_csw_t const* csw = cb_data->csw;

    // Print out Vendor ID, Product ID and Rev
    printf("%.8s %.16s rev %.4s\r\n", msc_inquiry_resp.vendor_id, msc_inquiry_resp.product_id, msc_inquiry_resp.product_rev);

  // Get capacity of device
  uint32_t const block_count = tuh_msc_get_block_count(dev_addr, cbw->lun);
  uint32_t const block_size = tuh_msc_get_block_size(dev_addr, cbw->lun);

  printf("Disk Size: %" PRIu32 " MB\r\n", block_count / ((1024*1024)/block_size));
  printf("Block Count = %" PRIu32 ", Block Size: %" PRIu32 "\r\n", block_count, block_size);

    char drive_path[3] = "0:";
    drive_path[0] += dev_addr;
    FRESULT result = f_mount(&msc_fatfs_volumes[dev_addr], drive_path, 1);
    if (result != FR_OK) {
        printf("MSC filesystem mount failed\n");
        return false;
    }

    char s[2];
    if (FR_OK != f_getcwd(s, 2)) {
        f_chdrive(drive_path);
        f_chdir("/");
    }

    msc_inquiry_complete = true;

    return true;
}

/**
 * @brief      Mount device
 *
 * @param[in]  dev_addr  The dev address
 */
void tuh_msc_mount_cb(uint8_t dev_addr) {
    uint8_t const lun = 0;
    //CONWriteString("MSC mounted, inquiring\r\n");
    tuh_msc_inquiry(dev_addr, lun, &msc_inquiry_resp, inquiry_complete_cb, 0);
}

/**
 * @brief      Unmount device
 *
 * @param[in]  dev_addr  The dev address
 */
void tuh_msc_umount_cb(uint8_t dev_addr) {
    char drive_path[3] = "0:";
    drive_path[0] += dev_addr;
    f_unmount(drive_path);
}


/**
 * @brief      Fatfs interface , wait for disk IO
 *
 * @param[in]  pdrv  The pdrv
 */
static void wait_for_disk_io(BYTE pdrv) {
    while (msc_volume_busy[pdrv]) {
        tuh_task();
    }
}

/**
 * @brief      Check if disk finished
 *
 * @param[in]  dev_addr  The dev address
 * @param      cb_data   The cb data
 *
 * @return     True if finished
 */
static bool disk_io_complete(uint8_t dev_addr, tuh_msc_complete_data_t const *cb_data) {
    (void)cb_data;
    msc_volume_busy[dev_addr] = false;
    return true;
}

/**
 * @brief      Get Disk status
 *
 * @param[in]  pdrv  The pdrv
 *
 * @return     DSTATUS object
 */
DSTATUS disk_status(BYTE pdrv) {
    uint8_t dev_addr = pdrv;
    return tuh_msc_mounted(dev_addr) ? 0 : STA_NODISK;
}

/**
 * @brief      Initialise a drive
 *
 * @param[in]  pdrv  The pdrv
 *
 * @return     DSTATUS object
 */
DSTATUS disk_initialize(BYTE pdrv) {
    (void)(pdrv);
    return 0;
}

/**
 * @brief      Read disk
 *
 * @param[in]  pdrv    The pdrv
 * @param      buff    The buffer
 * @param[in]  sector  The sector
 * @param[in]  count   The count
 *
 * @return     DRESULT object
 */
DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count) {
    uint8_t const dev_addr = pdrv;
    uint8_t const lun = 0;
    msc_volume_busy[pdrv] = true;
    tuh_msc_read10(dev_addr, lun, buff, sector, (uint16_t)count, disk_io_complete, 0);
    wait_for_disk_io(pdrv);
    return RES_OK;
}

/**
 * @brief      Write to disk
 *
 * @param[in]  pdrv    The pdrv
 * @param[in]  buff    The buffer
 * @param[in]  sector  The sector
 * @param[in]  count   The count
 *
 * @return     DRESULT object
 */
DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count) {
    uint8_t const dev_addr = pdrv;
    uint8_t const lun = 0;
    msc_volume_busy[pdrv] = true;
    tuh_msc_write10(dev_addr, lun, buff, sector, (uint16_t)count, disk_io_complete, 0);
    wait_for_disk_io(pdrv);
    return RES_OK;
}

/**
 * @brief      Handle IODRV commands
 *
 * @param[in]  pdrv  The pdrv
 * @param[in]  cmd   The command
 * @param      buff  The buffer
 *
 * @return     The value requested.
 */
DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff) {
    uint8_t const dev_addr = pdrv;
    uint8_t const lun = 0;
    switch (cmd) {
        case CTRL_SYNC:
            return RES_OK;
        case GET_SECTOR_COUNT:
            *((DWORD *)buff) = (WORD)tuh_msc_get_block_count(dev_addr, lun);
            return RES_OK;
        case GET_SECTOR_SIZE:
            *((WORD *)buff) = (WORD)tuh_msc_get_block_size(dev_addr, lun);
            return RES_OK;
        case GET_BLOCK_SIZE:
            *((DWORD *)buff) = 1;  // 1 sector
            return RES_OK;
        default:
            return RES_PARERR;
    }
}
