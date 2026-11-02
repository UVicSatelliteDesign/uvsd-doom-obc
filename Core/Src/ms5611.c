/*
 * Author: Sam Park
 * Please reach out if you have questions or need revisions!
*/

#include "ms5611.h"
#include "stm32h7xx_hal.h"
#include <stdint.h>
#include <math.h>


//put things in header later
#define MS5611_RESET 0x1E
#define MS5611_C1 0xA2
#define MS5611_D1 0X48
#define MS5611_D2 0X58
#define MS5611_ADC_READ 0X00

#define REFERENCE_PRESSURE 1013.25

/*
 * Read datasheet relating MS5611-01BA03 for details
 * https://www.amsys-sensor.com/downloads/data/MS5611-01BA03-AMSYS-datasheet.pdf
*/

/*
 * Notable Mentions:
 *
 * MS5611-01BA03 address for I^2C communication is 0B111011Cx (so either 0X76 or 0X77)
 * Read page 12 of datasheet for detail
 *
 * STM32_HAL expects 7-bit address to be shifted left by 1
*/



HAL_StatusTypeDef ms5611_reset(I2C_HandleTypeDef *hi2c) {
	uint8_t command = MS5611_RESET;

	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(
			hi2c,
			(0x76 << 1),	//target address for MS5611 I2C port for altimeter
			&command,
			1,	//number of bytes to transmit
			100);

	if (status != HAL_OK) {
		return status;
	}

	HAL_Delay(4);	//should I change this?
	return HAL_OK;
}


/*
 * 128-bit PROM stores 6 calibration coefficients (C1-C6) used to convert
 * raw pressure and temperature data into accurate measurements.
 * See page 6 of datasheeet for detail.
*/
HAL_StatusTypeDef ms5611_read_prom(I2C_HandleTypeDef *hi2c, uint16_t address, uint16_t coefficient[6]) {
	for (uint8_t i = 0; i < 6; i++) {
		uint8_t command = MS5611_C1 + (i * 2);	//calibration coefficient commands starting from C1
		uint8_t data[2];

		//transmit request for calibration coefficients
		HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(hi2c, (address << 1), &command, 1, 100);
		if (status != HAL_OK) {
			return status;
		}

		//retrieved calibration coefficients
		status = HAL_I2C_Master_Receive(hi2c, (address << 1), data, 2, 100);
		if (status != HAL_OK) {
			return status;
		}

		//storing calibration coefficients
		coefficient[i] = data[0] << 8;	//left shift 8 bits for left byte
		coefficient[i] |= data[1];
	}

	return HAL_OK;
}

/*
 * ADC (Analog to Digital Converter) converts the sensor's raw analog measurements into digital data
 * representing raw pressure measurements (D1) or raw temperature measurements (D2).
 * 24-bit raw measurement data is retrieved.
*/
HAL_StatusTypeDef ms5611_read_adc(I2C_HandleTypeDef *hi2c, uint16_t address, uint8_t convert_command, uint32_t *result) {
	if (result == NULL) {
		return HAL_ERROR;
	}
	if (convert_command != MS5611_D1 && convert_command != MS5611_D2) {
		return HAL_ERROR;
	}

	//first transmit for instructing conversion from raw analog to raw digital measurements
	uint16_t device_address = address << 1;
	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(hi2c, device_address, &convert_command, 1, 100);
	if (status != HAL_OK) {
		return status;
	}

	//ADC for OSR=4096 takes maximum 9.04ms
	HAL_Delay(10);

	//read converted raw digital data
	uint8_t command = MS5611_ADC_READ;
	status = HAL_I2C_Master_Transmit(hi2c, device_address, &command, 1, 100);
	if (status != HAL_OK) {
		return status;
	}

	//retrieve the raw digital measurement result
	uint8_t data[3];
	status = HAL_I2C_Master_Receive(hi2c, device_address, data, 3, 100);
	if (status != HAL_OK) {
		return status;
	}

	//store resulting 24-bit raw measurements
	*result = data[0] << 16;
	*result |= data[1] << 8;
	*result |= data[2];

	return status;
}


/*
 * See page 8 and 9 of datasheet for detail
*/
int ms5611_calc(uint16_t C[6], uint32_t D1, uint32_t D2, float *pressure_mbar, float *temperature_c) {
	if (pressure_mbar == NULL || temperature_c == NULL) {
		return 0;
	}

	int32_t dT = D2 - (int64_t)C[4] * (1 << 8);	//dT = D2 - C5 * 2^8
	int32_t TEMP = 2000 + dT * C[5] / (1 << 23);	//TEMP = 2000 + dT * C6 / 2^23

	int64_t OFF = (int64_t)C[1] * (1 << 16) + ((int64_t)C[3] * dT) / (1 << 7);	//OFF = C2 * 2^16 + (C4 * dT) / 2^7
	int64_t SENS = (int64_t)C[0] * (1 << 15) + ((int64_t)C[2] * dT) / (1 << 8);	//SENS = C1 * 2^15 + (C3 * dT) / 2^8


	//see page 9 of data-sheet for context
	int64_t T2 = 0;
	int64_t OFF2 = 0;
	int64_t SENS2 = 0;
	if (TEMP < 2000) {	//if TEMP < 20 degrees C
		T2 = ((int64_t)dT * dT) / (1LL << 31);	//T2 = dT^2 / 2^31 (1LL for 32bit overflow, rest should be fine)
		OFF2 = 5 * ((TEMP - 2000) * (TEMP - 2000)) / 2;	//OFF2 = 5(TEMP / 2000)^2 / 2
		SENS2 = 5 * ((TEMP - 2000) * (TEMP - 2000)) / 4;	//SENS2 = 5(TEMP - 2000)^2 / 4

		if (TEMP < -1500) {	//if TEMP < -15 degrees C
			OFF2 += 7 * ((TEMP + 1500) * (TEMP + 1500));	//OFF2 = OFF2 + 7(TEMP + 1500)^2
			SENS2 += 11 * ((TEMP + 1500) * (TEMP + 1500)) / 2;	//SENS2 = SENS2 + 11(TEMP + 1500)^2 / 2
		}
	}

	TEMP -= T2;	//TEMP = TEMP - T2
	OFF -= OFF2;	//OFF = OFF - OFF2
	SENS -= SENS2;	//SENS = SENS - SENS2

	int32_t P = ((int64_t)D1 * SENS / (1 << 21) - OFF) / (1 << 15);	//P = (D1 * SENS / 2^21 - OFF) / 2^15

	*pressure_mbar = (float)P / 100.0;
	*temperature_c = (float)TEMP / 100.0;

	return 1;

}
