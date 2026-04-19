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
#include "CHASSIS_TASK.h"

void DEBUG_UART_TASK()
{
    while (1)
    {
        usart1_printf("%d \r\n",rcData.rc.ch[4]);

//        usart1_printf("%f,%f \r\n",left_leg_joint_2_leg_parameters.fly_state,left_leg_joint_2_leg_parameters.support_force);




//        usart1_printf("%f,%f,%f,%f,%f,%f,%f \r\n",
//                      e0,
//                      e1,
//                      e2,
//                      e3,
//                      e4,
//                      e5,
//                      chassis_vx_given_speed
//                      );



        osDelay(1);
    }
}

