/**
 * @file  stm32h7xx_hal.h
 * @brief Minimal HAL mock for host-based unit tests.
 *
 * This file stands in for the real STM32 HAL so that application source
 * files can be compiled and tested on a standard Linux host without any
 * ARM toolchain or hardware present.
 *
 * Add stubs here as more of the real HAL surface area is exercised by tests.
 */

#ifndef __STM32H7xx_HAL_H
#define __STM32H7xx_HAL_H

#include <stdint.h>
#include <stddef.h>

/* ── Basic types ─────────────────────────────────────────────────────────── */

typedef enum {
    HAL_OK      = 0x00U,
    HAL_ERROR   = 0x01U,
    HAL_BUSY    = 0x02U,
    HAL_TIMEOUT = 0x03U
} HAL_StatusTypeDef;

typedef enum {
    HAL_UNLOCKED = 0x00U,
    HAL_LOCKED   = 0x01U
} HAL_LockTypeDef;

/* ── Tick / delay ────────────────────────────────────────────────────────── */

static inline uint32_t HAL_GetTick(void)    { return 0; }
static inline void     HAL_Delay(uint32_t d) { (void)d; }
static inline HAL_StatusTypeDef HAL_Init(void) { return HAL_OK; }
static inline void HAL_IncTick(void) {}

/* ── GPIO stub ───────────────────────────────────────────────────────────── */

typedef struct { uint32_t dummy; } GPIO_TypeDef;
typedef struct {
    uint32_t Pin;
    uint32_t Mode;
    uint32_t Pull;
    uint32_t Speed;
    uint32_t Alternate;
} GPIO_InitTypeDef;

#define GPIO_PIN_RESET 0U
#define GPIO_PIN_SET   1U
typedef uint32_t GPIO_PinState;

static inline void        HAL_GPIO_Init(GPIO_TypeDef *g, GPIO_InitTypeDef *c) { (void)g; (void)c; }
static inline GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *g, uint16_t p)     { (void)g; (void)p; return GPIO_PIN_RESET; }
static inline void        HAL_GPIO_WritePin(GPIO_TypeDef *g, uint16_t p, GPIO_PinState s) { (void)g; (void)p; (void)s; }
static inline void        HAL_GPIO_TogglePin(GPIO_TypeDef *g, uint16_t p)     { (void)g; (void)p; }

/* ── UART stub ───────────────────────────────────────────────────────────── */

typedef struct { uint32_t dummy; } USART_TypeDef;
typedef struct { USART_TypeDef *Instance; uint32_t BaudRate; } UART_HandleTypeDef;

static inline HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *h, const uint8_t *d, uint16_t s, uint32_t t)
    { (void)h; (void)d; (void)s; (void)t; return HAL_OK; }
static inline HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *h, uint8_t *d, uint16_t s, uint32_t t)
    { (void)h; (void)d; (void)s; (void)t; return HAL_OK; }

/* ── SPI stub ────────────────────────────────────────────────────────────── */

typedef struct { uint32_t dummy; } SPI_TypeDef;
typedef struct { SPI_TypeDef *Instance; } SPI_HandleTypeDef;

static inline HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *h, const uint8_t *d, uint16_t s, uint32_t t)
    { (void)h; (void)d; (void)s; (void)t; return HAL_OK; }
static inline HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *h, uint8_t *d, uint16_t s, uint32_t t)
    { (void)h; (void)d; (void)s; (void)t; return HAL_OK; }
static inline HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *h, const uint8_t *tx, uint8_t *rx, uint16_t s, uint32_t t)
    { (void)h; (void)tx; (void)rx; (void)s; (void)t; return HAL_OK; }

/* ── I2C stub ────────────────────────────────────────────────────────────── */

typedef struct { uint32_t dummy; } I2C_TypeDef;
typedef struct { I2C_TypeDef *Instance; } I2C_HandleTypeDef;

static inline HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *h, uint16_t a, uint8_t *d, uint16_t s, uint32_t t)
    { (void)h; (void)a; (void)d; (void)s; (void)t; return HAL_OK; }
static inline HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *h, uint16_t a, uint8_t *d, uint16_t s, uint32_t t)
    { (void)h; (void)a; (void)d; (void)s; (void)t; return HAL_OK; }

/* ── TIM stub ────────────────────────────────────────────────────────────── */

typedef struct { uint32_t dummy; } TIM_TypeDef;
typedef struct { TIM_TypeDef *Instance; struct { uint32_t Period; uint32_t Prescaler; uint32_t ClockDivision; uint32_t CounterMode; } Init; } TIM_HandleTypeDef;

static inline HAL_StatusTypeDef HAL_TIM_Base_Init(TIM_HandleTypeDef *h) { (void)h; return HAL_OK; }
static inline HAL_StatusTypeDef HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *h) { (void)h; return HAL_OK; }
static inline void HAL_TIM_IRQHandler(TIM_HandleTypeDef *h) { (void)h; }

/* ── NVIC / MPU stubs ────────────────────────────────────────────────────── */

