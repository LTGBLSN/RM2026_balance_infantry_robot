//
// Created by 21481 on 2026/1/30.
//



#include "cmsis_os.h"
#include "DJI_motors.h"
#include "remote_control.h"
#include "can_receive.h"
#include "dm_motor.h"
#include "GET_RC_TASK.h"
#include "ERROR_DETECTION.h"
#include "CAN_SENT_TASK.h"

void CAN_SENT_TASK()
{


    while (1)
    {
        switch (rcData.rc.s[0])
        {
            case 3:
            case 1:
            {
                FDCAN_DJI_motors(0, 0, 0, 0, 0x200, CAN_CHANNEL_2);//底盘12拨弹3
                FDCAN_DJI_motors(8000, 0, 0, 0, 0x1FF, CAN_CHANNEL_2);//yaw轴1
                DM_CAN_SENT(DM_GIVE_CURRENT);
                break;
            }
            default:
            {
                FDCAN_DJI_motors(0, 0, 0, 0, 0x200, CAN_CHANNEL_2);
                FDCAN_DJI_motors(0, 0, 0, 0, 0x1FF, CAN_CHANNEL_2);
                DM_CAN_SENT(DM_NO_CURRENT);



                break;
            }


        }

        osDelay(1);
    }
}


void DM_CAN_SENT(uint8_t DM_can_sent_state)
{
    //第一个电机
    if(DM8009_01.online_state == DM_MOTOR_SAFE)
    {
        switch (DM_can_sent_state)
        {
            case DM_GIVE_CURRENT:
            {
                Dm_Can_Send(DM8009_01.can_channel,DM8009_01.can_id,DM8009_01.motor_type,DM8009_01.give_tor);
            }
            case DM_NO_CURRENT:
            {
                Dm_Can_Send(DM8009_01.can_channel,DM8009_01.can_id,DM8009_01.motor_type,0.0f);
            }
        }
    }

    //第二个电机
    if(DM8009_02.online_state == DM_MOTOR_SAFE)
    {
        switch (DM_can_sent_state)
        {
            case DM_GIVE_CURRENT:
            {
                Dm_Can_Send(DM8009_02.can_channel,DM8009_02.can_id,DM8009_02.motor_type,DM8009_02.give_tor);
            }
            case DM_NO_CURRENT:
            {
                Dm_Can_Send(DM8009_02.can_channel,DM8009_02.can_id,DM8009_02.motor_type,0.0f);
            }
        }
    }


    //第三个电机
    if(DM8009_03.online_state == DM_MOTOR_SAFE)
    {
        switch (DM_can_sent_state)
        {
            case DM_GIVE_CURRENT:
            {
                Dm_Can_Send(DM8009_03.can_channel,DM8009_03.can_id,DM8009_03.motor_type,DM8009_03.give_tor);
            }
            case DM_NO_CURRENT:
            {
                Dm_Can_Send(DM8009_03.can_channel,DM8009_03.can_id,DM8009_03.motor_type,0.0f);
            }
        }
    }


    //第四个电机
    if(DM8009_04.online_state == DM_MOTOR_SAFE)
    {
        switch (DM_can_sent_state)
        {
            case DM_GIVE_CURRENT:
            {
                Dm_Can_Send(DM8009_04.can_channel,DM8009_04.can_id,DM8009_04.motor_type,DM8009_04.give_tor);
            }
            case DM_NO_CURRENT:
            {
                Dm_Can_Send(DM8009_04.can_channel,DM8009_04.can_id,DM8009_04.motor_type,0.0f);
            }
        }
    }



    else//电机保活
    {
        dm_motor_mode_set(CMD_ENABLE_MODE, CAN_CHANNEL_1,0x01);
        dm_motor_mode_set(CMD_ENABLE_MODE, CAN_CHANNEL_1,0x02);
        dm_motor_mode_set(CMD_ENABLE_MODE, CAN_CHANNEL_1,0x03);
        dm_motor_mode_set(CMD_ENABLE_MODE, CAN_CHANNEL_1,0x04);
        //行不行啊我靠不知道这h7一块发会不会一块睡觉
        //牛逼啊可以一块发
    }
}


