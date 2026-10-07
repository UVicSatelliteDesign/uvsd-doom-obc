/**
 * @file  FreeRTOS.h
 * @brief Minimal FreeRTOS kernel mock for host-based unit tests.
 *
 * Firmware headers (main.h) include FreeRTOS.h and task.h directly once a
 * module uses the native FreeRTOS API (e.g. burnwire.c's ulTaskNotifyTake).
 * The real kernel can't build on the host, so tests get these stand-ins.
 */

#ifndef MOCK_FREERTOS_H
#define MOCK_FREERTOS_H

#include <stdint.h>
#include <stddef.h>

typedef long          BaseType_t;
typedef unsigned long UBaseType_t;
typedef uint32_t      TickType_t;

#define pdFALSE            ((BaseType_t)0)
#define pdTRUE             ((BaseType_t)1)
#define pdPASS             pdTRUE
#define pdFAIL             pdFALSE
#define portMAX_DELAY      ((TickType_t)0xFFFFFFFFUL)
#define configTICK_RATE_HZ ((TickType_t)1000)
#define portTICK_PERIOD_MS ((TickType_t)1000 / configTICK_RATE_HZ)
#define pdMS_TO_TICKS(ms)  ((TickType_t)(((TickType_t)(ms) * configTICK_RATE_HZ) / (TickType_t)1000))

#endif /* MOCK_FREERTOS_H */
