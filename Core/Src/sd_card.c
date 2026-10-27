/**
 * @file sd_card.c
 * @brief SD card file access through FatFs, on the USER drive that
 *        MX_FATFS_Init() links (USERFatFS / USERPath from fatfs.h).
 */

#include <stdbool.h>

#include "sd_card.h"
#include "fatfs.h"


static bool sd_mounted = false;


// Turns a FatFs result into the sd_status_t the rest of the OBC uses
static sd_status_t sd_status_from(FRESULT res) {
	switch (res) {
	case FR_OK:
		return SD_OK;
	case FR_NO_FILE:
	case FR_NO_PATH:
		return SD_ERR_NOT_FOUND;
	case FR_INVALID_NAME:
	case FR_INVALID_DRIVE:
		return SD_ERR_INVALID_ARG;
	case FR_DENIED:             // FatFs's answer when there's no room left for a new file
		return SD_ERR_FULL;
	case FR_NOT_ENABLED:
		return SD_ERR_NOT_MOUNTED;
	default:
		return SD_ERR_IO;
	}
}


sd_status_t sd_init(void) {
	FRESULT res = f_mount(&USERFatFS, USERPath, 1);   // 1 = mount now, so a missing card shows up here

	sd_mounted = (res == FR_OK);
	return sd_status_from(res);
}


sd_status_t sd_write(const char *path, const void *data, size_t length, sd_write_mode_t mode) {
	if (path == NULL || path[0] == '\0' || data == NULL || length == 0 ||
	    (mode != SD_WRITE_APPEND && mode != SD_WRITE_OVERWRITE)) {
		return SD_ERR_INVALID_ARG;
	}

	if (!sd_mounted) {
		return SD_ERR_NOT_MOUNTED;
	}

	FIL file;
	FRESULT res = f_open(&file, path, FA_WRITE | (mode == SD_WRITE_APPEND ? FA_OPEN_APPEND : FA_CREATE_ALWAYS));
	if (res != FR_OK) {
		return sd_status_from(res);
	}

	UINT written = 0;
	res = f_write(&file, data, (UINT)length, &written);

	FRESULT close_res = f_close(&file);   // flushes the data to the card
	if (res == FR_OK) {
		res = close_res;
	}

	if (res != FR_OK) {
		return sd_status_from(res);
	}

	// FatFs reports a full card as a short write, not an error
	return (written < length) ? SD_ERR_FULL : SD_OK;
}


sd_status_t sd_read(const char *path, uint32_t offset, void *buffer, size_t length, size_t *bytes_read) {
	if (bytes_read != NULL) {
		*bytes_read = 0;
	}

	if (path == NULL || path[0] == '\0' || buffer == NULL || length == 0) {
		return SD_ERR_INVALID_ARG;
	}

	if (!sd_mounted) {
		return SD_ERR_NOT_MOUNTED;
	}

	FIL file;
	FRESULT res = f_open(&file, path, FA_READ);
	if (res != FR_OK) {
		return sd_status_from(res);
	}

	// In read mode f_lseek stops at the end of the file, so an offset past it reads 0 bytes
	UINT got = 0;
	res = f_lseek(&file, offset);
	if (res == FR_OK) {
		res = f_read(&file, buffer, (UINT)length, &got);
	}

	(void)f_close(&file);   // opened read-only, so there's nothing to flush

	if (bytes_read != NULL) {
		*bytes_read = got;
	}
	return sd_status_from(res);
}


sd_status_t sd_file_size(const char *path, uint32_t *size) {
	if (path == NULL || path[0] == '\0' || size == NULL) {
		return SD_ERR_INVALID_ARG;
	}

	if (!sd_mounted) {
		return SD_ERR_NOT_MOUNTED;
	}

	FILINFO info;
	FRESULT res = f_stat(path, &info);
	if (res != FR_OK) {
		return sd_status_from(res);
	}

	*size = (uint32_t)info.fsize;
	return SD_OK;
}
