/*
 * SD_card.h
 *
 *  Created on: 17, October, 2026.
 *  Author: Artyom Svetlichnyy.
 */

#ifndef INC_SD_CARD_H_
#define INC_SD_CARD_H_

#include <stdbool.h>
#include <string.h>
#include "fatfs.h"

enum information_type {
	GENERAL,
	DATA,
	WARNING,
	OBC_ERROR
};


/*
 * @brief: Mounts the SD card volume.
 * @return: The FRESULT enum from the operation.
 */
FRESULT SD_mount(void);

/*
 * @brief: Dismounts the SD card volume.
 * @return: The FRESULT enum from the operation.
 */
FRESULT SD_dismount(void);

/*
 * @brief: Forcefully creates a new file system (chosen by FatFS) on the SD card.
 * @return: The FRESULT enum from the operation.
 */
FRESULT SD_format(void);

/*
 * @brief: Sets up the needed directories for SD card operation.
 * @return: The FRESULT enum from the operation.
 */
FRESULT SD_set_up_directories(void);

/*
 * @brief: Writes a message into the SD card's log.txt file.
 * @param: Pointer to the data to be written.
 * @param: The size of the data.
 * @param: The type of data it is defined by the information_type enum.
 * @return: The FRESULT enum from the operation.
 */
FRESULT SD_write_log_entry(uint8_t* data, uint8_t data_size, enum information_type type_of_data);

/*
 * @brief: Retrieves the Doom WAD file from the SD card.
 * @return: The FRESULT enum from the operation.
 */
FRESULT SD_retrieve_wad_file();

/*
 * @note: Running this function is required before calling any write functions to the SD card!
 * @brief: Verifies the SD card is ready for operation & enables the global SD_functional flag.
 * @return: Whether the SD card is set up correctly for operation.
 */
bool SD_verify_set_up(void);

#endif /* INC_SD_CARD_H_ */
