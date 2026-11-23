/**
 * @file telemetry_sensors.c
 * @brief Stub implementations for telemetry sensor reading functions.
 */

#include "telemetry_sensors.h"

//related to the MS5611 sensor for altimeter readings
#include "ms5611.h"
#include <math.h>
#include <stdbool.h>
#define MS5611_ADDRESS 0X76
static uint16_t altimeter_coefficients[6];	//coefficients C1-C6
static bool altimeter_initialized = false;
static I2C_HandleTypeDef *altimeter_hi2c;


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



/* ALTIMETER CODE BEGINS */

//Altimeter initialization
HAL_StatusTypeDef altimeter_init(I2C_HandleTypeDef *hi2c) {

	altimeter_initialized = false;

	if (hi2c == NULL) {
		return HAL_ERROR;
	}

	//reset sensor
	HAL_StatusTypeDef status = ms5611_reset(hi2c);
	if (status != HAL_OK) {
		return status;
	}

	//get calibration coefficients
	status = ms5611_read_prom(hi2c, MS5611_ADDRESS, altimeter_coefficients);
	if (status != HAL_OK) {
		return status;
	}

	altimeter_hi2c = hi2c;
	altimeter_initialized = true;

	return status;
}


//Returns Geometric Altitude measurements in meters
/* Note:
 * The MS5611 sensor is rated for a minimum extended pressure range of 10 mbar.
 * Therefore, altitude can only be measured up to the stratosphere below 32km. */
float read_altimeter(void) {

	if (!altimeter_initialized || altimeter_hi2c == NULL) {
		return NAN;
	}

	double pressure_mbar;	//compensated pressure measurement in (mbar)
	double temperature_c;	//compensated temperature measurement in (°C)
	uint32_t D1;	//digital pressure value
	uint32_t D2;	//digital temperature value

	//get raw digital pressure value
	HAL_StatusTypeDef status = ms5611_read_adc(altimeter_hi2c, MS5611_ADDRESS, MS5611_D1, &D1);
	if (status != HAL_OK) {
		return NAN;
	}

	//get raw digital temperature value
	status = ms5611_read_adc(altimeter_hi2c, MS5611_ADDRESS, MS5611_D2, &D2);
	if (status != HAL_OK) {
		return NAN;
	}

	//calculate and store calibrated pressure measurement from raw data
	int successful = ms5611_calc(altimeter_coefficients, D1, D2, &pressure_mbar, &temperature_c);
	if (!successful || pressure_mbar <= 0.0) {
		return NAN;
	}

	//calculating altitude
	float altitude_m = calculate_altitude(pressure_mbar);

	return altitude_m;
}


/* Calculations are derived from International Standard Atmosphere and U.S. Standard Atmosphere.
 * For more specific details on derivation and variables, see pages 1-12 here::
 * https://www.ngdc.noaa.gov/stp/space-weather/online-publications/miscellaneous/us-standard-atmosphere-1976/us-standard-atmosphere_st76-1562_noaa.pdf
 * Note:
 * The MS5611 sensor is rated for a minimum pressure range of 10 mbar.
 * Therefore, altitude can only be accurately measured up to the stratosphere below 32km. */
float calculate_altitude(double P) {

	/*Physical constants*/
	const double g = 9.80665;	//gravity constant of 9.80665 m/s^2
	const double R = 287.05287;	//specific gas constant of 287.05287 J/(kg*K)
	const double e_radius = 6356766.0;	//Earth's radius of 6,356,766 m

	double P_0 = 1013.25;	//reference sea level pressure
	double P_1 = 226.32;	//base troposphere pressure
	double P_2 = 54.749;	//base stratosphere pressure

	//base temperature in degrees Kelvin for given atmospheric layer
	double T_0 = 288.15;	//troposphere
	double T = 216.65;		//tropopause and stratosphere

	//temperature differential per meter
	double L_0 = -0.0065;	//troposphere
	double L_2 = 0.001;		//stratosphere


	double h_altitude;	//geopotential altitude handle



	/* Calculation for Geopotential Altitude
	 * for the given atmospheric layer (based on the given atmospheric pressure)
	 * and constant reference gravity 'g' */
	if (P >= P_1) {	//Troposphere
		h_altitude = (T_0 / L_0) * (pow( (P / P_0), -((R * L_0) / g)) - 1.0);
	}
	else if (P >= P_2) { //Tropopause
		h_altitude = 11000.0 - (((R * T) / g) * log(P / P_1));
	}
	else {	//Stratopshere
		h_altitude = 20000.0 + (T / L_2) * (pow( (P / P_2), -((R * L_2) / g)) - 1.0);
	}

	/* Conversion to Geometric Altitude */
	double z_altitude = (e_radius * h_altitude) / (e_radius - h_altitude);

	return (float)z_altitude;

}

/* ALTIMETER CODE ENDS */



vector3f_t read_gyroscope(void) {
	vector3f_t angular_velocity = {0.0f, 0.0f, 0.0f};
	return angular_velocity;
}


vector3f_t read_accelerometer(void) {
	vector3f_t acceleration = {0.0f, 0.0f, 0.0f};
	return acceleration;
}
