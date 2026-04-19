//
// Created by 21481 on 2026/2/12.
//

#include "DJI_motors.h"
#include "cmsis_os.h"
#include "ERROR_DETECTION.h"
#include "usart.h"

void ERROR_DETECTION()
{
    while (1)
    {


        rc_connection_status();//遥控器离线判断
        pitch_6020_status();//pitch电机过温判断




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



void pitch_6020_status()
{
    if(GM6020_TEMP_MAX > motor_can1_data[5].temperate)//观测值需要改
    {
        pitch_6020_state = GM6020_SAFE ;//安全
    }
    else
    {
        pitch_6020_state = GM6020_DIE ;//过温
    }

}
