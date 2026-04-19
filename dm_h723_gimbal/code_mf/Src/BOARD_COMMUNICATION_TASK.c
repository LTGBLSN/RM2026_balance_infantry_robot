//
// Created by 21481 on 2026/3/2.
//

#include "main.h"
#include "fdcan.h"
#include "cmsis_os.h"
#include "BOARD_COMMUNICATION_TASK.h"
#include "can_receive.h"
#include "GET_RC_TASK.h"

void BOARD_COMMUNICATION_TASK()
{
    while (1)
    {
        gimbal_rc_2_chassis(YAW_6020_ID1_GIVEN_CURRENT,
                            rcData.rc.s[0],
                            rcData.rc.ch[0],
                            rcData.rc.ch[1],
                            0xA1,
                            CAN_CHANNEL_2);
        gimbal_rc_2_chassis(SHOOT_2006_GIVEN_SPEED,
                            rcData.rc.s[1],
                            rcData.rc.ch[2],
                            rcData.rc.ch[3],
                            0xA2,
                            CAN_CHANNEL_2);
        gimbal_rc_2_chassis_uint(rcData.key.v,
                            rcData.rc.ch[4],
                            0,
                            0,
                            0xA3,
                            CAN_CHANNEL_2);

//        gimbal_rc_2_chassis(SHOOT_2006_GIVEN_SPEED,
//                            rcData.rc.s[1],
//                            rcData.rc.ch[2],
//                            rcData.rc.ch[3],
//                            0xA2,
//                            CAN_CHANNEL_2);



//        gimbal_2_chassis_A(
//                YAW_6020_ID1_GIVEN_CURRENT,
//                rcData.rc.ch[1],//chassis_vx
//                rcData.rc.ch[2],//chassis_vround
//                state_00,
//                state_01,
//                0xA1,
//                CAN_CHANNEL_1);



        osDelay(1);
    }
}


uint8_t state_convert(int16_t data_01 ,int16_t data_02 , int16_t data_03 ,int16_t data_04 )
{
    uint8_t chassis_motor_key_state  ;//底盘是否断电（0断电 1上电）
    uint8_t chassis_vround_mode_state ;//底盘小陀螺还是跟随云台（0跟随云台 1小陀螺）
    uint8_t chassis_stand_mode_state ;//底盘直立模式（0为板凳轮毂不动，1为正常lqr直立）



    uint8_t final_all_state = 0b00000000;

    return final_all_state ;
}


void gimbal_2_chassis_A(int16_t yaw_6020,int16_t rc_01 ,int16_t rc_02 ,uint8_t state_01 ,uint8_t state_02, uint16_t id, uint8_t can_channel)
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
    can_send_data[0] = yaw_6020 >> 8;
    can_send_data[1] = yaw_6020;
    can_send_data[2] = rc_01 >> 8;
    can_send_data[3] = rc_01;
    can_send_data[4] = rc_02 >> 8;
    can_send_data[5] = rc_02;
    can_send_data[6] = state_01;
    can_send_data[7] = state_02;

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




void gimbal_rc_2_chassis(int16_t data , int16_t rc_s , int16_t rc_ch_A , int16_t rc_ch_B , uint16_t id, uint8_t can_channel)
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
    can_send_data[0] = data >> 8;
    can_send_data[1] = data;
    can_send_data[2] = rc_s >> 8;
    can_send_data[3] = rc_s;
    can_send_data[4] = rc_ch_A >> 8;
    can_send_data[5] = rc_ch_A;
    can_send_data[6] = rc_ch_B >> 8;;
    can_send_data[7] = rc_ch_B;

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


void gimbal_rc_2_chassis_uint(uint16_t data , int16_t rc_s , uint16_t rc_ch_A , uint16_t rc_ch_B , uint16_t id, uint8_t can_channel)
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
    can_send_data[0] = data >> 8;
    can_send_data[1] = data;
    can_send_data[2] = rc_s >> 8;
    can_send_data[3] = rc_s;
    can_send_data[4] = rc_ch_A >> 8;
    can_send_data[5] = rc_ch_A;
    can_send_data[6] = rc_ch_B >> 8;;
    can_send_data[7] = rc_ch_B;

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


