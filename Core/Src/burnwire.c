/**
 * @file		: burnwire.c
 * @brief		: Implementation of burnwire/parachute deployment functions
 */

#include "main.h"
#include "obc_interface.h"

#define BURNWIRE_ALTITUDE_LIMIT 23100 // Altitude in meters at which to trigger burnwire
#define BURNWIRE_TIME_LIMIT (55*60) // Time in seconds at which to trigger burnwire

void burnwire_parachute(void *vpParameters) {
	float altitude = 0.0;
	uint32_t time = 0;
	uint32_t received_notification = 0;

	do {
		altitude = read_altitude();
		time = get_flight_time();
		received_notification = ulTaskNotifyTake(pdFALSE, 0);
	} while (altitude < BURNWIRE_ALTITUDE_LIMIT || time < BURNWIRE_TIME_LIMIT || !(received_notification & REQUEST & BURNWIRE));

	// One of the conditions for triggering burnwire has been met
	xTaskNotify(telemetryTaskHandle, INFO & BURNWIRE, eSetValueWithOverwrite); // TODO: which task if any should this notify?
	set_burnwire_pin();
	start_long_timer();

	do {
		received_notification = ulTaskNotifyTake(pdFALSE, 0);
		if (received_notification & INFO & DEPLOYMENT_SWITCHES) {
			start_short_timer();
		}
	} while (!(received_notification & INFO & LONG_TIMER) || !(received_notification & INFO & SHORT_TIMER));

	// One of the timers has expired; turn off the burnwire pin
	HAL_GPIO_WritePin(PARA_BRN_1_GPIO_Port, PARA_BRN_1_Pin, GPIO_PIN_RESET);

	// End task
	vTaskDelete(NULL);
}
