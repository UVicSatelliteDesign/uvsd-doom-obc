/**
 * @file		: burnwire.h
 * @brief		: Burnwire/parachute deployment task and its callbacks
 */

#ifndef BURNWIRE_H
#define BURNWIRE_H

#include <stdint.h>

// From the mech team's COTS Parachute & Balloon Selection sheet (4 m/s ascent)
#define BURNWIRE_ALTITUDE_LIMIT 15600 // Altitude in meters at which to trigger burnwire
#define BURNWIRE_TIME_LIMIT (65*60) // Time in seconds at which to trigger burnwire

// Message must be exactly REQUEST | BURNWIRE (see main.h); anything else is ignored
void burnwire_command_callback(uint32_t message);
void deployment_switches_callback(void);
void short_timer_callback(void);
void long_timer_callback(void);

#endif // BURNWIRE_H
