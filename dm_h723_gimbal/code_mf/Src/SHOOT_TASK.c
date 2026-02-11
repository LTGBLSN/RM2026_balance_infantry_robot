//
// Created by 21481 on 2026/1/31.
//




#include "cmsis_os.h"
#include "remote_control.h"
#include "SHOOT_TASK.h"
#include "DJI_motors.h"
#include "pid.h"
#include "GET_RC_TASK.h"


pid_type_def shoot_2006_ID1_speed_pid;

void SHOOT_TASK()
{
    while (1)
    {




        shoot_pid_control();

        osDelay(1);
    }
}

void SHOOOT_STOP_CHECK()
{
    while (1)
    {
        shoot_speed_compute();//目标速度计算
        shoot_stop_check();

        osDelay(1);
    }
}




void shoot_speed_compute()
{
    if(rcData.rc.s[1] == 2)
    {
        SHOOT_2006_ID1_GIVEN_SPEED = 2500 ;
    } else if(rcData.rc.s[1] == 1)
    {
        SHOOT_2006_ID1_GIVEN_SPEED = 9257 ;
    } else if(rcData.rc.s[1] == 3)
    {
        SHOOT_2006_ID1_GIVEN_SPEED = 6171 ;
    }

}

void shoot_stop_check()
{
    if(SHOOT_2006_ID1_GIVEN_SPEED > 0)
    {

        //如果卡住了//待更新，卡弹检测灵敏度不够，存才缓慢旋转
        if(motor_can1_data[0].speed_rpm == 0)
        {
            osDelay(SHOOT_SPEED_CHECK_TIME);
            if(motor_can1_data[0].speed_rpm == 0)
            {
                SHOOT_2006_ID1_GIVEN_SPEED = SHOOT_TURN_OFF_SPEED ;
                osDelay(SHOOT_TURN_OFF_TIME);
                shoot_speed_compute();//目标速度计算
            }
        }


    }
}





void shoot_pid_control()
{
    //shoot
    SHOOT_2006_ID1_GIVEN_CURRENT = shoot_2006_id1_speed_pid_loop(SHOOT_2006_ID1_GIVEN_SPEED);
}







//shoot
void shoot_2006_id1_speed_pid_init(void)
{
    static fp32 shoot_2006_id1_speed_kpkikd[3] = {SHOOT_2006_ID1_SPEED_PID_KP, SHOOT_2006_ID1_SPEED_PID_KI, SHOOT_2006_ID1_SPEED_PID_KD};
    PID_init(&shoot_2006_ID1_speed_pid, PID_POSITION, shoot_2006_id1_speed_kpkikd, SHOOT_2006_ID1_SPEED_PID_OUT_MAX, SHOOT_2006_ID1_SPEED_PID_KI_MAX);

}

int16_t shoot_2006_id1_speed_pid_loop(float shoot_2006_ID1_speed_set_loop)
{
    PID_calc(&shoot_2006_ID1_speed_pid, motor_can1_data[0].speed_rpm , shoot_2006_ID1_speed_set_loop);
    int16_t shoot_2006_id1_given_current_loop = (int16_t)(shoot_2006_ID1_speed_pid.out);

    return shoot_2006_id1_given_current_loop ;

}



