#include <stdbool.h>
#include "obc_interface.h"

bool SD_functional = false;
uint8_t SD_read_buffer[_MAX_SS];	// Unsure whether it is needed.
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

FRESULT SD_write_data(){
	/*
	 * Determine the format of writing data onto the SD card.
	 */
}

FRESULT SD_clean(){
	FRESULT SD_clean_result = f_mkfs(SDPath, FM_ANY, 0, SD_write_buffer, sizeof(SD_write_buffer));
	return (SD_clean_result);
}
