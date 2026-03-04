//
// Created by 21481 on 2026/1/31.
//




#include "cmsis_os.h"
#include "remote_control.h"
#include "SHOOT_TASK.h"
#include "DJI_motors.h"
#include "pid.h"
#include "GET_RC_TASK.h"




void SHOOT_TASK()
{
    while (1)
    {

        //检查摩擦轮是否转动（这里可能有bug，比如子弹卡在摩擦轮不知道3508自己能不能拧出来）
        if( (motor_can1_data[0].speed_rpm > (int16_t)SHOOT_CHECK_FRICTION_WHEEL_SPEED) || (motor_can1_data[0].speed_rpm < (int16_t)-SHOOT_CHECK_FRICTION_WHEEL_SPEED) )
        {
            if(rcData.rc.ch[4] < -300)
            {
                SHOOT_2006_GIVEN_SPEED = SHOOT_TURN_ON_SPEED ;
            }
            else
            {
                SHOOT_2006_GIVEN_SPEED = 0 ;
            }


        }
        else
        {
            SHOOT_2006_GIVEN_SPEED = 0 ;
        }

        osDelay(1);
    }
}

