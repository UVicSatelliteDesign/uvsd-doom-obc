/*
 * SD_card.c
 *
 *  Created on: 17, October, 2026.
 *  Author: Artyom Svetlichnyy.
 */

#include "SD_card.h"
#include <string.h>

bool SD_functional = false;		// Run SD_verify_set_up to change.
uint8_t SD_buffer[_MAX_SS];		// Buffer has to be word aligned according to STM32 RM0433 (page 2409). [NO D-CACHE ENABLED!]

FRESULT SD_mount(void){
	return (f_mount(&SDFatFS, SDPath, 1));
}

FRESULT SD_dismount(void){
	SD_functional = false;
	return (f_mount(NULL, SDPath, 1));
}

FRESULT SD_format(void){
	return (f_mkfs(SDPath, FM_ANY, 0, SD_buffer, sizeof(SD_buffer)));
}

FRESULT SD_set_up_directories(void){
	FRESULT result = f_mkdir("LOG");
	if (result != FR_OK && result != FR_EXIST){
		return (result);
	}

	result = f_mkdir("WAD");
	return ((result == FR_EXIST) ? FR_OK : result);
}

FRESULT SD_write_log_entry(uint8_t* data, uint8_t data_size, enum information_type type_of_data){
	if (!SD_functional){
		return (FR_NOT_READY);
	}

	FRESULT result = f_open(&SDFile, "LOG/log.txt", FA_OPEN_APPEND | FA_WRITE);
	if (result != FR_OK){
		return (result);
	}

	const char* entry_prefix = NULL;
	switch (type_of_data){
		case (OBC_DATA):
			entry_prefix = "DATA: ";
			break;
		case (OBC_WARNING):
			entry_prefix = "WARNING: ";
			break;
		case (OBC_ERROR):
			entry_prefix = "ERROR: ";
			break;
		default:
			entry_prefix = "GENERAL: ";
	}

	UINT bytes_written = 0;
	result = f_write(&SDFile, entry_prefix, strlen(entry_prefix), &bytes_written);
	if (result != FR_OK || bytes_written == 0){
		f_close(&SDFile);
		return (result);
	}

	result = f_write(&SDFile, data, data_size, &bytes_written);
	if (result != FR_OK || bytes_written == 0){
		f_close(&SDFile);
		return (result);
	}

	result = f_write(&SDFile, "\n", 1, &bytes_written);
	f_close(&SDFile);

	return (result);
}

FRESULT SD_retrieve_wad_file(){
	FRESULT result = f_open(&SDFile, "WAD/doom.wad", FA_READ);
	if (result == FR_OK){
		UINT bytes_read = 0;
		while (1){

			result = f_read(&SDFile, SD_buffer, sizeof(SD_buffer), &bytes_read);
			if (result != FR_OK || bytes_read == 0){
				break;	// Could potentially just be the end of file.
			}

			// TODO : Something has to accept the buffer contents within the loop!

		}
	}

	f_close(&SDFile);
	return (result);
}

bool SD_verify_set_up(void){
	// Check that the SD is mounted:
	if (SDFatFS.fs_type == 0){
		return (false);
	}

	// Check reading by checking directories:
	DIR directory;
	FRESULT result = f_opendir(&directory, "Log");
	f_closedir(&directory);
	if (result != FR_OK){
		return (false);
	}

	// Write into the log that the SD card was verified:
	SD_functional = true;		// Prematurely set to true to test writing functionality.
	uint8_t log_entry[] = "SD card status verification.";
	result = SD_write_log_entry(log_entry, sizeof(log_entry) , OBC_GENERAL);
	if (result != FR_OK){
		SD_functional = false;
		return (false);
	}

	return (true);
}


