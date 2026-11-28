/*
 * command_handler.c
 *
 * Created on: 2, November, 2026.
 * Author: Artyom Svetlichnyy
 */

#include "command_handler.h"
#include "cmsis_os2.h"
#include <string.h>		// Used for memcpy.

#define TTC_UART_PERIPHERAL UART7
#define MAXIMUM_PACKET_LENGTH_BYTES 237
#define RX_BUFFER_LENGTH_BYTES (MAXIMUM_PACKET_LENGTH_BYTES * 4)	// Number of packets chosen experimentally.
#define PACKET_QUEUE_DEPTH 6										// Number of messages chosen experimentally.

typedef struct{
	uint16_t offset;
	uint8_t length_of_packet;
} packet_message_t;

static uint8_t ring_buffer_Rx[RX_BUFFER_LENGTH_BYTES];
static volatile uint16_t last_packet_end_index = 0;
static osMessageQueueId_t packet_queue_Rx;

void begin_command_handling(UART_HandleTypeDef* huart){
	packet_queue_Rx = osMessageQueueNew(PACKET_QUEUE_DEPTH, sizeof(packet_message_t), NULL);
	last_packet_end_index = 0;
	HAL_UARTEx_ReceiveToIdle_DMA(huart, ring_buffer_Rx, RX_BUFFER_LENGTH_BYTES);
	__HAL_DMA_DISABLE_IT(huart -> hdmarx, DMA_IT_HT);
}

/*
 * @brief: User implementation of HAL UART receive event call back.
 * @note: Refer to the declaration of the function within HAL drivers.
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	if (huart -> Instance == TTC_UART_PERIPHERAL){
		if (HAL_UARTEx_GetRxEventType(huart) == HAL_UART_RXEVENT_IDLE){
			uint16_t calculated_packet_length = 0;		// Uses uint16_t to avoid integer overflow in case no idle line.
			if (Size >= last_packet_end_index){
				calculated_packet_length = Size - 1 * last_packet_end_index;
			}
			else{
				// DMA had wrapped around within the ring buffer.
				calculated_packet_length = RX_BUFFER_LENGTH_BYTES - 1 * last_packet_end_index + Size;
			}

			if (calculated_packet_length > 0 && calculated_packet_length <= MAXIMUM_PACKET_LENGTH_BYTES){
				packet_message_t message = {
						.offset = last_packet_end_index,
						.length_of_packet = calculated_packet_length
				};

				osMessageQueuePut(packet_queue_Rx, &message, 0, 0);

			last_packet_end_index = Size;	// Advance even if the message is not queued, packet is ignored.
			}
		}
	}
}

/*
 * @brief: The command interpretation logic.
 */
static void command_interpret(const uint8_t* command_array){
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
	packet_message_t message;
	uint8_t command[MAXIMUM_PACKET_LENGTH_BYTES];

	if (osMessageQueueGet(packet_queue_Rx, &message, NULL, osWaitForever) == osOK){
		if (message.offset + message.length_of_packet <= RX_BUFFER_LENGTH_BYTES){
			memcpy(command, &ring_buffer_Rx[message.offset], message.length_of_packet);
		}
		else{
			// DMA had wrapped around within the ring buffer.
			uint16_t partial_packet_length = RX_BUFFER_LENGTH_BYTES - 1 * message.offset;
			memcpy(command, &ring_buffer_Rx[message.offset], partial_packet_length);
			memcpy(&command[partial_packet_length], ring_buffer_Rx,
					message.length_of_packet - 1 * partial_packet_length);
		}

		command_interpret(command);
	}
}

/*
 * @brief: User implementation of HAL UART error callback.
 * @note: Refer to the declaration of the function within HAL drivers.
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
	// ToDo: Make the error handling more elegant.
	if (huart -> Instance == TTC_UART_PERIPHERAL){
		last_packet_end_index = 0;
		osMessageQueueReset(packet_queue_Rx);	// Unsure whether this works within ISR.

		// If there is a problem with UART itself, this will create an undesirable loop of interrupts.
		HAL_UARTEx_ReceiveToIdle_DMA(huart, ring_buffer_Rx, RX_BUFFER_LENGTH_BYTES);
	}
}



