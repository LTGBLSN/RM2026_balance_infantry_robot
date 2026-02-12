//
// Created by 21481 on 2026/2/12.
//

#include "main.h"
#include "AUTO_AIM_TASK.h"
#include "usart.h"


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart7)
    {

        auto_aim_communication_data_parse(uart7_receive_data);//自瞄数据解析


        HAL_UART_Receive_DMA(&huart7, &uart7_receive_data, 1);//继续进行中断接收



    }
}

