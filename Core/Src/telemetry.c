/**
 * @file		: telemetry.c
 * @brief		: Implementation of telemetry functions
 */

#include "main.h"

// FreeRTOS task entry: xTaskCreate needs void (*)(void *), so the parameter
// can't be const.
// cppcheck-suppress constParameterPointer
void handle_telemetry(void *vpParameters) {
	/* TODO: Implement telemetry collection */
	for(;;) {}
}
