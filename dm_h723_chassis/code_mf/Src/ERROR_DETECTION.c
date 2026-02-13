//
// Created by 21481 on 2026/2/12.
//

#include "DJI_motors.h"
#include "cmsis_os.h"
#include "ERROR_DETECTION.h"
#include "dm_motor.h"

void ERROR_DETECTION()
{
    while (1)
    {

        rc_connection_status();//遥控器离线判断
        yaw_6020_status();//pitch电机过温判断
        DM_motor_status();//达妙电机离线判断
        //暂无自瞄通讯判断




        osDelay(1);
    }


}





void rc_connection_status()
{
//    if(HAL_GetTick() - rc_receive_time > RC_NO_DATA_TIMEOUT)
//    {
//        rc_receive_state = RC_OFFLINE ;//遥控器离线
//    }
//    else
//    {
//        rc_receive_state = RC_ONLINE ;//遥控器在线
//    }
}



void yaw_6020_status()
{
    if(GM6020_TEMP_MAX > motor_can2_data[4].temperate)//观测值需要改
    {
        yaw_6020_state = GM6020_SAFE ;//安全
    }
    else
    {
        yaw_6020_state = GM6020_DIE ;//过温
    }

}

void DM_motor_status()
{
    //第一个电机
    if((HAL_GetTick() - DM8009_01.last_online_time) > DM_MOTOR_CHECK_TIME )
    {
        DM8009_01.online_state = DM_MOTOR_DIE ;//离线
    }
    else
    {
        DM8009_01.online_state = DM_MOTOR_SAFE;//在线

    }


    //第二个电机
    if((HAL_GetTick() - DM8009_02.last_online_time) > DM_MOTOR_CHECK_TIME )
    {
        DM8009_02.online_state = DM_MOTOR_DIE ;//离线
    }
    else
    {
        DM8009_02.online_state = DM_MOTOR_SAFE;//在线

    }

    //第三个电机
    if((HAL_GetTick() - DM8009_03.last_online_time) > DM_MOTOR_CHECK_TIME )
    {
        DM8009_03.online_state = DM_MOTOR_DIE ;//离线
    }
    else
    {
        DM8009_03.online_state = DM_MOTOR_SAFE;//在线

    }



    //第四个电机
    if((HAL_GetTick() - DM8009_04.last_online_time) > DM_MOTOR_CHECK_TIME )
    {
        DM8009_04.online_state = DM_MOTOR_DIE ;//离线
    }
    else
    {
        DM8009_04.online_state = DM_MOTOR_SAFE;//在线

    }


}




