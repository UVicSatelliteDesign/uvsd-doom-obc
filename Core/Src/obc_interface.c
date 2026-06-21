#include <stdbool.h>
#include <string.h>
#include "obc_interface.h"

bool SD_functional = false;
uint8_t SD_read_buffer[_MAX_SS];	// Unsure whether it is needed.
uint8_t SD_write_buffer[_MAX_SS];
UINT bytes_written;					// Needed for FatFS to keep track of bytes written to a file.

FRESULT SD_mount(){
	// Determine the practical difference of setting parameter to 1.
	FRESULT mount_result = f_mount(&SDFatFS, SDPath, 1);

	if (mount_result != FR_OK){
			SD_functional = false;

			if (mount_result == FR_NOT_READY){
					/*
					 * File system object registered successfully but volume not ready for work.
					 * The volume mount process will be attempted on
					 * next file / directory function automatically.
					 */
				}
		}

	/*
	 * Additionally, we could possibly receive FR_NO_FILESYSTEM.
	 * This would indicate that either SD_format was not run first or it had failed.
	 * There could be a possibility for calling SD_format within this function,
	 * but that would put data stored on the SD at risk.
	 */

	return (mount_result);
}

FRESULT SD_dismount(){
	SD_functional = false;
	return (f_mount(NULL, SDPath, 0));
}

FRESULT SD_format(){
	FRESULT format_result = f_mkfs(SDPath, FM_ANY, 0, SD_write_buffer, sizeof(SD_write_buffer));

	if (format_result != FR_OK){
		SD_functional = false;
	}

	return (format_result);
}

FRESULT SD_set_up_directories(){
	FRESULT set_up_result = f_mkdir("Home");

	if (set_up_result != FR_OK){
		SD_functional = false;
		return (set_up_result);
	}

	set_up_result = f_mkdir("Home/Telemetry");
	if (set_up_result != FR_OK){
		SD_functional = false;
		return (set_up_result);
	}

	return (set_up_result);
}

// For calculating data_size of a string, use strlen() as it doesn't count the null terminator!
FRESULT SD_write_data(const uint8_t* data, uint8_t data_size, enum Type type_of_data){
	FRESULT result = f_open(&SDFile, "Home/Telemetry/telemetry.txt", FA_OPEN_APPEND | FA_WRITE);
	if (result != FR_OK){
		SD_functional = false;
		f_close(&SDFile);
		return (result);
	}

	// Check the header file for all types.
	const char* prefix = NULL;
	switch (type_of_data){
		case (TEL_DATA):
			prefix = "DATA: ";
			break;
		case (TEL_WARNING):
			prefix = "WARNING: ";
			break;
		case (TEL_ERROR):
			prefix = "ERROR: ";
			break;
		default:
			prefix = "GENERAL: ";
	}

	result = f_write(&SDFile, prefix, strlen(prefix), &bytes_written);
	if (result != FR_OK || bytes_written == 0){
		SD_functional = false;
		f_close(&SDFile);
		return (result);
	}

	result = f_write(&SDFile, data, data_size, &bytes_written);
	if (result != FR_OK || bytes_written == 0){
		SD_functional = false;
		f_close(&SDFile);
		return (result);
	}

	result = f_write(&SDFile, "\n", 1, &bytes_written);
	if (result != FR_OK || bytes_written == 0){
			SD_functional = false;
			f_close(&SDFile);
			return (result);
	}

	f_close(&SDFile);
	return (result);
}

FRESULT SD_clean(){
	FRESULT SD_clean_result = f_mkfs(SDPath, FM_ANY, 0, SD_write_buffer, sizeof(SD_write_buffer));
	return (SD_clean_result);
}
