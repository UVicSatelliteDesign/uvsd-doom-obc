/**
 * @file		: burnwire.c
 * @brief		: Implementation of burnwire/parachute deployment functions
 */

#include "main.h"
#include "burnwire.h"
#include "obc_interface.h"
#include "telemetry_sensors.h"

// Set from other tasks and ISRs, so volatile
volatile uint8_t trigger_burnwire = 0;
volatile uint8_t deployment_switches = 0;
volatile uint8_t burnwire_started = 0;
volatile uint8_t burnwire_done = 0;

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
	burnwire_started = 1;
	set_burnwire_pin();
	start_long_timer();

	// End task, remaining logic handled in ISRs
	vTaskDelete(NULL);
}

void burnwire_command_callback(uint32_t message){
	// Only an explicit burnwire request may trigger the burnwire
	if (message == (REQUEST | BURNWIRE)){
		trigger_burnwire = 1;
	}
}

void deployment_switches_callback(void){
	deployment_switches = 1;
	// Only shortens a burn in progress. Starting the short timer before the
	// burn would set burnwire_done early, and the long timer would then never
	// turn the burnwire off.
	if (burnwire_started){
		start_short_timer();
	}
}

void short_timer_callback(void){
	// If not already done, turn off the burnwire pin
	if (!burnwire_done){
		reset_burnwire_pin();
		burnwire_done = 1;
	}
}

void long_timer_callback(void){
	// If not already done, turn off the burnwire pin
	if (!burnwire_done){
		reset_burnwire_pin();
		burnwire_done = 1;
	}
}
