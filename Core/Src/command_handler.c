/*
 * command_handler.c
 *
 * Created on: 2, November, 2026.
 * Author: Artyom Svetlichnyy
 */

#include "command_handler.h"
#include "cmsis_os2.h"

#define TTC_UART_PERIPHERAL UART7
#define MAXIMUM_COMMAND_LENGTH_BYTES 237
#define RX_EVENT_FLAG 0x01
#define RX_BUFFER_NUMBER_OF_ELEMENTS (sizeof(ring_buffer_Rx) / sizeof(ring_buffer_Rx[0]))

static osThreadId_t command_handle_task;
static uint8_t ring_buffer_Rx[MAXIMUM_COMMAND_LENGTH_BYTES * 3];
static volatile uint16_t rx_buffer_head;	// Controlled by ISR, see below RxEventCallback.
static uint16_t rx_buffer_tail;				// Controlled by task.

void begin_command_handling(UART_HandleTypeDef* huart){
	command_handle_task = osThreadGetId();
	HAL_UARTEx_ReceiveToIdle_DMA(huart, ring_buffer_Rx, RX_BUFFER_NUMBER_OF_ELEMENTS);
}

/*
 * @brief: User implementation of HAL UART receive event call back.
 * @note: Refer to the declaration of the function within HAL drivers.
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	if (huart -> Instance == TTC_UART_PERIPHERAL){
		if (HAL_UARTEx_GetRxEventType(huart) == HAL_UART_RXEVENT_IDLE){
			rx_buffer_head = Size % RX_BUFFER_NUMBER_OF_ELEMENTS;
			osThreadFlagsSet(command_handle_task, RX_EVENT_FLAG);	// Contains portYIELD_FROM_ISR() within.
		}
	}
}

/*
 * @brief: The command interpretation logic.
 */
static void command_interpret(uint8_t* command_array){
	// ToDo: Implement command interpretation.

	uint8_t opcode = command_array[4];

	// 1) Check whether opcode supports payload:
	switch (opcode){
		case (0x02):
				// Doom input from keyboard.
				break;
		case (0x03):
				// Doom input from gamepad.
				break;
	}

	// Everything beyond here does not have a payload.
	// 2) Activate appropriate functionality based on opcode.
}

void command_handle(void){
	osThreadFlagsWait(RX_EVENT_FLAG, osFlagsWaitAny, osWaitForever);	// Clears the flag by default after receiving.
	uint8_t data_byte;
	uint8_t index = 0;
	uint8_t command[MAXIMUM_COMMAND_LENGTH_BYTES];

	// ToDo: We need a way to check whether we are about to read more than one packet.
	while (rx_buffer_tail != rx_buffer_head){
		data_byte = ring_buffer_Rx[rx_buffer_tail];
		rx_buffer_tail = (rx_buffer_tail + 1) % RX_BUFFER_NUMBER_OF_ELEMENTS;
		command[index++] = data_byte;
	}

	command_interpret(command);
}

/*
 * @brief: User implementation of HAL UART error callback.
 * @note: Refer to the declaration of the function within HAL drivers.
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
	// ToDo: Make the error handling more elegant.
	if (huart -> Instance == TTC_UART_PERIPHERAL){
		rx_buffer_head = 0;
		rx_buffer_tail = 0;

		// if there is a problem with UART itself, this will create an undesirable loop of interrupts.
		HAL_UARTEx_ReceiveToIdle_DMA(huart, ring_buffer_Rx, RX_BUFFER_NUMBER_OF_ELEMENTS);
	}
}



