#include "stm32h7xx_hal.h"
#include <stdint.h>
#include <math.h>


#define MS5611_RESET 0x1E
#define MS5611_C1 0xA2
#define MS5611_D1 0X48
#define MS5611_D2 0X58
#define MS5611_ADC_READ 0X00

#define REFERENCE_PRESSURE 1013.25
