/**
 * @file command.h
 * @brief Declarations for forwarding OBC traffic to other boards.
 *
 * COMP-OBC-1: handle commands from TTC and forward relevant commands
 * to onboard subsystems, including game inputs to the payload.
 */

#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Max body size forwarded from TTC: 246-byte payload minus 1 command byte. */
#define CMD_MAX_BODY_LEN 245u

/**
 * @brief Destination of a command received from TTC.
 *
 * ADCS is listed because the command tests still sketch it; balloon
 * hardware does not currently include an ADCS board.
 */
typedef enum {
	SUBSYSTEM_PAYLOAD = 0,
	SUBSYSTEM_ADCS    = 1,
	SUBSYSTEM_EPS     = 2,
	SUBSYSTEM_OBC     = 3,
	SUBSYSTEM_COUNT
} SubsystemID_t;

/**
 * @brief Source of a DOOM game input to be sent to the payload board.
 *
 * Matches COMP-OBC-1 / COMP-GS-2: keyboard and game controller inputs.
 */
typedef enum {
	DOOM_INPUT_KEYBOARD   = 0,
	DOOM_INPUT_CONTROLLER = 1
} DoomInputSource_t;

/**
 * @brief Result of sending data to another board.
 */
typedef enum {
	CMD_OK              =  0,
	CMD_ERR_NULL        = -1,
	CMD_ERR_UNKNOWN     = -2,
	CMD_ERR_INVALID_LEN = -3,
	CMD_ERR_TX          = -4
} CommandStatus_t;

/**
 * @brief A DOOM game command to forward from the OBC to the payload board.
 */
typedef struct {
	DoomInputSource_t source;
	uint8_t command; 
	uint16_t length;
	const uint8_t *data;
} doom_command_t;

/**
 * @brief A command reply to forward from the OBC to TTC for downlink.
 */
typedef struct {
	uint8_t command_id;
	CommandStatus_t status;
	uint16_t length;
	const uint8_t *data;
} command_response_t;

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

#ifdef __cplusplus
}
#endif

#endif /* COMMAND_H */
