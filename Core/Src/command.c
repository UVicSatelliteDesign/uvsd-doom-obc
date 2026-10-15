/**
 * @file		: command.c
 * @brief		: Implementation of command handling functions
 */

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "command_interface.h"
 
/* Placeholder poll period. Tune once the TTC link rate and buffering are known. */
#define COMMAND_POLL_PERIOD_MS 10
 
void handle_command(void *vpParameters) {
	(void)vpParameters;
 
	for(;;) {
		/* Drain every pending command before sleeping */
		while (command_available()) {
			command_t cmd = command_read();
 
			if (!command_is_valid(&cmd)) {
				/* TODO: report invalid command back to the TTC */
				continue;
			}
 
			if (command_is_doom(&cmd)) {
				/* DOOM command: the payload handles it, the OBC only forwards it */
				payload_forward_command(&cmd);
			} else {
				/* OBC command (e.g. send telemetry): the OBC executes it */
				obc_execute_command(&cmd);
			}
		}
 
		/* Yield so lower priority tasks get CPU time */
		vTaskDelay(pdMS_TO_TICKS(COMMAND_POLL_PERIOD_MS));
	}
}
