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

void payload_forward_command(const command_t *cmd) {
	/* TODO: send the command to the payload */
	(void)cmd;
}

void obc_execute_command(const command_t *cmd) {
	/* TODO: execute the OBC command (e.g. send telemetry) */
	(void)cmd;
}