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


void uart_sent_debug()
{
    while (1)
    {
        usart6_printf("%f,%f \r\n",
                      pitch_angle_from_bmi088,
                      angular_speed);


//        usart6_printf("%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f \r\n",
//                      DM8009P_01_LEFT_FRONT.return_angle,
//                      DM8009P_01_LEFT_FRONT.give_speed,
//                      DM8009P_01_LEFT_FRONT.return_speed_rpm,
//                      DM8009P_02_RIGHT_FRONT.return_angle,
//                      DM8009P_02_RIGHT_FRONT.give_speed,
//                      DM8009P_02_RIGHT_FRONT.return_speed_rpm,
//                      DM8009P_03_RIGHT_BEHIND.return_angle,
//                      DM8009P_03_RIGHT_BEHIND.give_speed,
//                      DM8009P_03_RIGHT_BEHIND.return_speed_rpm,
//                      DM8009P_04_LEFT_BEHIND.return_angle,
//                      DM8009P_04_LEFT_BEHIND.give_speed,
//                      DM8009P_04_LEFT_BEHIND.return_speed_rpm);




        osDelay(5);




    }

}



