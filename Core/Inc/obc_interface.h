/**
 * @file		: obc_interface.h
 * @brief		: Interface functions for sensors etc.
 */

#ifndef OBC_INTERFACE_H
#define OBC_INTERFACE_H

#include "main.h"

uint32_t get_flight_time();
void set_burnwire_pin();
void reset_burnwire_pin();
void start_long_timer();
void start_short_timer();

#endif // OBC_INTERFACE_H
