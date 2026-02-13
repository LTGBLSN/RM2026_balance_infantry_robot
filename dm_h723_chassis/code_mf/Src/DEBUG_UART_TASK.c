//
// Created by 21481 on 2026/1/29.
//

#include "main.h"
#include "uart_printf.h"
#include "cmsis_os.h"
#include "remote_control.h"
#include "IMU_DATA_GET.h"
#include "DJI_motors.h"
#include "dm_motor.h"
#include "GET_RC_TASK.h"
#include "DM_IMU.h"

void DEBUG_UART_TASK()
{
    while (1)
    {
        usart1_printf("%d,%d,%d \r\n",DM8009_01.online_state,DM8009_01.last_online_time,HAL_GetTick());
        osDelay(1);
    }
}

