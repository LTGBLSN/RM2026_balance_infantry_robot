//
// Created by 21481 on 2025/12/15.
//
#include <string.h>
#include "dm_motor.h"
#include "can_receive.h"
#include "main.h"
#include "fdcan.h"

struct dm_motor DM4340_01 ;
void dm_motor_init()
{
    memset(&DM4340_01, 0, sizeof(DM4340_01));//清零

    //第一个电机
    DM4340_01.motor_type = DM4340 ;
    DM4340_01.can_id = 0x01 ;
    DM4340_01.can_master_id = 0x11 ;
    DM4340_01.can_channel = CAN_CHANNEL_1 ;
    DM4340_01.give_tor = 0.4f ;



}

// 电机模式切换
void dm_motor_mode_set(uint8_t cmd , uint8_t can_channel , uint8_t can_id)
{
    uint8_t buf[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00};
    switch(cmd)
    {
        case CMD_ENABLE_MODE://
            buf[7] = 0xFC;
            break;

        case CMD_DISABLE_MODE://
            buf[7] = 0xFD;
            break;

        case CMD_ZERO_POSITION://设置零点
            buf[7] = 0xFE;
            break;

        default:
            return; /* 直接退出函数 */
    }
    DMCanTransmit(buf, sizeof(buf), can_channel, can_id);
}


/**
  * @brief  Can总线发送控制参数
  * @param
  * @retval
  */
