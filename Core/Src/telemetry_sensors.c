/**
 * @file telemetry_sensors.c
 * @brief Stub implementations for telemetry sensor reading functions.
 */

#include "telemetry_sensors.h"

//related to MS5611
#include "ms5611.h"
#include <math.h>
#define REFERENCE_PRESSURE 1013.25

//put things in header later
#define MS5611_RESET 0x1E
#define MS5611_C1 0xA2
#define MS5611_D1 0X48
#define MS5611_D2 0X58
#define MS5611_ADC_READ 0X00

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
	float pressure_mbar;	//pressure pointer
	float temperature_c;	//temperature pointer
	uint32_t *D1;	//digital pressure value
	uint32_t *D2;	//digital temperature value
	I2C_HandleTypeDef *altimeter_hi2c4 = NULL;	//for later
	uint16_t C[6];	//coefficients C1-C6

	//reset sensor
	ms5611_reset(altimeter_hi2c4);

	HAL_StatusTypeDef get_coefficients = ms5611_read_prom(altimeter_hi2c4, 0X76, C);

	//get raw digital pressure value
	ms5611_read_adc(altimeter_hi2c4, 0X76, MS5611_D1, D1);

	//get raw digital temperature value
	ms5611_read_adc(altimeter_hi2c4, 0X76, MS5611_D2, D2);

	//calculate and store calibrated pressure measurement from raw data
	int calc_pressure = ms5611_calc(C, *D1, *D2, &pressure_mbar, &temperature_c);

	//calculating altitude
	float altitude = 44330 * (1.0 - powf((pressure_mbar / REFERENCE_PRESSURE), 0.190284));


	//return 0.0f;
	return altitude;
}


vector3f_t read_gyroscope(void) {
	vector3f_t angular_velocity = {0.0f, 0.0f, 0.0f};
	return angular_velocity;
}


vector3f_t read_accelerometer(void) {
	vector3f_t acceleration = {0.0f, 0.0f, 0.0f};
	return acceleration;
}
