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
        //π€≤‚÷µ
//        usart1_printf("%f,%f,%f,%f,%f,%f,%f \r\n",
//                       chassis_all_finial_state.chassis_pitch_speed_rad_s,
//                      chassis_all_finial_state.pitch_angle_rad,
//                      chassis_all_finial_state.leg_length,
//                      chassis_all_finial_state.virtual_leg_angle_rad,
//                      chassis_all_finial_state.virtual_leg_speed_rad_s,
//                      chassis_all_finial_state.chassis_speed_m_s,
//                      chassis_all_finial_state.chassis_move_x_m
//                      );

        usart1_printf("%f,%f,%f \r\n",
                      right_leg_joint_2_leg_parameters.goal_virtual_leg_length,
                      right_leg_joint_2_leg_parameters.return_virtual_leg_length,
                      right_leg_joint_2_leg_parameters.virtual_leg_give_tor
                      );


        osDelay(1);
    }
}

