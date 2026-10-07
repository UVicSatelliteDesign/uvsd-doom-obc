/**
 * @file  task.h
 * @brief Minimal FreeRTOS task API mock for host-based unit tests.
 *
 * Tests can steer task notifications through mock_task_notify_value:
 * the next ulTaskNotifyTake / xTaskNotifyWait returns that value and clears it.
 * xTaskNotify records the last value sent in mock_task_last_notify.
 */

#ifndef MOCK_TASK_H
#define MOCK_TASK_H

#include "FreeRTOS.h"

typedef void *TaskHandle_t;

typedef enum {
    eNoAction = 0,
    eSetBits,
    eIncrement,
    eSetValueWithOverwrite,
    eSetValueWithoutOverwrite
} eNotifyAction;

/* Test hooks (static so each test binary gets its own copy). */
static uint32_t   mock_task_notify_value = 0;
static uint32_t   mock_task_last_notify  = 0;
static TickType_t mock_task_tick_count   = 0;

static inline void vTaskDelay(TickType_t ticks) { mock_task_tick_count += ticks; }
static inline TickType_t xTaskGetTickCount(void) { return mock_task_tick_count; }
static inline TaskHandle_t xTaskGetCurrentTaskHandle(void) { return (TaskHandle_t)1; }
static inline TaskHandle_t xTaskGetHandle(const char *name) { (void)name; return (TaskHandle_t)1; }
static inline void vTaskDelete(TaskHandle_t t) { (void)t; }

static inline uint32_t ulTaskNotifyTake(BaseType_t clear_on_exit, TickType_t wait)
{
    (void)clear_on_exit; (void)wait;
    uint32_t v = mock_task_notify_value;
    mock_task_notify_value = 0;
    return v;
}

static inline BaseType_t xTaskNotifyWait(uint32_t clear_on_entry, uint32_t clear_on_exit,
                                         uint32_t *value, TickType_t wait)
{
    (void)clear_on_entry; (void)clear_on_exit; (void)wait;
    if (value) *value = mock_task_notify_value;
    BaseType_t got = mock_task_notify_value ? pdTRUE : pdFALSE;
    mock_task_notify_value = 0;
    return got;
}

static inline BaseType_t xTaskNotify(TaskHandle_t t, uint32_t value, eNotifyAction action)
{
    (void)t; (void)action;
    mock_task_last_notify = value;
    return pdPASS;
}

/* Silence -Wunused-variable in tests that never touch the hooks. */
static inline void mock_task_reset(void)
{
    mock_task_notify_value = 0;
    mock_task_last_notify  = 0;
    mock_task_tick_count   = 0;
}

#endif /* MOCK_TASK_H */