//p:期望位置 v:期望速度 kp: kd: t:前馈扭矩
void Dm_Can_Send( uint8_t can_channel , uint8_t can_id , uint8_t motor_type , float give_tor )
{
    float f_p = 0.0f ;
    float f_v = 0.0f ;
    float f_kp = 0.0f ;
    float f_kd = 0.0f ;
    float f_t = give_tor;

    uint16_t p, v, kp, kd, t;//最终发送，经过转换的
    uint8_t buf[8];

    /* 限制输入的参数在定义的范围内 */
    switch (motor_type) {

        case DM8009P:
        {
            LIMIT_MIN_MAX(f_p,  DM8009P_P_MIN,  DM8009P_P_MAX);
            LIMIT_MIN_MAX(f_v,  DM8009P_V_MIN,  DM8009P_V_MAX);
            LIMIT_MIN_MAX(f_kp, DM8009P_KP_MIN, DM8009P_KP_MAX);
            LIMIT_MIN_MAX(f_kd, DM8009P_KD_MIN, DM8009P_KD_MAX);
            LIMIT_MIN_MAX(f_t,  DM8009P_T_MIN,  DM8009P_T_MAX);
            /* 根据协议，对float参数进行转换 */
            p = float_to_uint(f_p,      DM8009P_P_MIN,  DM8009P_P_MAX,  16);
            v = float_to_uint(f_v,      DM8009P_V_MIN,  DM8009P_V_MAX,  12);
            kp = float_to_uint(f_kp,    DM8009P_KP_MIN, DM8009P_KP_MAX, 12);
            kd = float_to_uint(f_kd,    DM8009P_KD_MIN, DM8009P_KD_MAX, 12);
            t = float_to_uint(f_t,      DM8009P_T_MIN,  DM8009P_T_MAX,  12);
            break;
        }

        case DM4310:
        {
            LIMIT_MIN_MAX(f_p,  DM4310_P_MIN,  DM4310_P_MAX);
            LIMIT_MIN_MAX(f_v,  DM4310_V_MIN,  DM4310_V_MAX);
            LIMIT_MIN_MAX(f_kp, DM4310_KP_MIN, DM4310_KP_MAX);
            LIMIT_MIN_MAX(f_kd, DM4310_KD_MIN, DM4310_KD_MAX);
            LIMIT_MIN_MAX(f_t,  DM4310_T_MIN,  DM4310_T_MAX);
            /* 根据协议，对float参数进行转换 */
            p = float_to_uint(f_p,      DM4310_P_MIN,  DM4310_P_MAX,  16);
            v = float_to_uint(f_v,      DM4310_V_MIN,  DM4310_V_MAX,  12);
            kp = float_to_uint(f_kp,    DM4310_KP_MIN, DM4310_KP_MAX, 12);
            kd = float_to_uint(f_kd,    DM4310_KD_MIN, DM4310_KD_MAX, 12);
            t = float_to_uint(f_t,      DM4310_T_MIN,  DM4310_T_MAX,  12);
            break;
        }

        case DM3507:
        {
            LIMIT_MIN_MAX(f_p,  DM3507_P_MIN,  DM3507_P_MAX);
            LIMIT_MIN_MAX(f_v,  DM3507_V_MIN,  DM3507_V_MAX);
            LIMIT_MIN_MAX(f_kp, DM3507_KP_MIN, DM3507_KP_MAX);
            LIMIT_MIN_MAX(f_kd, DM3507_KD_MIN, DM3507_KD_MAX);
            LIMIT_MIN_MAX(f_t,  DM3507_T_MIN,  DM3507_T_MAX);
            /* 根据协议，对float参数进行转换 */
            p = float_to_uint(f_p,      DM3507_P_MIN,  DM3507_P_MAX,  16);
            v = float_to_uint(f_v,      DM3507_V_MIN,  DM3507_V_MAX,  12);
            kp = float_to_uint(f_kp,    DM3507_KP_MIN, DM3507_KP_MAX, 12);
            kd = float_to_uint(f_kd,    DM3507_KD_MIN, DM3507_KD_MAX, 12);
            t = float_to_uint(f_t,      DM3507_T_MIN,  DM3507_T_MAX,  12);
            break;
        }

        case DM4340:
        {
            LIMIT_MIN_MAX(f_p,  DM4340_P_MIN,  DM4340_P_MAX);
            LIMIT_MIN_MAX(f_v,  DM4340_V_MIN,  DM4340_V_MAX);
            LIMIT_MIN_MAX(f_kp, DM4340_KP_MIN, DM4340_KP_MAX);
            LIMIT_MIN_MAX(f_kd, DM4340_KD_MIN, DM4340_KD_MAX);
            LIMIT_MIN_MAX(f_t,  DM4340_T_MIN,  DM4340_T_MAX);
            /* 根据协议，对float参数进行转换 */
            p = float_to_uint(f_p,      DM4340_P_MIN,  DM4340_P_MAX,  16);
            v = float_to_uint(f_v,      DM4340_V_MIN,  DM4340_V_MAX,  12);
            kp = float_to_uint(f_kp,    DM4340_KP_MIN, DM4340_KP_MAX, 12);
            kd = float_to_uint(f_kd,    DM4340_KD_MIN, DM4340_KD_MAX, 12);
            t = float_to_uint(f_t,      DM4340_T_MIN,  DM4340_T_MAX,  12);
            break;
        }
        case XIAOMI:
        {
            LIMIT_MIN_MAX(f_p,  XIAOMI_P_MIN,  XIAOMI_P_MAX);
            LIMIT_MIN_MAX(f_v,  XIAOMI_V_MIN,  XIAOMI_V_MAX);
            LIMIT_MIN_MAX(f_kp, XIAOMI_KP_MIN, XIAOMI_KP_MAX);
            LIMIT_MIN_MAX(f_kd, XIAOMI_KD_MIN, XIAOMI_KD_MAX);
            LIMIT_MIN_MAX(f_t,  XIAOMI_T_MIN,  XIAOMI_T_MAX);
            /* 根据协议，对float参数进行转换 */
            p = float_to_uint(f_p,      XIAOMI_P_MIN,  XIAOMI_P_MAX,  16);
            v = float_to_uint(f_v,      XIAOMI_V_MIN,  XIAOMI_V_MAX,  12);
            kp = float_to_uint(f_kp,    XIAOMI_KP_MIN, XIAOMI_KP_MAX, 12);
            kd = float_to_uint(f_kd,    XIAOMI_KD_MIN, XIAOMI_KD_MAX, 12);
            t = float_to_uint(f_t,      XIAOMI_T_MIN,  XIAOMI_T_MAX,  12);
            break;
        }
        default:
        {
            break;
        }

    }

    /* 根据传输协议，把数据转换为CAN命令数据字段 */
    buf[0] = p>>8;
    buf[1] = p&0xFF;
    buf[2] = v>>4;
    buf[3] = ((v&0xF)<<4)|(kp>>8);
    buf[4] = kp&0xFF;
    buf[5] = kd>>4;
    buf[6] = ((kd&0xF)<<4)|(t>>8);
    buf[7] = t&0xff;

    /* 通过CAN接口把buf中的内容发送出去 */
    DMCanTransmit(buf, sizeof(buf), can_channel, can_id);
}




