/**
 * @file  task_mock.c
 * @brief Storage for the task.h mock's test hooks.
 */

#include "task.h"

uint32_t   mock_task_notify_value = 0;
uint32_t   mock_task_last_notify  = 0;
TickType_t mock_task_tick_count   = 0;
