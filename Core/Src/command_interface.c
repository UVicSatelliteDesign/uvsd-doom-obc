/**
 * @file		: command_interface.c
 * @brief		: Stub implementations of the command handling interface.
 *				  TODO: implement these.
 */

#include "command_interface.h"

bool command_available(void) {
	/* TODO: check for a received command from the TTC */
	return false;
}

command_t command_read(void) {
	/* TODO (CompDB15): read the next command from the TTC */
	command_t cmd = {0};
	return cmd;
}

bool command_is_valid(const command_t *cmd) {
	/* TODO: validate the command */
	(void)cmd;
	return true;
}

bool command_is_doom(const command_t *cmd) {
	/* TODO: decide whether this is a DOOM or OBC command */
	(void)cmd;
	return false;
}

CommandStatus_t send_command_to_payload(const doom_command_t *command) {
	(void)command;
	/* TODO: Transmit game-input bytes to the payload board. */
	return CMD_OK;
}

CommandStatus_t send_command_response_to_ttc(const command_response_t *response) {
	(void)response;
	/* TODO: Transmit response bytes to TTC for downlink. */
	return CMD_OK;
}

void obc_execute_command(const command_t *cmd) {
	/* TODO: execute the OBC command (e.g. send telemetry) */
	(void)cmd;
}