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

/**
 * @brief Forwards a DOOM game command to the payload board.
 * @param command Game input to send. Must not be NULL. command->data may be
 *                NULL only when command->length is 0.
 * @return CMD_OK on success, or a CommandStatus_t error code.
 */
CommandStatus_t send_command_to_payload(const doom_command_t *command);

/**
 * @brief Forwards a command response to TTC for transmission to the ground.
 * @param response Reply to send. Must not be NULL. response->data may be
 *                 NULL only when response->length is 0.
 * @return CMD_OK on success, or a CommandStatus_t error code.
 */
CommandStatus_t send_command_response_to_ttc(const command_response_t *response);

/* Executes an OBC command (e.g. send telemetry on request). */
void obc_execute_command(const command_t *cmd);

#endif /* INC_COMMAND_INTERFACE_H_ */