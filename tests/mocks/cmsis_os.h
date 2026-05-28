/**
 * @file  cmsis_os.h
 * @brief Minimal CMSIS-OS2 mock for host-based unit tests.
 */

#ifndef __CMSIS_OS_H
#define __CMSIS_OS_H

#include <stdint.h>
#include <stddef.h>

/* ── Priority ────────────────────────────────────────────────────────────── */

typedef enum {
    osPriorityNone          =  0,
    osPriorityIdle          =  1,
    osPriorityLow           =  8,
    osPriorityNormal        = 24,
    osPriorityAboveNormal   = 32,
    osPriorityHigh          = 40,
    osPriorityRealtime      = 48,
    osPriorityISR           = 56,
    osPriorityError         = -1
} osPriority_t;

/* ── Status ──────────────────────────────────────────────────────────────── */

typedef enum {
    osOK            = 0,
    osError         = -1,
    osErrorTimeout  = -2,
    osErrorResource = -3,
    osErrorParameter= -4,
    osErrorNoMemory = -5
} osStatus_t;

/* ── Thread ──────────────────────────────────────────────────────────────── */

typedef void *osThreadId_t;

typedef struct {
    const char   *name;
    uint32_t      attr_bits;
    void         *cb_mem;
    uint32_t      cb_size;
    void         *stack_mem;
    uint32_t      stack_size;
    osPriority_t  priority;
    uint32_t      tz_module;
    uint32_t      reserved;
} osThreadAttr_t;

static inline osStatus_t   osKernelInitialize(void)                              { return osOK; }
static inline osStatus_t   osKernelStart(void)                                   { return osOK; }
static inline osThreadId_t osThreadNew(void (*f)(void*), void *arg, const osThreadAttr_t *a)
    { (void)f; (void)arg; (void)a; return (osThreadId_t)1; }
static inline void         osDelay(uint32_t ms)                                  { (void)ms; }

/* ── Mutex ───────────────────────────────────────────────────────────────── */

typedef void *osMutexId_t;
typedef struct { const char *name; uint32_t attr_bits; void *cb_mem; uint32_t cb_size; } osMutexAttr_t;

static inline osMutexId_t osMutexNew(const osMutexAttr_t *a)         { (void)a; return (osMutexId_t)1; }
static inline osStatus_t  osMutexAcquire(osMutexId_t m, uint32_t t)  { (void)m; (void)t; return osOK; }
static inline osStatus_t  osMutexRelease(osMutexId_t m)              { (void)m; return osOK; }

/* ── Semaphore ───────────────────────────────────────────────────────────── */

typedef void *osSemaphoreId_t;
typedef struct { const char *name; uint32_t attr_bits; void *cb_mem; uint32_t cb_size; } osSemaphoreAttr_t;

static inline osSemaphoreId_t osSemaphoreNew(uint32_t max, uint32_t init, const osSemaphoreAttr_t *a)
    { (void)max; (void)init; (void)a; return (osSemaphoreId_t)1; }
static inline osStatus_t osSemaphoreAcquire(osSemaphoreId_t s, uint32_t t) { (void)s; (void)t; return osOK; }
static inline osStatus_t osSemaphoreRelease(osSemaphoreId_t s)             { (void)s; return osOK; }

/* ── Queue ───────────────────────────────────────────────────────────────── */

typedef void *osMessageQueueId_t;
typedef struct { const char *name; uint32_t attr_bits; void *cb_mem; uint32_t cb_size;
                 void *mq_mem; uint32_t mq_size; } osMessageQueueAttr_t;

static inline osMessageQueueId_t osMessageQueueNew(uint32_t cap, uint32_t msgsz, const osMessageQueueAttr_t *a)
    { (void)cap; (void)msgsz; (void)a; return (osMessageQueueId_t)1; }
static inline osStatus_t osMessageQueuePut(osMessageQueueId_t q, const void *m, uint8_t pri, uint32_t t)
    { (void)q; (void)m; (void)pri; (void)t; return osOK; }
static inline osStatus_t osMessageQueueGet(osMessageQueueId_t q, void *m, uint8_t *pri, uint32_t t)
    { (void)q; (void)m; (void)pri; (void)t; return osOK; }

#endif /* __CMSIS_OS_H */