//
// Created by 21481 on 2025/3/16.
//

#include <stdio.h>
#include <string.h>
#include "main.h"
#include "cmsis_os.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "uart_printf.h"
#include "uart_sent.h"
#include "CAN_receive.h"
#include "dm_motor.h"
#include "DM_IMU.h"
#include "CHASSIS_TASK.h"


void uart_sent_debug()
{
    while (1)
    {
        usart6_printf("%d,%d,%d,%d \r\n",
                      gimbal_info.shoot_given_speed,
                      SHOOT_2006_ID3_GIVEN_SPEED,
                      motor_can1_data[2].speed_rpm,
                      SHOOT_2006_ID3_GIVEN_CURRENT);






        osDelay(5);




    }

}



