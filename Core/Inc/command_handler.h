/*
 * command_handler.h
 *
 * Created on: 2, November, 2026
 * Author: Artyom Svetlichnyy
 */

#ifndef INC_COMMAND_HANDLER_H_
#define INC_COMMAND_HANDLER_H_

// SIMPLIFICATION: All commands have an idle pause in between.
// CURRENT PROBLEM: What happens if the command processing task gets blocked by higher priority tasks?

#include <stdint.h>
#include "stm32h7xx_hal.h"

/*
 * @brief: Sets the UART peripheral to listen for commands, serving as an abstraction layer.
 * @param: UART peripheral handle to begin listening for commands.
 */
void begin_command_handling(UART_HandleTypeDef* huart);

/*
 * @brief: The command parsing and interpretation function.
 */
void command_handle(void);

#endif /* INC_COMMAND_HANDLER_H_ */