static inline void HAL_NVIC_SetPriority(int irq, uint32_t p, uint32_t s) { (void)irq; (void)p; (void)s; }
static inline void HAL_NVIC_EnableIRQ(int irq) { (void)irq; }

typedef struct {
    uint8_t  Enable;
    uint8_t  Number;
    uint32_t BaseAddress;
    uint8_t  Size;
    uint8_t  SubRegionDisable;
    uint8_t  TypeExtField;
    uint8_t  AccessPermission;
    uint8_t  DisableExec;
    uint8_t  IsShareable;
    uint8_t  IsCacheable;
    uint8_t  IsBufferable;
} MPU_Region_InitTypeDef;

#define MPU_REGION_ENABLE           1U
#define MPU_REGION_NUMBER0          0U
#define MPU_REGION_SIZE_4GB         0x1FU
#define MPU_TEX_LEVEL0              0U
#define MPU_REGION_NO_ACCESS        0U
#define MPU_INSTRUCTION_ACCESS_DISABLE 1U
#define MPU_ACCESS_SHAREABLE        1U
#define MPU_ACCESS_NOT_CACHEABLE    0U
#define MPU_ACCESS_NOT_BUFFERABLE   0U
#define MPU_PRIVILEGED_DEFAULT      4U

static inline void HAL_MPU_Disable(void) {}
static inline void HAL_MPU_Enable(uint32_t ctrl) { (void)ctrl; }
static inline void HAL_MPU_ConfigRegion(MPU_Region_InitTypeDef *r) { (void)r; }

/* ── RCC stubs ───────────────────────────────────────────────────────────── */

typedef struct {
    uint32_t OscillatorType;
    uint32_t HSIState;
    uint32_t HSICalibrationValue;
    struct { uint32_t PLLState; } PLL;
} RCC_OscInitTypeDef;

typedef struct {
    uint32_t ClockType;
    uint32_t SYSCLKSource;
    uint32_t SYSCLKDivider;
    uint32_t AHBCLKDivider;
    uint32_t APB3CLKDivider;
    uint32_t APB1CLKDivider;
    uint32_t APB2CLKDivider;
    uint32_t APB4CLKDivider;
    uint32_t D3PCLK1Divider;
    uint32_t D1PCLK1Divider;
} RCC_ClkInitTypeDef;

#define RCC_OSCILLATORTYPE_HSI   0x00000002U
#define RCC_HSI_DIV1             0x00000000U
#define RCC_PLL_NONE             0x00000000U
#define RCC_CLOCKTYPE_HCLK       0x00000002U
#define RCC_CLOCKTYPE_SYSCLK     0x00000001U
#define RCC_CLOCKTYPE_PCLK1      0x00000004U
#define RCC_CLOCKTYPE_PCLK2      0x00000008U
#define RCC_CLOCKTYPE_D3PCLK1    0x00000020U
#define RCC_CLOCKTYPE_D1PCLK1    0x00000010U
#define RCC_SYSCLKSOURCE_HSI     0x00000000U
#define RCC_SYSCLK_DIV1          0x00000000U
#define RCC_HCLK_DIV1            0x00000000U
#define RCC_APB1_DIV1            0x00000000U
#define RCC_APB2_DIV1            0x00000000U
#define RCC_APB3_DIV1            0x00000000U
#define RCC_APB4_DIV1            0x00000000U
#define FLASH_LATENCY_1          0x00000001U

static inline HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef *c)          { (void)c; return HAL_OK; }
static inline HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *c, uint32_t l) { (void)c; (void)l; return HAL_OK; }
static inline void              HAL_RCC_GetClockConfig(RCC_ClkInitTypeDef *c, uint32_t *l) { (void)c; (void)l; }
static inline uint32_t          HAL_RCC_GetPCLK1Freq(void) { return 64000000U; }

/* ── PWR stubs ───────────────────────────────────────────────────────────── */

#define PWR_LDO_SUPPLY                    0U
#define PWR_REGULATOR_VOLTAGE_SCALE3      0U
#define PWR_FLAG_VOSRDY                   0U

static inline void HAL_PWREx_ConfigSupply(uint32_t s)  { (void)s; }
static inline int  __HAL_PWR_GET_FLAG(uint32_t f)      { (void)f; return 1; }

/* Macro wrappers used in main.c */
#define __HAL_PWR_VOLTAGESCALING_CONFIG(x)  ((void)(x))
#define __HAL_RCC_SYSCFG_CLK_ENABLE()
#define __HAL_RCC_TIM6_CLK_ENABLE()
#define __HAL_TIM_DISABLE_IT(h, f)  ((void)(h), (void)(f))
#define __HAL_TIM_ENABLE_IT(h, f)   ((void)(h), (void)(f))

/* IRQ numbers (just needs to be int for the stubs) */
#define PendSV_IRQn     -2
#define TIM6_DAC_IRQn    54

extern uint32_t SystemCoreClock;
extern uint32_t uwTickPrio;

#endif /* __STM32H7xx_HAL_H */