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

void can_sent()
{
    while (1)
    {

        Dm_Can_Send(DM8009P_01_RIGHT_FRONT);
        Dm_Can_Send(DM8009P_02_RIGHT_FRONT);
        Dm_Can_Send(DM8009P_03_RIGHT_FRONT);
        Dm_Can_Send(DM8009P_04_RIGHT_FRONT);
        osDelay(1);
    }

}




