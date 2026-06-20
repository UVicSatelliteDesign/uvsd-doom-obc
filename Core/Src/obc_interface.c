#include <stdbool.h>
#include "obc_interface.h"

bool SD_functional = false;
uint8_t SD_read_buffer[_MAX_SS];
uint8_t SD_write_buffer[_MAX_SS];

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

	return (mount_result);
}

FRESULT SD_dismount(){
	SD_functional = false;
	return (f_mount(NULL, SDPath, 0));
}

FRESULT SD_format(){
	/*
	 * Determine what format the SD card should follow.
	 */
}

FRESULT SD_write_data(){
	/*
	 * Determine the format of writing data onto the SD card.
	 */
}

FRESULT SD_clean(){
	FRESULT SD_clean_result = f_mkfs(SDPath, 0, _MAX_SS, SD_write_buffer, sizeof(SD_write_buffer));
	return (SD_clean_result);
}
