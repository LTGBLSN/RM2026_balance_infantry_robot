//
// Created by 21481 on 2026/1/30.
//




#include "main.h"
#include "fdcan.h"
#include "DJI_motors.h"


motor_measure_t motor_can1_data[7];
motor_measure_t motor_can2_data[7];



void FDCAN_DJI_motors(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4, uint16_t id, uint8_t can_channel)
{
    // 假设这些全局变量已经定义，或者在函数内部定义
    FDCAN_TxHeaderTypeDef tx_message;
    uint8_t can_send_data[8];

    // 1. 配置发送报文头部
    tx_message.Identifier = id;
    tx_message.IdType = FDCAN_STANDARD_ID;   // 标准 ID
    tx_message.TxFrameType = FDCAN_DATA_FRAME; // 数据帧
    tx_message.DataLength = FDCAN_DLC_BYTES_8; // 数据长度：8字节

    // 以下是 FDCAN 特有的配置项，因你用的是 Classic Mode，全设为 OFF/Disable
    tx_message.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_message.BitRateSwitch = FDCAN_BRS_OFF;      // 不开启速率切换
    tx_message.FDFormat = FDCAN_CLASSIC_CAN;       // 经典 CAN 模式
    tx_message.TxEventFifoControl = FDCAN_NO_TX_EVENTS; // 不保存发送事件
    tx_message.MessageMarker = 0;                  // 消息标记

    // 2. 填充数据（大端模式：高8位在前，低8位在后）
    can_send_data[0] = motor1 >> 8;
    can_send_data[1] = motor1;
    can_send_data[2] = motor2 >> 8;
    can_send_data[3] = motor2;
    can_send_data[4] = motor3 >> 8;
    can_send_data[5] = motor3;
    can_send_data[6] = motor4 >> 8;
    can_send_data[7] = motor4;

    // 3. 发送函数：FDCAN 使用的是 AddMessageToTxFifoQ
    if (can_channel == 1)
    {
        if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &tx_message, can_send_data) != HAL_OK)
        {
        }
    }
    else if (can_channel == 2)
    {
        if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &tx_message, can_send_data) != HAL_OK)
        {
        }
    } else if (can_channel == 3)
    {
        if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan3, &tx_message, can_send_data) != HAL_OK)
        {
        }
    }
}





