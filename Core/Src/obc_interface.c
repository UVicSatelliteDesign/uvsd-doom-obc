/**
 * @file		: obc_interface.c
 * @brief		: Interface functions for sensors etc.
 */

#include "obc_interface.h"
#include "burnwire.h"
#include "timers.h"

#define BURNWIRE_LONG_TIME 120 // TODO: Set to actual time in seconds
#define BURNWIRE_SHORT_TIME 60 // TODO: Set to actual time in seconds

static TimerHandle_t long_timer = NULL;
static TimerHandle_t short_timer = NULL;

// Timer callbacks run in the FreeRTOS timer service task
static void long_timer_expired(TimerHandle_t xTimer) {
	(void)xTimer;
	long_timer_callback();
}

static void short_timer_expired(TimerHandle_t xTimer) {
	(void)xTimer;
	short_timer_callback();
}

// Create the one-shot burnwire timers. Call once before the scheduler starts,
// since timers cannot be created from an ISR (the short timer is started from one).
void init_burnwire_timers(void) {
	long_timer = xTimerCreate("Burnwire Long",
			pdMS_TO_TICKS(BURNWIRE_LONG_TIME * 1000U),
			pdFALSE,
			NULL,
			long_timer_expired);
	short_timer = xTimerCreate("Burnwire Short",
			pdMS_TO_TICKS(BURNWIRE_SHORT_TIME * 1000U),
			pdFALSE,
			NULL,
			short_timer_expired);
}

// Time since the scheduler started in seconds
uint32_t get_flight_time(void) {
	return xTaskGetTickCount() / configTICK_RATE_HZ;
}

void set_burnwire_pin(void) {
	// Set burnwire pin high to enable 7V7 output to burnwire
	HAL_GPIO_WritePin(PARA_BRN_1_GPIO_Port, PARA_BRN_1_Pin, GPIO_PIN_SET);
	return;
}

void reset_burnwire_pin(void) {
	// Set burnwire pin low to disable 7V7 output to burnwire
	HAL_GPIO_WritePin(PARA_BRN_1_GPIO_Port, PARA_BRN_1_Pin, GPIO_PIN_RESET);
	return;
}

// Start a timer from either a task or an ISR
static void start_timer(TimerHandle_t timer) {
	if (timer == NULL) {
		return;
	}

	if (xPortIsInsideInterrupt()) {
		BaseType_t higher_priority_task_woken = pdFALSE;
		xTimerStartFromISR(timer, &higher_priority_task_woken);
		portYIELD_FROM_ISR(higher_priority_task_woken);
	} else {
		// Wait for room in the timer command queue; the burnwire must not stay on
		xTimerStart(timer, portMAX_DELAY);
	}
}

// Start "long timer" for burnwire pin set duration
void start_long_timer(void) {
	start_timer(long_timer);
}

// Start "short timer" for burnwire pin set duration
void start_short_timer(void) {
	start_timer(short_timer);
}
