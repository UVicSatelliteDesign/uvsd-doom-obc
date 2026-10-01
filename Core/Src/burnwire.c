/**
 * @file		: burnwire.c
 * @brief		: Implementation of burnwire/parachute deployment functions
 */

#include "main.h"
#include "obc_interface.h"
#include "telemetry_sensors.h"

#define BURNWIRE_ALTITUDE_LIMIT 23100 // Altitude in meters at which to trigger burnwire
#define BURNWIRE_TIME_LIMIT (55*60) // Time in seconds at which to trigger burnwire

void burnwire_parachute(void *vpParameters) {
	float altitude = 0.0;
	uint32_t time = 0;
	uint32_t received_notification = 0;

	do {
		altitude = read_altimeter();
		time = get_flight_time();
		received_notification = ulTaskNotifyTake(pdFALSE, 0);
		vTaskDelay(1000/portTICK_PERIOD_MS);
	} while (altitude < BURNWIRE_ALTITUDE_LIMIT || time < BURNWIRE_TIME_LIMIT || !(received_notification & REQUEST & BURNWIRE));

	// One of the conditions for triggering burnwire has been met
	//xTaskNotify(xTaskGetHandle("TASK_NAME"), INFO & BURNWIRE, eSetBits); // TODO: which task if any should this notify?
	set_burnwire_pin();
	start_long_timer();

	do {
		received_notification = ulTaskNotifyTake(pdFALSE, 0);
		if (received_notification & INFO & DEPLOYMENT_SWITCHES) {
			start_short_timer();
		}
		vTaskDelay(1000/portTICK_PERIOD_MS);
	} while (!(received_notification & INFO & LONG_TIMER) || !(received_notification & INFO & SHORT_TIMER));

	// One of the timers has expired; turn off the burnwire pin
	reset_burnwire_pin();

	// End task
	vTaskDelete(NULL);
}
