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

void DM_CAN_SENT()
{
    while (1)
    {
        dm_1_4_can1_cmd(DM8009P_01_LEFT_FRONT.give_tor,
                        DM8009P_02_RIGHT_FRONT.give_tor,
                        DM8009P_03_RIGHT_BEHIND.give_tor,
                        DM8009P_04_LEFT_BEHIND.give_tor);//妈妈生的，一拖四真好用，沟槽f4can资源真少

        CAN1_cmd_chassis_shoot(0,0,0,0);

        osDelay(1);
    }

}


void DJI_CAN_SENT()
{
    while (1)
    {
////        首先尝试直接发送
//        if(CAN1_cmd_chassis_shoot(0,0,0,0) == HAL_OK)
//        {
//
//        } else
//        {
//            //存入队列
//
//        }

        osDelay(1);
    }

}




