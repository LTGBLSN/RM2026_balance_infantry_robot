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
            {//起立但轮毂不动
                {

//                    FDCAN_DJI_motors(0, 0, 0, 0, 0x200, CAN_CHANNEL_2);
//                    FDCAN_DJI_motors(0, 0, 0, 0, 0x1FF, CAN_CHANNEL_2);

                    FDCAN_DJI_motors(0, 0, SHOOT_2006_ID3_GIVEN_CURRENT, 0, 0x200, CAN_CHANNEL_2);//底盘12拨弹3
                    FDCAN_DJI_motors(YAW_6020_ID1_GIVEN_CURRENT, 0, 0, 0, 0x1FF, CAN_CHANNEL_2);//yaw轴1
                    DM_CAN_SENT(DM_GIVE_CURRENT);
                    break;
                }
            }
            case 1:
            {//直立
                FDCAN_DJI_motors(chassis_right_3508_id1_given_current, chassis_left_3508_id2_given_current, SHOOT_2006_ID3_GIVEN_CURRENT, 0, 0x200, CAN_CHANNEL_2);//底盘12拨弹3
                FDCAN_DJI_motors(YAW_6020_ID1_GIVEN_CURRENT, 0, 0, 0, 0x1FF, CAN_CHANNEL_2);//yaw轴1
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
    if(DM8009P_01_RIGHT_BIG_LEG_FRONT.online_state == DM_MOTOR_SAFE)
    {
        switch (DM_can_sent_state)
        {
            case DM_GIVE_CURRENT:
            {
                Dm_Can_Send(DM8009P_01_RIGHT_BIG_LEG_FRONT.can_channel, DM8009P_01_RIGHT_BIG_LEG_FRONT.can_id, DM8009P_01_RIGHT_BIG_LEG_FRONT.motor_type, DM8009P_01_RIGHT_BIG_LEG_FRONT.give_tor);
                break;
            }
            case DM_NO_CURRENT:
            {
                Dm_Can_Send(DM8009P_01_RIGHT_BIG_LEG_FRONT.can_channel, DM8009P_01_RIGHT_BIG_LEG_FRONT.can_id, DM8009P_01_RIGHT_BIG_LEG_FRONT.motor_type, 0.0f);
                break;
            }
            default:
            {
                break;
            }
        }
    }
    else//电机保活
    {
        dm_motor_mode_set(CMD_ENABLE_MODE, CAN_CHANNEL_1,DM8009P_01_RIGHT_BIG_LEG_FRONT.can_id);
    }


    //第二个电机
    if(DM8009P_02_LEFT_BIG_LEG_FRONT.online_state == DM_MOTOR_SAFE)
    {
        switch (DM_can_sent_state)
        {
            case DM_GIVE_CURRENT:
            {
                Dm_Can_Send(DM8009P_02_LEFT_BIG_LEG_FRONT.can_channel, DM8009P_02_LEFT_BIG_LEG_FRONT.can_id, DM8009P_02_LEFT_BIG_LEG_FRONT.motor_type, DM8009P_02_LEFT_BIG_LEG_FRONT.give_tor);
                break;
            }
            case DM_NO_CURRENT:
            {
                Dm_Can_Send(DM8009P_02_LEFT_BIG_LEG_FRONT.can_channel, DM8009P_02_LEFT_BIG_LEG_FRONT.can_id, DM8009P_02_LEFT_BIG_LEG_FRONT.motor_type, 0.0f);
                break;
            }
            default:
            {
                break;
            }
        }
    }
    else//电机保活
    {
        dm_motor_mode_set(CMD_ENABLE_MODE, CAN_CHANNEL_1,DM8009P_02_LEFT_BIG_LEG_FRONT.can_id);
    }

    //第三个电机
    if(DM8009P_03_LEFT_SMALL_LEG_BEHIND.online_state == DM_MOTOR_SAFE)
    {
        switch (DM_can_sent_state)
        {
            case DM_GIVE_CURRENT:
            {
                Dm_Can_Send(DM8009P_03_LEFT_SMALL_LEG_BEHIND.can_channel, DM8009P_03_LEFT_SMALL_LEG_BEHIND.can_id, DM8009P_03_LEFT_SMALL_LEG_BEHIND.motor_type, DM8009P_03_LEFT_SMALL_LEG_BEHIND.give_tor);
                break;
            }
            case DM_NO_CURRENT:
            {
                Dm_Can_Send(DM8009P_03_LEFT_SMALL_LEG_BEHIND.can_channel, DM8009P_03_LEFT_SMALL_LEG_BEHIND.can_id, DM8009P_03_LEFT_SMALL_LEG_BEHIND.motor_type, 0.0f);
                break;
            }
            default:
            {
                break;
            }
        }
    }
    else//电机保活
    {
        dm_motor_mode_set(CMD_ENABLE_MODE, CAN_CHANNEL_1,DM8009P_03_LEFT_SMALL_LEG_BEHIND.can_id);
    }


    //第四个电机
    if(DM8009P_04_RIGHT_SMALL_LEG_BEHIND.online_state == DM_MOTOR_SAFE)
    {
        switch (DM_can_sent_state)
        {
            case DM_GIVE_CURRENT:
            {
                Dm_Can_Send(DM8009P_04_RIGHT_SMALL_LEG_BEHIND.can_channel, DM8009P_04_RIGHT_SMALL_LEG_BEHIND.can_id, DM8009P_04_RIGHT_SMALL_LEG_BEHIND.motor_type, DM8009P_04_RIGHT_SMALL_LEG_BEHIND.give_tor);
                break;
            }
            case DM_NO_CURRENT:
            {
                Dm_Can_Send(DM8009P_04_RIGHT_SMALL_LEG_BEHIND.can_channel, DM8009P_04_RIGHT_SMALL_LEG_BEHIND.can_id, DM8009P_04_RIGHT_SMALL_LEG_BEHIND.motor_type, 0.0f);
                break;
            }
            default:
            {
                break;
            }
        }
    }
    else//电机保活
    {
        dm_motor_mode_set(CMD_ENABLE_MODE, CAN_CHANNEL_1,DM8009P_04_RIGHT_SMALL_LEG_BEHIND.can_id);
    }


}
