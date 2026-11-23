#ifndef MS5611_H
#define MS5611_H

/* Includes */
#include "stm32h7xx_hal.h"
#include <stdint.h>
#include <math.h>

#define MS5611_ADDRESS 0X76	//MS5611-01BA03 address for I^2C communication is 0B111011Cx (so either 0X76 or 0X77)
#define MS5611_RESET 0x1E
#define MS5611_C1 0xA2
#define MS5611_D1 0X48
#define MS5611_D2 0X58
#define MS5611_ADC_READ 0X00

HAL_StatusTypeDef ms5611_reset(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef ms5611_read_prom(I2C_HandleTypeDef *hi2c, uint16_t address, uint16_t coefficient[6]);
HAL_StatusTypeDef ms5611_read_adc(I2C_HandleTypeDef *hi2c, uint16_t address, uint8_t convert_command, uint32_t *result);
int ms5611_calc(uint16_t C[6], uint32_t D1, uint32_t D2, double *pressure_mbar, double *temperature_c);

#endif
