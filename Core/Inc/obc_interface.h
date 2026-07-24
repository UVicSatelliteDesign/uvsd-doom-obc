#ifndef INC_OBC_INTERFACE_H_
#define INC_OBC_INTERFACE_H_

#include "fatfs.h"

// Types of logged information for telemetry.
enum Type {
	TEL_GENERAL,
	TEL_DATA,
	TEL_WARNING,
	TEL_ERROR
};

/*
 * Brief: Mounts the SD card.
 * Return: Response state.
 */
FRESULT SD_mount();

/*
 * Brief: Dismounts the SD card,
 * Return: Response state.
 */
FRESULT SD_dismount();

/*
 * Brief: Formats the SD card (losing data) into a FAT type chosen by FatFS.
 * 		  Not needed for set up of SD card if it is already of the right type.
 * Return: Response state.
 */
FRESULT SD_format();

/*
 * Brief: Creates directories within a configured FAT file system.
 * Return: Response state.
 */
FRESULT SD_set_up_directories();

/*
 * Brief: Writes given data onto a mounted SD card.
 * Parameter data: Pointer to the data.
 * Parameter type_of_data: The type of data to be written. (UNSURE WHAT TYPES ARE USED.)
 * Return: Response state.
 */
FRESULT SD_write_data(const uint8_t* data, uint8_t data_size, enum Type type_of_data);

/*
 * Brief: Checks that everything is set up correctly and can set SD_functional to true.
 * Return: The new boolean of SD_functional.
 */
bool SD_verify_state();

#endif /* INC_OBC_INTERFACE_H_ */
