/**
 * @file		: command.c
 * @brief		: Implementation of command handling functions
 */

#include "main.h"
#include "command.h"

void handle_command(void *vpParameters) {
	/* TODO: Implement command handling */
	for(;;) {}
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
