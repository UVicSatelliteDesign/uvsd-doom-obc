/**
 * @file sd_card.c
 * @brief Stub implementations for SD card read and write functions.
 *
 * TODO: implement with FatFs (f_mount, f_open, f_lseek, f_read, f_write, f_stat)
 * using USERFatFS / USERPath from fatfs.h.
 */

#include "sd_card.h"


sd_status_t sd_init(void) {
	return SD_OK;
}


sd_status_t sd_write(const char *path, const void *data, size_t length, sd_write_mode_t mode) {
	(void)mode;

	if (path == NULL || path[0] == '\0' || data == NULL || length == 0) {
		return SD_ERR_INVALID_ARG;
	}

	return SD_OK;
}


sd_status_t sd_read(const char *path, uint32_t offset, void *buffer, size_t length, size_t *bytes_read) {
	(void)offset;

	if (bytes_read != NULL) {
		*bytes_read = 0;
	}

	if (path == NULL || path[0] == '\0' || buffer == NULL || length == 0) {
		return SD_ERR_INVALID_ARG;
	}

	return SD_OK;
}


sd_status_t sd_file_size(const char *path, uint32_t *size) {
	if (path == NULL || path[0] == '\0' || size == NULL) {
		return SD_ERR_INVALID_ARG;
	}

	*size = 0;
	return SD_OK;
}
