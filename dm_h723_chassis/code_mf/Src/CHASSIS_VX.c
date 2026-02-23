#include "cmsis_os.h"
#include "main.h"
#include "CHASSIS_TASK.h"
#include "pid.h"
#include "CHASSIS_VX.h"
#include "GET_RC_TASK.h"

//
// Created by 21481 on 2025/12/29.

pid_type_def chassis_vx_pid;



void CHASSIS_VX_TASK()
{
    while (1)
    {



        float chassis_vx_given_speed = 0.0f ;

        if(rcData.rc.s[1] == 2)
        {
//        遥控器控制目标角度
            angular_angle = (float )rcData.rc.ch[2]*0.02f;
        } else
        {
            if(rcData.rc.s[0] == 1)
            {
                chassis_vx_given_speed = (float )rcData.rc.ch[2]*0.001f;
                angular_angle = chassis_vx_pid_loop(chassis_vx_given_speed) + 4.8f ;
            }


        }


        osDelay(50);
    }
}


void chassis_vx_pid_init(void)
{
    static fp32 chassis_vx_kpkikd[3] = {CHASSIS_VX_PID_KP, CHASSIS_VX_PID_KI, CHASSIS_VX_PID_KD};
    PID_init(&chassis_vx_pid, PID_POSITION, chassis_vx_kpkikd, CHASSIS_VX_PID_OUT_MAX, CHASSIS_VX_PID_KI_MAX);

}

float chassis_vx_pid_loop(float chassis_vx_set_loop)
{
    PID_calc(&chassis_vx_pid, chassis_all_finial_state.chassis_speed_m_s, chassis_vx_set_loop);
    float chassis_vx_given_speed_loop = (float )(chassis_vx_pid.out);

    return chassis_vx_given_speed_loop ;

}
