//
// Created by 21481 on 2025/12/15.
//
#include <string.h>
#include "dm_motor.h"
#include "can.h"

struct dm_motor DM8009P_01_LEFT_FRONT ;
struct dm_motor DM8009P_02_RIGHT_FRONT ;
struct dm_motor DM8009P_03_RIGHT_BEHIND ;
struct dm_motor DM8009P_04_LEFT_BEHIND ;

void dm_motor_init()
{
    memset(&DM8009P_01_LEFT_FRONT, 0, sizeof(DM8009P_01_LEFT_FRONT));//清零
    memset(&DM8009P_02_RIGHT_FRONT, 0, sizeof(DM8009P_02_RIGHT_FRONT));//清零
    memset(&DM8009P_03_RIGHT_BEHIND, 0, sizeof(DM8009P_03_RIGHT_BEHIND));//清零
    memset(&DM8009P_04_LEFT_BEHIND, 0, sizeof(DM8009P_04_LEFT_BEHIND));//清零

    //第一个电机
    DM8009P_01_LEFT_FRONT.motor_type = DM8009P ;
    DM8009P_01_LEFT_FRONT.motor_firmware = DM8009P_MIT_FIRMWARE ;
    DM8009P_01_LEFT_FRONT.can_id = 0x01 ;
    DM8009P_01_LEFT_FRONT.can_master_id = 0x11 ;
    DM8009P_01_LEFT_FRONT.can_channel = can1 ;
    //第一个电机
    DM8009P_02_RIGHT_FRONT.motor_type = DM8009P ;
    DM8009P_02_RIGHT_FRONT.motor_firmware = DM8009P_MIT_FIRMWARE ;
    DM8009P_02_RIGHT_FRONT.can_id = 0x02 ;
    DM8009P_02_RIGHT_FRONT.can_master_id = 0x12 ;
    DM8009P_02_RIGHT_FRONT.can_channel = can1 ;
    //第一个电机
    DM8009P_03_RIGHT_BEHIND.motor_type = DM8009P ;
    DM8009P_03_RIGHT_BEHIND.motor_firmware = DM8009P_MIT_FIRMWARE ;
    DM8009P_03_RIGHT_BEHIND.can_id = 0x03 ;
    DM8009P_03_RIGHT_BEHIND.can_master_id = 0x13 ;
    DM8009P_03_RIGHT_BEHIND.can_channel = can1 ;
    //第一个电机
    DM8009P_04_LEFT_BEHIND.motor_type = DM8009P ;
    DM8009P_04_LEFT_BEHIND.motor_firmware = DM8009P_MIT_FIRMWARE ;
    DM8009P_04_LEFT_BEHIND.can_id = 0x04 ;
    DM8009P_04_LEFT_BEHIND.can_master_id = 0x14 ;
    DM8009P_04_LEFT_BEHIND.can_channel = can1 ;



}

// 电机模式切换
void dm_motor_mode_set(uint8_t cmd , struct dm_motor dmMotor_cmd)
{
    uint8_t buf[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00};
    switch(cmd)
    {
        case CMD_MOTOR_MODE://
            buf[7] = 0xFC;
            break;

        case CMD_RESET_MODE://
            buf[7] = 0xFD;
            break;

        case CMD_ZERO_POSITION://设置零点
            buf[7] = 0xFE;
            break;

        default:
            return; /* 直接退出函数 */
    }
    DMCanTransmit(buf, sizeof(buf), dmMotor_cmd.can_channel, dmMotor_cmd.can_id);
}


/**
  * @brief  Can总线发送控制参数
  * @param  dm_motor_para 电机参数结构体（包含位置、速度、KP、KD、扭矩等）
  * @retval HAL_StatusTypeDef 发送状态（HAL_OK=成功，否则失败）
  */
