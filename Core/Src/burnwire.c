/**
 * @file		: burnwire.c
 * @brief		: Implementation of burnwire/parachute deployment functions
 */

#include "main.h"
#include "obc_interface.h"
#include "telemetry_sensors.h"

#define BURNWIRE_ALTITUDE_LIMIT 23100 // Altitude in meters at which to trigger burnwire
#define BURNWIRE_TIME_LIMIT (55*60) // Time in seconds at which to trigger burnwire

uint8_t trigger_burnwire = 0;
uint8_t deployment_switches = 0;
uint8_t burnwire_done = 0;

void burnwire_parachute(void *vpParameters) {
	float altitude = 0.0;
	uint32_t time = 0;

	do {
		altitude = read_altimeter();
		time = get_flight_time();
		vTaskDelay(1000/portTICK_PERIOD_MS);
	} while (altitude < BURNWIRE_ALTITUDE_LIMIT && time < BURNWIRE_TIME_LIMIT && !trigger_burnwire);

	// One of the conditions for triggering burnwire has been met
	// TODO: notify for telemetry?
	set_burnwire_pin();
	start_long_timer();

	// End task, remaining logic handled in ISRs
	vTaskDelete(NULL);
}

void burnwire_command_callback(){
	trigger_burnwire = 1;
}

void deployment_switches_callback(){
	deployment_switches = 1;
	start_short_timer();
}

void short_timer_callback(){
	// If not already done, turn off the burnwire pin
	if (!burnwire_done){
		reset_burnwire_pin();
		burnwire_done = 1;
	}
}

void long_timer_callback(){
	// If not already done, turn off the burnwire pin
	if (!burnwire_done){
		reset_burnwire_pin();
		burnwire_done = 1;
	}
}