//下半部分为复用函数

/**
  * @brief          根据长度数字返回 FDCAN 库需要的 DLC 宏
  */
static uint32_t Get_FDCAN_DLC(uint8_t len)
{
    switch (len)
    {
        case 0: return FDCAN_DLC_BYTES_0;
        case 1: return FDCAN_DLC_BYTES_1;
        case 2: return FDCAN_DLC_BYTES_2;
        case 3: return FDCAN_DLC_BYTES_3;
        case 4: return FDCAN_DLC_BYTES_4;
        case 5: return FDCAN_DLC_BYTES_5;
        case 6: return FDCAN_DLC_BYTES_6;
        case 7: return FDCAN_DLC_BYTES_7;
        case 8: return FDCAN_DLC_BYTES_8;
        default: return FDCAN_DLC_BYTES_8; // 默认返回8
    }
}

/**
  * @brief          把buf中的内容通过FDCAN接口发送出去 (适配 H7)
  */
static void DMCanTransmit(uint8_t *buf, uint8_t len, uint8_t can_channel, uint8_t motor_id)
{
    // 参数合法性检查
    if (buf == NULL || len == 0 || len > 8) return;

    FDCAN_TxHeaderTypeDef TxHead;

    // 1. 配置发送报文头 (通用于所有通道)
    TxHead.Identifier = motor_id;                // 报文 ID
    TxHead.IdType = FDCAN_STANDARD_ID;           // 标准 ID
    TxHead.TxFrameType = FDCAN_DATA_FRAME;       // 数据帧
    TxHead.DataLength = Get_FDCAN_DLC(len);      // 使用上面写的转换函数，搞定 DLC

    // 经典模式下的固定配置
    TxHead.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHead.BitRateSwitch = FDCAN_BRS_OFF;        // 关闭速率切换
    TxHead.FDFormat = FDCAN_CLASSIC_CAN;         // 经典 CAN 模式 (对应你的 CubeMX)
    TxHead.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHead.MessageMarker = 0;

    // 2. 根据 channel 选择句柄发送
    FDCAN_HandleTypeDef *hfdcan_ptr = NULL;

    if (can_channel == 1)      hfdcan_ptr = &hfdcan1;
    else if (can_channel == 2) hfdcan_ptr = &hfdcan2;
    else if (can_channel == 3) hfdcan_ptr = &hfdcan3;

    // 3. 执行发送
    if (hfdcan_ptr != NULL)
    {
        if (HAL_FDCAN_AddMessageToTxFifoQ(hfdcan_ptr, &TxHead, buf) != HAL_OK)
        {
            // 发送失败处理，可以在这里加断点或错误记数
        }
    }
}


/**
  * @brief  Converts a float to an unsigned int, given range and number of bits
  * @param
  * @retval
  */
uint16_t float_to_uint(float x, float x_min, float x_max, uint8_t bits)
{
    float span = x_max - x_min;
    float offset = x_min;

    return (uint16_t) ((x-offset)*((float)((1<<bits)-1))/span);
}


/**
  * @brief  converts unsigned int to float, given range and number of bits
  * @param
  * @retval
  */
float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
    float span = x_max - x_min;
    float offset = x_min;
    return ((float)x_int)*span/((float)((1<<bits)-1)) + offset;
}

