//
// Created by 21481 on 2026/2/14.
//



#include <string.h>
#include "remote_control.h"
#include "usart.h"
#include "referee.h"

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef * huart, uint16_t Size)
{

    if(huart->Instance == UART5)
    {

#if REMOTE_TYPE == SBUS
        if (Size <= SBUS_BUFF_SIZE)
        {
            HAL_UARTEx_ReceiveToIdle_DMA(&huart5, rx_subs_buff, SBUS_BUFF_SIZE * 2); // 接收完毕后重启
            sbus_frame_parse(&sbus_remoter, rx_subs_buff);
//			memset(rx_buff, 0, BUFF_SIZE);
        }
        else  // 接收数据长度大于BUFF_SIZE，错误处理
        {
            HAL_UARTEx_ReceiveToIdle_DMA(&huart5, rx_subs_buff, SBUS_BUFF_SIZE * 2); // 接收完毕后重启
            memset(rx_subs_buff, 0, SBUS_BUFF_SIZE);
        }
#endif

#if REMOTE_TYPE == DBUS
        // DBUS一帧固定18字节
        if (Size == DBUS_FRAME_SIZE)
        {
            dbus_frame_parse(&dbus_remoter, rx_dbus_buff);
        }

        // 重新开启DMA接收
        HAL_UARTEx_ReceiveToIdle_DMA(huart, rx_dbus_buff, DBUS_BUFF_SIZE);
        // 注意：H7开启了Cache时，可能需要在这里处理DCache的一致性 (SCB_InvalidateDCache_by_Addr)
#endif
    }
    if(huart->Instance == UART7)
    {
        // 2. 统计 flag 用于判断裁判系统是否在线（对应你 TIM 定时器里的在线监测逻辑）
        Referee.Flag++;
        Referee_Data_Process(Size);

        HAL_UARTEx_ReceiveToIdle_DMA(Referee.huart, Referee.p_Rx_Buffer, Referee.Rx_Buffer_Length);
    }




}


void HAL_UART_ErrorCallback(UART_HandleTypeDef * huart)
{
    if(huart->Instance == UART5)
    {
#if REMOTE_TYPE == SBUS
        HAL_UARTEx_ReceiveToIdle_DMA(&huart5, rx_subs_buff, SBUS_BUFF_SIZE * 2); // 接收发生错误后重启
        memset(rx_subs_buff, 0, SBUS_BUFF_SIZE);							   // 清除接收缓存
#endif

#if REMOTE_TYPE == DBUS
        HAL_UARTEx_ReceiveToIdle_DMA(huart, rx_dbus_buff, DBUS_BUFF_SIZE);
#endif

    }



}