HAL_StatusTypeDef Dm_Can_Send(struct dm_motor dm_motor_para)
{
    float f_p = 0.0f;
    float f_v = 0.0f;
    float f_kp = 0.0f;
    float f_kd = 0.0f;
    float f_t = dm_motor_para.give_tor;

    uint16_t p = 0, v = 0, kp = 0, kd = 0, t = 0; // 最终发送，经过转换的
    uint8_t buf[8] = {0};

    /* 限制输入的参数在定义的范围内 */
    switch (dm_motor_para.motor_type) {
        case DM8009P: {
            LIMIT_MIN_MAX(f_p, DM8009P_P_MIN, DM8009P_P_MAX);
            LIMIT_MIN_MAX(f_v, DM8009P_V_MIN, DM8009P_V_MAX);
            LIMIT_MIN_MAX(f_kp, DM8009P_KP_MIN, DM8009P_KP_MAX);
            LIMIT_MIN_MAX(f_kd, DM8009P_KD_MIN, DM8009P_KD_MAX);
            LIMIT_MIN_MAX(f_t, DM8009P_T_MIN, DM8009P_T_MAX);

            p = float_to_uint(f_p, DM8009P_P_MIN, DM8009P_P_MAX, 16);
            v = float_to_uint(f_v, DM8009P_V_MIN, DM8009P_V_MAX, 12);
            kp = float_to_uint(f_kp, DM8009P_KP_MIN, DM8009P_KP_MAX, 12);
            kd = float_to_uint(f_kd, DM8009P_KD_MIN, DM8009P_KD_MAX, 12);
            t = float_to_uint(f_t, DM8009P_T_MIN, DM8009P_T_MAX, 12);
            break;
        }
        case DM4310: {
            LIMIT_MIN_MAX(f_p, DM4310_P_MIN, DM4310_P_MAX);
            LIMIT_MIN_MAX(f_v, DM4310_V_MIN, DM4310_V_MAX);
            LIMIT_MIN_MAX(f_kp, DM4310_KP_MIN, DM4310_KP_MAX);
            LIMIT_MIN_MAX(f_kd, DM4310_KD_MIN, DM4310_KD_MAX);
            LIMIT_MIN_MAX(f_t, DM4310_T_MIN, DM4310_T_MAX);

            p = float_to_uint(f_p, DM4310_P_MIN, DM4310_P_MAX, 16);
            v = float_to_uint(f_v, DM4310_V_MIN, DM4310_V_MAX, 12);
            kp = float_to_uint(f_kp, DM4310_KP_MIN, DM4310_KP_MAX, 12);
            kd = float_to_uint(f_kd, DM4310_KD_MIN, DM4310_KD_MAX, 12);
            t = float_to_uint(f_t, DM4310_T_MIN, DM4310_T_MAX, 12);
            break;
        }
        case DM3507: {
            LIMIT_MIN_MAX(f_p, DM3507_P_MIN, DM3507_P_MAX);
            LIMIT_MIN_MAX(f_v, DM3507_V_MIN, DM3507_V_MAX);
            LIMIT_MIN_MAX(f_kp, DM3507_KP_MIN, DM3507_KP_MAX);
            LIMIT_MIN_MAX(f_kd, DM3507_KD_MIN, DM3507_KD_MAX);
            LIMIT_MIN_MAX(f_t, DM3507_T_MIN, DM3507_T_MAX);

            p = float_to_uint(f_p, DM3507_P_MIN, DM3507_P_MAX, 16);
            v = float_to_uint(f_v, DM3507_V_MIN, DM3507_V_MAX, 12);
            kp = float_to_uint(f_kp, DM3507_KP_MIN, DM3507_KP_MAX, 12);
            kd = float_to_uint(f_kd, DM3507_KD_MIN, DM3507_KD_MAX, 12);
            t = float_to_uint(f_t, DM3507_T_MIN, DM3507_T_MAX, 12);
            break;
        }
        default: {

        }
    }

    /* 根据传输协议，把数据转换为CAN命令数据字段 */
    buf[0] = p >> 8;
    buf[1] = p & 0xFF;
    buf[2] = v >> 4;
    buf[3] = ((v & 0xF) << 4) | (kp >> 8);
    buf[4] = kp & 0xFF;
    buf[5] = kd >> 4;
    buf[6] = ((kd & 0xF) << 4) | (t >> 8);
    buf[7] = t & 0xFF;

    /* 通过CAN接口发送数据，并返回发送状态 */
    return DMCanTransmit(buf, sizeof(buf), dm_motor_para.can_channel, dm_motor_para.can_id);
}




//下半部分为复用函数

/**
 * @brief 通过指定CAN通道发送数据
 * @param buf 要发送的数据缓冲区
 * @param len 数据长度（1-8字节）
 * @param can_channel CAN通道（0x01=CAN1, 0x02=CAN2）
 * @param motor_id 目标电机ID（标准ID，0x000-0x7FF）
 * @return HAL_StatusTypeDef 发送状态（HAL_OK=成功，否则失败）
 */
HAL_StatusTypeDef DMCanTransmit(uint8_t *buf, uint8_t len, uint8_t can_channel, uint8_t motor_id)
{
    if ((buf == NULL) || (len == 0) || (len > 8))  // CAN 2.0B 最多8字节
    {
        return HAL_ERROR;  // 参数错误
    }

    CAN_HandleTypeDef *hcan;
    CAN_TxHeaderTypeDef txHeader;
    uint32_t txMailbox;

    // 根据通道选择CAN句柄
    if (can_channel == 0x01)
    {
        hcan = &hcan1;
    }
    else if (can_channel == 0x02)
    {
        hcan = &hcan2;
    }
    else
    {
        return HAL_ERROR;  // 无效通道
    }

    // 设置发送头信息
    txHeader.StdId = motor_id;
    txHeader.IDE = CAN_ID_STD;
    txHeader.RTR = CAN_RTR_DATA;
    txHeader.DLC = len;

    // 发送消息并返回状态
    return HAL_CAN_AddTxMessage(hcan, &txHeader, buf, &txMailbox);
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

//一拖四固件
HAL_StatusTypeDef dm_1_4_can1_cmd(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    CAN_TxHeaderTypeDef  dm_tx_message;
    uint8_t              dm_can_send_data[8];
    uint32_t send_mail_box;
    dm_tx_message.StdId = 0x3FE;
    dm_tx_message.IDE = CAN_ID_STD;
    dm_tx_message.RTR = CAN_RTR_DATA;
    dm_tx_message.DLC = 0x08;

    dm_can_send_data[0] = motor1 >> 8;
    dm_can_send_data[1] = motor1;
    dm_can_send_data[2] = motor2 >> 8;
    dm_can_send_data[3] = motor2;
    dm_can_send_data[4] = motor3 >> 8;
    dm_can_send_data[5] = motor3;
    dm_can_send_data[6] = motor4 >> 8;
    dm_can_send_data[7] = motor4;

    // 返回 HAL_CAN_AddTxMessage 的执行状态
    return HAL_CAN_AddTxMessage(&hcan1, &dm_tx_message, dm_can_send_data, &send_mail_box);
}


