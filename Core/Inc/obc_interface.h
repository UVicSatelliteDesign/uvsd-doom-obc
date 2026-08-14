/**
 * @file		: obc_interface.h
 * @brief		: Interface functions for sensors etc.
 */

#include "main.h"

float read_altitude();
uint32_t get_flight_time();
void set_burnwire_pin();
void start_long_timer();
void start_short_timer();