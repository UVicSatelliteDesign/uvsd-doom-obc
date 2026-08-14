/**
 * @file		: obc_interface.c
 * @brief		: Interface functions for sensors etc.
 */

#include "obc_interface.h"

#define BURNWIRE_LONG_TIME 120 // TODO: Set to actual time in seconds
#define BURNWIRE_SHORT_TIME 60 // TODO: Set to actual time in seconds

// Should return current altitude in meters
float read_altitude() {
	return 0.0;
}

// Should return time since launch in seconds
uint32_t get_flight_time() {
	return 0;
}

void set_burnwire_pin() {
	// Set burnwire pin high to enable 7V7 output to burnwire
	HAL_GPIO_WritePin(PARA_BRN_1_GPIO_Port, PARA_BRN_1_Pin, GPIO_PIN_SET);
	return;
}

// Should start "long timer" for burnwire pin set duration
void start_long_timer() {
	// TODO: Use BURNWIRE_LONG_TIME
	return;
}

// Should start "short timer" for burnwire pin set duration
void start_short_timer() {
	// TODO: Use BURNWIRE_SHORT_TIME
	return;
}