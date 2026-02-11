/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId debug_uartHandle;
osThreadId ledHandle;
osThreadId get_rcHandle;
osThreadId imu_dataHandle;
osThreadId can_sentHandle;
osThreadId shoot_taskHandle;
osThreadId stop_checkHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void DEBUG_UART_TASK(void const * argument);
void LED_TASK(void const * argument);
void GET_RC_TASK(void const * argument);
void IMU_DATA_GET(void const * argument);
void CAN_SENT_TASK(void const * argument);
void SHOOT_TASK(void const * argument);
void SHOOOT_STOP_CHECK(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of debug_uart */
  osThreadDef(debug_uart, DEBUG_UART_TASK, osPriorityIdle, 0, 1024);
  debug_uartHandle = osThreadCreate(osThread(debug_uart), NULL);

  /* definition and creation of led */
  osThreadDef(led, LED_TASK, osPriorityIdle, 0, 128);
  ledHandle = osThreadCreate(osThread(led), NULL);

  /* definition and creation of get_rc */
  osThreadDef(get_rc, GET_RC_TASK, osPriorityIdle, 0, 256);
  get_rcHandle = osThreadCreate(osThread(get_rc), NULL);

  /* definition and creation of imu_data */
  osThreadDef(imu_data, IMU_DATA_GET, osPriorityIdle, 0, 1024);
  imu_dataHandle = osThreadCreate(osThread(imu_data), NULL);

  /* definition and creation of can_sent */
  osThreadDef(can_sent, CAN_SENT_TASK, osPriorityIdle, 0, 256);
  can_sentHandle = osThreadCreate(osThread(can_sent), NULL);

  /* definition and creation of shoot_task */
  osThreadDef(shoot_task, SHOOT_TASK, osPriorityIdle, 0, 128);
  shoot_taskHandle = osThreadCreate(osThread(shoot_task), NULL);

  /* definition and creation of stop_check */
  osThreadDef(stop_check, SHOOOT_STOP_CHECK, osPriorityIdle, 0, 128);
  stop_checkHandle = osThreadCreate(osThread(stop_check), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_DEBUG_UART_TASK */
/**
* @brief Function implementing the debug_uart thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DEBUG_UART_TASK */
__weak void DEBUG_UART_TASK(void const * argument)
{
  /* USER CODE BEGIN DEBUG_UART_TASK */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END DEBUG_UART_TASK */
}

/* USER CODE BEGIN Header_LED_TASK */
/**
* @brief Function implementing the led thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LED_TASK */
__weak void LED_TASK(void const * argument)
{
  /* USER CODE BEGIN LED_TASK */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END LED_TASK */
}

/* USER CODE BEGIN Header_GET_RC_TASK */
/**
* @brief Function implementing the get_rc thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_GET_RC_TASK */
__weak void GET_RC_TASK(void const * argument)
{
  /* USER CODE BEGIN GET_RC_TASK */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END GET_RC_TASK */
}

/* USER CODE BEGIN Header_IMU_DATA_GET */
/**
* @brief Function implementing the imu_data thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_IMU_DATA_GET */
__weak void IMU_DATA_GET(void const * argument)
{
  /* USER CODE BEGIN IMU_DATA_GET */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END IMU_DATA_GET */
}

/* USER CODE BEGIN Header_CAN_SENT_TASK */
/**
* @brief Function implementing the can_sent thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CAN_SENT_TASK */
__weak void CAN_SENT_TASK(void const * argument)
{
  /* USER CODE BEGIN CAN_SENT_TASK */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END CAN_SENT_TASK */
}

/* USER CODE BEGIN Header_SHOOT_TASK */
/**
* @brief Function implementing the shoot_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SHOOT_TASK */
__weak void SHOOT_TASK(void const * argument)
{
  /* USER CODE BEGIN SHOOT_TASK */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END SHOOT_TASK */
}

/* USER CODE BEGIN Header_SHOOOT_STOP_CHECK */
/**
* @brief Function implementing the stop_check thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SHOOOT_STOP_CHECK */
__weak void SHOOOT_STOP_CHECK(void const * argument)
{
  /* USER CODE BEGIN SHOOOT_STOP_CHECK */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END SHOOOT_STOP_CHECK */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
