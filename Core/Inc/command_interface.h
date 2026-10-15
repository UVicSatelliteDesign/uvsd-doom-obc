/**
 * @file		: command_interface.h
 * @brief		: Interface functions called by the handle_command task.
 */

#ifndef INC_COMMAND_INTERFACE_H_
#define INC_COMMAND_INTERFACE_H_

#include <stdbool.h>
#include <stdint.h>

/* Placeholder size and layout. Finalize against the TTC command format. */
#define COMMAND_MAX_LENGTH 64

typedef struct {
	uint8_t id;
	uint8_t length;
	uint8_t data[COMMAND_MAX_LENGTH];
} command_t;

/* Returns true if the TTC has delivered a command that has not been read yet. */
bool command_available(void);

/* Reads the next pending command from the TTC. */
command_t command_read(void);

/* Returns true if the command is well formed and safe to act on. */
bool command_is_valid(const command_t *cmd);

/* Returns true for a DOOM (payload) command, false for an OBC command. */
bool command_is_doom(const command_t *cmd);

/* Passes a DOOM command along to the payload. */
void payload_forward_command(const command_t *cmd);

/* Executes an OBC command (e.g. send telemetry on request). */
void obc_execute_command(const command_t *cmd);

#endif /* INC_COMMAND_INTERFACE_H_ */