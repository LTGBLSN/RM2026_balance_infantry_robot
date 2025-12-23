//
// Created by 21481 on 2025/3/17.
//
#include "main.h"
#include "cmsis_os.h"
#include "can.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

#include "board_LED.h"
#include "uart_printf.h"
#include "uart_sent.h"
#include "bsp_can.h"
#include "CAN_receive.h"
#include "can_sent.h"
#include "dm_motor.h"
#include "error_detection.h"

void DM_CAN_SENT()
{
    osDelay(2500);
    while (1)
    {
        //底盘控制频率500hz
        if(rc_receive_state == RC_ONLINE)
        {
            if(rc_s0 == 3 )
            {
                Dm_Can_Send(DM8009P_01_LEFT_FRONT);
                Dm_Can_Send(DM8009P_02_RIGHT_FRONT);

                osDelay(1);//必须间隔，否则发送失败，邮箱深度为3
                CAN1_cmd_chassis_shoot(0,0,0,0);
                Dm_Can_Send(DM8009P_03_RIGHT_BEHIND);
                Dm_Can_Send(DM8009P_04_LEFT_BEHIND);
                osDelay(1);

            }
            else if(rc_s0 == 1 )
            {
                Dm_Can_Send(DM8009P_01_LEFT_FRONT);
                Dm_Can_Send(DM8009P_02_RIGHT_FRONT);


                osDelay(1);//必须间隔，否则发送失败，邮箱深度为3
                CAN1_cmd_chassis_shoot(chassis_right_3508_id1_given_current,chassis_left_3508_id2_given_current,0,0);
                Dm_Can_Send(DM8009P_03_RIGHT_BEHIND);
                Dm_Can_Send(DM8009P_04_LEFT_BEHIND);
                osDelay(1);
            }
            else
            {
                DM_ALL_STOP();
            }
        }
         else
        {
            DM_ALL_STOP();
        }


    }

}


void DM_ALL_STOP()
{
    struct dm_motor DM_STOP ;

    osDelay(1);
    DM_STOP.can_id = 0x01 ;
    DM_STOP.give_tor = 0 ;
    DM_STOP.motor_type = DM8009P ;
    DM_STOP.can_channel = 0x01 ;
    Dm_Can_Send(DM_STOP);

    osDelay(1);
    DM_STOP.can_id = 0x02 ;
    DM_STOP.give_tor = 0 ;
    DM_STOP.motor_type = DM8009P ;
    DM_STOP.can_channel = 0x01 ;
    Dm_Can_Send(DM_STOP);
    osDelay(1);

    osDelay(1);
    DM_STOP.can_id = 0x03 ;
    DM_STOP.give_tor = 0 ;
    DM_STOP.motor_type = DM8009P ;
    DM_STOP.can_channel = 0x01 ;
    Dm_Can_Send(DM_STOP);
    osDelay(1);


    osDelay(1);
    DM_STOP.can_id = 0x04 ;
    DM_STOP.give_tor = 0 ;
    DM_STOP.motor_type = DM8009P ;
    DM_STOP.can_channel = 0x01 ;
    Dm_Can_Send(DM_STOP);
    osDelay(1);

    CAN1_cmd_chassis_shoot(0,0,0,0);
}


