/**
 * @file		: obc_interface.h
 * @brief		: Interface functions for sensors etc.
 */

#ifndef OBC_INTERFACE_H
#define OBC_INTERFACE_H

#include "main.h"

uint32_t get_flight_time(void);
void set_burnwire_pin(void);
void reset_burnwire_pin(void);
void start_long_timer(void);
void start_short_timer(void);

#endif // OBC_INTERFACE_H
