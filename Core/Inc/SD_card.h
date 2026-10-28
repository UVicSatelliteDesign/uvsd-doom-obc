/*
 * SD_card.h
 *
 *  Created on: 17, October, 2026.
 *  Author: Artyom Svetlichnyy.
 */

/*
 * Usage Guide:
 * 1) Mount the SD card volume through SD_mount().
 * 2) Format the SD card through SD_format(), but ideally should
 *    be formatted prior to usage. Refer to @note on SD_format() for the reason.
 * 3) Create the directory structure using SD_set_up_direcoties().
 * 4) Verify the SD card set up using SD_verify_set_up() in order to get
 *    access to usage of SD_write_log_entry().
 * 5) Use SD_write_log_entry(*arguments*) to write log entries into log.txt.
 * 6) Use SD_retrieve_wad_file() to send doom.wad over to its destination.
 * 7) Dismount the SD card when its functionality is no longer needed.
 *
 * Note:
 * SD_retrieve_wad_file() is yet to be fully implemented; marked as TODO.
 */

#ifndef INC_SD_CARD_H_
#define INC_SD_CARD_H_

#include <stdbool.h>
#include <string.h>
#include "fatfs.h"

enum information_type {
	OBC_GENERAL,
	OBC_DATA,
	OBC_WARNING,
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
 * @note: This function needs a larger task stack size than other SD functions! (Works with 2048 words.)
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
