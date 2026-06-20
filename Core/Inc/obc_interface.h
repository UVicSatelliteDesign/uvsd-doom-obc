#ifndef INC_OBC_INTERFACE_H_
#define INC_OBC_INTERFACE_H_

#include "fatfs.h"

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
 * Brief: Formats the SD card (PROVIDE MORE DETAIL.)
 * Return: Response state.
 */
FRESULT SD_format();

/*
 * Brief: Writes given data onto a mounted SD card.
 * Parameter data: Pointer to the data.
 * Parameter data_length: The number of characters in data.
 * Parameter type_of_data: The type of data to be written. (UNSURE WHAT TYPES ARE USED.)
 * Return: Response state.
 */
FRESULT SD_write_data();

/*
 * Brief: Completely wipes the formating and data on the SD card.
 * Return: Response state.
 * Note: This function does NOT overwrite all memory.
 * 			It simply makes the data inaccessible, and removes the directory structure.
 */
FRESULT SD_clean();

#endif /* INC_OBC_INTERFACE_H_ */
