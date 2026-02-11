//
// Created by 21481 on 2026/1/29.
//

#include "remote_control.h"
#include "string.h"
#include "usart.h"
#include "remote_control.h"



#if REMOTE_TYPE == SBUS

#define SBUS_HEAD 0X0F
#define SBUS_END 0X00

uint8_t rx_subs_buff[SBUS_BUFF_SIZE];
subs_remoter_t sbus_remoter;

void sbus_frame_parse(subs_remoter_t *remoter, uint8_t *buf)
{
    if ((buf[0] != SBUS_HEAD) || (buf[24] != SBUS_END))
        return;

    if (buf[23] == 0x0C)
        remoter->online = 0;
    else
        remoter->online = 1;

    remoter->rc.ch[0] = ((buf[1] | buf[2] << 8) & 0x07FF);
    remoter->rc.ch[1] = ((buf[2] >> 3 | buf[3] << 5) & 0x07FF);
    remoter->rc.ch[2] = ((buf[3] >> 6 | buf[4] << 2 | buf[5] << 10) & 0x07FF);
    remoter->rc.ch[3] = ((buf[5] >> 1 | buf[6] << 7) & 0x07FF);
    remoter->rc.ch[4] = ((buf[6] >> 4 | buf[7] << 4) & 0x07FF);
    remoter->rc.ch[5] = ((buf[7] >> 7 | buf[8] << 1 | buf[9] << 9) & 0x07FF);
    remoter->rc.ch[6] = ((buf[9] >> 2 | buf[10] << 6) & 0x07FF);
    remoter->rc.ch[7] = ((buf[10] >> 5 | buf[11] << 3) & 0x07FF);
    remoter->rc.ch[8] = ((buf[12] | buf[13] << 8) & 0x07FF);
    remoter->rc.ch[9] = ((buf[13] >> 3 | buf[14] << 5) & 0x07FF);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef * huart, uint16_t Size)
{

    if(huart->Instance == UART5)
    {
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
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef * huart)
{
    if(huart->Instance == UART5)
    {
        HAL_UARTEx_ReceiveToIdle_DMA(&huart5, rx_subs_buff, SBUS_BUFF_SIZE * 2); // 接收发生错误后重启
        memset(rx_subs_buff, 0, SBUS_BUFF_SIZE);							   // 清除接收缓存
    }
}

#endif

#if REMOTE_TYPE == DBUS

uint8_t rx_dbus_buff[DBUS_BUFF_SIZE];
dbus_ctrl_t dbus_remoter;

/**
  * @brief DBUS协议解析逻辑
  */
void dbus_frame_parse(dbus_ctrl_t *remoter, uint8_t *buf)
{
    if (buf == NULL) return;

    // 摇杆通道解析
    remoter->rc.ch[0] = (buf[0] | (buf[1] << 8)) & 0x07ff;
    remoter->rc.ch[1] = ((buf[1] >> 3) | (buf[2] << 5)) & 0x07ff;
    remoter->rc.ch[2] = ((buf[2] >> 6) | (buf[3] << 2) | (buf[4] << 10)) & 0x07ff;
    remoter->rc.ch[3] = ((buf[4] >> 1) | (buf[5] << 7)) & 0x07ff;

    // 减去中位偏移量量 (1024)
    remoter->rc.ch[0] -= RC_CH_VALUE_OFFSET;
    remoter->rc.ch[1] -= RC_CH_VALUE_OFFSET;
    remoter->rc.ch[2] -= RC_CH_VALUE_OFFSET;
    remoter->rc.ch[3] -= RC_CH_VALUE_OFFSET;

    // 开关解析
    remoter->rc.s[0] = (buf[5] >> 4) & 0x0003;      // 左侧开关
    remoter->rc.s[1] = ((buf[5] >> 4) & 0x000C) >> 2; // 右侧开关

    // 鼠标解析
    remoter->mouse.x = buf[6] | (buf[7] << 8);
    remoter->mouse.y = buf[8] | (buf[9] << 8);
    remoter->mouse.z = buf[10] | (buf[11] << 8);
    remoter->mouse.press_l = buf[12];
    remoter->mouse.press_r = buf[13];

    // 键盘按键
    remoter->key.v = buf[14] | (buf[15] << 8);

    // 侧边拨轮 (某些遥控器型号有)
    remoter->rc.ch[4] = (buf[16] | (buf[17] << 8)) - RC_CH_VALUE_OFFSET;

    remoter->online = 1;
}

/**
  * @brief HAL库串口接收完成回调（由空闲中断触发）
  */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    // 假设你使用的是 UART3，请根据实际连接修改 Instance
    if (huart->Instance == UART5)
    {
        // DBUS一帧固定18字节
        if (Size == DBUS_FRAME_SIZE)
        {
            dbus_frame_parse(&dbus_remoter, rx_dbus_buff);
        }

        // 重新开启DMA接收
        HAL_UARTEx_ReceiveToIdle_DMA(huart, rx_dbus_buff, DBUS_BUFF_SIZE);
        // 注意：H7开启了Cache时，可能需要在这里处理DCache的一致性 (SCB_InvalidateDCache_by_Addr)
    }
}

/**
  * @brief 串口错误回调
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == UART5)
    {
        HAL_UARTEx_ReceiveToIdle_DMA(huart, rx_dbus_buff, DBUS_BUFF_SIZE);
    }
}



#endif



