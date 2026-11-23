/**
 * @file telemetry_sensors.h
 * @brief function declarations and types for reading all telemetry sensors.
 */


#ifndef TELEMETRY_SENSORS_H
#define TELEMETRY_SENSORS_H


#include "stm32h7xx_hal.h"


/**
 * @brief 3-axis measurement structure for motion sensors.
 */

typedef struct {
	float x;
	float y;
	float z;
} vector3f_t;


/**
 * @brief Reads the current drawn by the OBC
 * @return Current in Amperes (A).
 */

float read_obc_current(void);


/**
 * @brief  Reads the current drawn by the NRF
 * @return Current in Amperes (A).
 */

float read_nrf_current(void);


/**
 * @brief Reads the current drawn by the SLIP TTC
 * @return Current in Amperes (A).
 */


float read_slipttc_current(void);


/**
 * @brief Reads the current drawn by the ORCA TTC
 * @return Current in Amperes (A).
 */


float read_orcattc_current(void);


/**
 * @brief Reads the current drawn by the burn-wire.
 * @return Current in Amperes (A).
 */

float read_burnwire_current(void);


/**
 * @brief Reads the voltage of the OBC
 * @return Voltage in Volts (V).
 */

float read_obc_voltage(void);


/**
 * @brief Reads the voltage of the NRF.
 * @return Voltage in Volts (V).
 */

float read_nrf_voltage(void);


/**
 * @brief Reads the voltage of the SLIP TTC.
 * @return Voltage in Volts (V).
 */

float read_slipttc_voltage(void);


/**
 * @brief Reads the voltage of the ORCA TTC.
 * @return Voltage in Volts (V).
 */

float read_orcattc_voltage(void);


/**
 * @brief Reads the voltage of the burn-wire.
 * @return Voltage in Volts (V).
 */

float read_burnwire_voltage(void);


/**
 * @brief Reads the temperature of the 3.3 Volt regulator.
 * @return Temperature in degrees Celsius (°C).
 */

float read_3v3reg_temperature(void);



/**
 * @brief Reads the temperature of the 5.0 Volt regulator.
 * @return Temperature in degrees Celsius (°C).
 */

float read_5vreg_temperature(void);



/**
 * @brief Reads the temperature of the 7.7 Volt regulator.
 * @return Temperature in degrees Celsius (°C).
 */

float read_7v7reg_temperature(void);




/* ALTIMETER CODE BEGINS */


/* Altimeter initialization with the MS5611 sensor.
 * Sensor is reset before initialization as per specification.
 * MS5611's 128-bit PROM stores 6 calibration coefficients (C1-C6) used
 * to convert raw pressure and temperature data into accurate measurements.
 * See page 6 of datasheeet for detail here:
 * https://www.amsys-sensor.com/downloads/data/MS5611-01BA03-AMSYS-datasheet.pdf
 */
HAL_StatusTypeDef altimeter_init(I2C_HandleTypeDef *hi2c);


/**
 * @brief Reads the current altitude.
 * @return Altitude in Meters (m).
 */
float read_altimeter(I2C_HandleTypeDef *hi2c);


/* Calculations are derived from International Standard Atmosphere and U.S. Standard Atmosphere.
 * Note:
 * The MS5611 sensor is rated for a minimum extended pressure range of 10 mbar.
 * Therefore, altitude can only be measured up to the stratosphere below 32km.
 */
float calculate_altitude(double P);

/* ALTIMETER CODE ENDS */



/**
 * @brief Reads the current 3-axis angular velocity from the gyroscope.
 * @return Angular velocity in Degrees per Second (°/s) for the X, Y, and Z axes.
 */


vector3f_t read_gyroscope(void);


/**
 * @brief Reads the current 3-axis acceleration from the accelerometer.
 * @return Acceleration in Meters per Second per Second (m/s^2) for the X, Y, and Z axes.
 */

vector3f_t read_accelerometer(void);

#endif /* TELEMETRY_SENSORS_H */



