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
        usart6_printf("%f,%f,%f,%f \r\n",
                      angular_angle,
                       angular_speed,
                      pitch_angle_from_bmi088,
                      (float )rc_ch3*0.02f);






        osDelay(5);




    }

}



