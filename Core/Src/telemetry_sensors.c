/**
 * @file telemetry_sensors.c
 * @brief Stub implementations for telemetry sensor reading functions.
 */

#include "telemetry_sensors.h"

// Current reading function stubs

float read_obc_current(void) {
	return 0.0f;
}


float read_nrf_current(void) {
	return 0.0f;
}


float read_slipttc_current(void) {
	return 0.0f;
}


float read_orcattc_current(void) {
	return 0.0f;
}


float read_burnwire_current(void) {
	return 0.0f;
}


// Voltage reading function stubs

float read_obc_voltage(void) {
	return 0.0f;
}


float read_nrf_voltage(void) {
	return 0.0f;
}


float read_slipttc_voltage(void) {
	return 0.0f;
}


float read_orcattc_voltage(void) {
	return 0.0f;
}


float read_burnwire_voltage(void) {
	return 0.0f;
}


// Temperature reading function stubs

float read_3v3reg_temperature(void) {
	return 0.0f;
}


float read_5vreg_temperature(void) {
	return 0.0f;
}


float read_7v7reg_temperature(void) {
	return 0.0f;
}

// Motion & position reading function stubs


float read_altimeter(void) {
	return 0.0f;
}


vector3f_t read_gyroscope(void) {
	vector3f_t angular_velocity = {0.0f, 0.0f, 0.0f};
	return angular_velocity;
}


vector3f_t read_accelerometer(void) {
	vector3f_t acceleration = {0.0f, 0.0f, 0.0f};
	return acceleration;
}
