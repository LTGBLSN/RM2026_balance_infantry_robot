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
#include "referee.h"

void DEBUG_UART_TASK()
{
    while (1)
    {
        usart1_printf("%f,%f \r\n",
                      DM_imu.pitch,
                      angular_angle);
//        usart1_printf("%f,%f \r\n",
//                      DM8009P_01_RIGHT_BIG_LEG_FRONT.return_speed,
//                      DM8009P_01_RIGHT_BIG_LEG_FRONT.give_speed);
        osDelay(1);
    }
}

