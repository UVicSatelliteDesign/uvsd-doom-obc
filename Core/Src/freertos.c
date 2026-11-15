/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file          : freertos.c
  * @brief        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

#include "semphr.h"
#include "telemetry_sensors.h"
#include "command.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// Burnwire thresholds
#define ALTITUDE_LIMIT 1000.0f
#define TIME_LIMIT 1000


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

SemaphoreHandle_t sdMutex = NULL;

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

int task_communication_center(void)
{
  // Initialize the SD card mutex
  sdMutex = xSemaphoreCreateMutex();

  for(;;){
    
    int is_burnwire_activated = check_burnwire_activation();

    if(is_burnwire_activated == 1){
      // Empty until we get the burnwire and notify GS and telemetry

      send_burnwire_event();
    }

    osDelay(100);
  }
}

// send command to the burnwire to activate
int check_burnwire_activation()
{
  // Read current altitude from telemetry sensors
  float altitude = read_altimeter();
  int time_elapsed = 0;
  int manual_request = receive_burnwire_command();

  // Altitude activation (1000 is placeholder)
  if(altitude >= ALTITUDE_LIMIT){
    return 1;
  }

  // Time activation placeholder
  if(time_elapsed >= TIME_LIMIT){
    return 1;
  }

  // Manual activation from ground station placeholder
  if(manual_request == 1){
    return 1;
  }

  // No activation
  return 0;
}

// receive burnwire command from command
int receive_burnwire_command()
{
  
  // Haven't received GS command
  return 0;
}

// notifies the ground station and telemetry
int send_burnwire_event()
{
  
  return 0;
}

// send telemetry data to the ground station
int send_telemetry_data()
{

  return 0; 
}

// receive telemetry data from the telemetry sensors
int receive_telemetry_data()
{
  
  return 0;
}


// Should use a mutex to lock the SD card so different tasks don't try to access it at the same time.
int lock_sd_card(void)
{
  // Mutex is not initalized yet
  if(sdMutex == NULL){
    return 0;
  }

  // Waits forever until the SD card is available
  if(xSemaphoreTake(sdMutex, portMAX_DELAY) == pdTRUE){
    return 1;
  }

  // Mutex is in use at the moment
  return 0;
}


void unlock_sd_card(void)
{
  // Unlock the SD card for other tasks to use
  if(sdMutex != NULL){
      xSemaphoreGive(sdMutex);
  }
}


/* USER CODE END Application */

