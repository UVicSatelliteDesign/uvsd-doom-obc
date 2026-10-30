/**
 * @file sd_card.h
 * @brief Simple functions for reading and writing files on the SD card.
 */


#ifndef SD_CARD_H
#define SD_CARD_H

#include <stddef.h>
#include <stdint.h>


// What every SD function gives back so you know if it worked

typedef enum {
	SD_OK = 0,              // all good
	SD_ERR_INVALID_ARG,     // passed a NULL pointer, an empty path, or a length of 0
	SD_ERR_NOT_MOUNTED,     // sd_init() hasn't worked yet
	SD_ERR_NOT_FOUND,       // that file or folder doesn't exist
	SD_ERR_FULL,            // the card's out of space
	SD_ERR_IO               // something went wrong with the card or file system
} sd_status_t;


// What sd_write() should do if the file's already there

typedef enum {
	SD_WRITE_APPEND,        // tack the data onto the end (makes the file if it's not there)
	SD_WRITE_OVERWRITE      // wipe the file and start fresh (makes the file if it's not there)
} sd_write_mode_t;


/**
 * @brief Mounts the SD card file system. Must be called once before any other SD function.
 * @return SD_OK on success, otherwise an error code.
 */

sd_status_t sd_init(void);


/**
 * @brief Writes a block of data to a file on the SD card.
 * @param path Path of the file to write.
 * @param data Data to write.
 * @param length Number of bytes to write.
 * @param mode Append to or overwrite the existing file.
 * @return SD_OK if all bytes were written, otherwise an error code.
 */

sd_status_t sd_write(const char *path, const void *data, size_t length, sd_write_mode_t mode);


/**
 * @brief Reads a block of data from a file on the SD card.
 * @param path Path of the file to read.
 * @param offset Byte position in the file to start reading from.
 * @param buffer Destination buffer.
 * @param length Maximum number of bytes to read (size of buffer).
 * @param bytes_read Set to the number of bytes actually read, which is less than
 *                   length if the end of the file was reached. May be NULL.
 * @return SD_OK on success, otherwise an error code.
 */

sd_status_t sd_read(const char *path, uint32_t offset, void *buffer, size_t length, size_t *bytes_read);


/**
 * @brief Gets the size of a file on the SD card.
 * @param path Path of the file.
 * @param size Set to the file size in bytes.
 * @return SD_OK on success, otherwise an error code.
 */

sd_status_t sd_file_size(const char *path, uint32_t *size);

#endif 
