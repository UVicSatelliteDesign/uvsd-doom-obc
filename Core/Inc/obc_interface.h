#ifndef INC_OBC_INTERFACE_H_
#define INC_OBC_INTERFACE_H_

#include "fatfs.h"

// Types of logged information for telemetry. [Also exists "GENERAL" as default.]
enum Type {
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
 * Brief: Formats the SD card to a FAT type AND creates the directory structure.
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
 * Parameter data_size: The number of characters in data.
 * Parameter type_of_data: The type of data to be written. (UNSURE WHAT TYPES ARE USED.)
 * Return: Response state.
 * Note: For calculating data_size of a string,
 * 		 use strlen() as it doesn't count the null terminator!
 */
FRESULT SD_write_data(const uint8_t* data, uint8_t data_size, enum Type type_of_data);

/*
 * Brief: Completely wipes the formating and data on the SD card.
 * Return: Response state.
 * Note: This function does NOT overwrite all memory.
 * 			It simply makes the data inaccessible, and removes the directory structure.
 */
FRESULT SD_clean();

#endif /* INC_OBC_INTERFACE_H_ */
