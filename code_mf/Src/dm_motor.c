//
// Created by 21481 on 2025/12/15.
//
#include <string.h>
#include "dm_motor.h"
#include "can.h"

struct dm_motor DM8009P_01_RIGHT_FRONT ;
struct dm_motor DM8009P_02_RIGHT_FRONT ;
struct dm_motor DM8009P_03_RIGHT_FRONT ;
struct dm_motor DM8009P_04_RIGHT_FRONT ;

void dm_motor_init()
{
    memset(&DM8009P_01_RIGHT_FRONT, 0, sizeof(DM8009P_01_RIGHT_FRONT));//清零
    memset(&DM8009P_02_RIGHT_FRONT, 0, sizeof(DM8009P_02_RIGHT_FRONT));//清零
    memset(&DM8009P_03_RIGHT_FRONT, 0, sizeof(DM8009P_03_RIGHT_FRONT));//清零
    memset(&DM8009P_04_RIGHT_FRONT, 0, sizeof(DM8009P_04_RIGHT_FRONT));//清零

    //第一个电机
    DM8009P_01_RIGHT_FRONT.motor_type = DM8009P ;
    DM8009P_01_RIGHT_FRONT.can_id = 0x01 ;
    DM8009P_01_RIGHT_FRONT.can_master_id = 0x11 ;
    DM8009P_01_RIGHT_FRONT.can_channel = can1 ;
    //第一个电机
    DM8009P_02_RIGHT_FRONT.motor_type = DM8009P ;
    DM8009P_02_RIGHT_FRONT.can_id = 0x02 ;
    DM8009P_02_RIGHT_FRONT.can_master_id = 0x12 ;
    DM8009P_02_RIGHT_FRONT.can_channel = can1 ;
    //第一个电机
    DM8009P_03_RIGHT_FRONT.motor_type = DM8009P ;
    DM8009P_03_RIGHT_FRONT.can_id = 0x03 ;
    DM8009P_03_RIGHT_FRONT.can_master_id = 0x13 ;
    DM8009P_03_RIGHT_FRONT.can_channel = can1 ;
    //第一个电机
    DM8009P_04_RIGHT_FRONT.motor_type = DM8009P ;
    DM8009P_04_RIGHT_FRONT.can_id = 0x04 ;
    DM8009P_04_RIGHT_FRONT.can_master_id = 0x14 ;
    DM8009P_04_RIGHT_FRONT.can_channel = can1 ;



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
  * @param
  * @retval
  */
//p:期望位置 v:期望速度 kp: kd: t:前馈扭矩
void Dm_Can_Send(struct dm_motor dm_motor_para)
{
    float f_p = 0.0f ;
    float f_v = 0.0f ;
    float f_kp = 0.0f ;
    float f_kd = 0.0f ;
    float f_t = dm_motor_para.give_tor;

    uint16_t p, v, kp, kd, t;//最终发送，经过转换的
    uint8_t buf[8];

    /* 限制输入的参数在定义的范围内 */
    switch (dm_motor_para.motor_type) {

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
    DMCanTransmit(buf, sizeof(buf), dm_motor_para.can_channel, dm_motor_para.can_id);
}




//下半部分为复用函数

/* 把buf中的内容通过CAN接口发送出去 */
static void DMCanTransmit(uint8_t *buf, uint8_t len , uint8_t can_channel , uint8_t motor_id)
{
    if(can_channel == 0x01)
    {
        CAN_TxHeaderTypeDef can1_TxHead;             /**!< can通信发送协议头 */
        uint32_t can1_TxMailbox;

        if((buf != NULL) && (len != 0))
        {
            can1_TxHead.StdId    = motor_id;     /* 指定标准标识符，该值在0x00-0x7FF */
            can1_TxHead.IDE      = CAN_ID_STD;       /* 指定将要传输消息的标识符类型 */
            can1_TxHead.RTR      = CAN_RTR_DATA;     /* 指定消息传输帧类型 */
            can1_TxHead.DLC      = len;              /* 指定将要传输的帧长度 */

            if(HAL_CAN_AddTxMessage(&hcan1, &can1_TxHead, buf, (uint32_t *)&can1_TxMailbox) == HAL_OK )
            {
            }
        }
    }
    if(can_channel == 0x02)
    {
        CAN_TxHeaderTypeDef can2_TxHead;             /**!< can通信发送协议头 */
        uint32_t can2_TxMailbox;

        if((buf != NULL) && (len != 0))
        {
            can2_TxHead.StdId    = motor_id;     /* 指定标准标识符，该值在0x00-0x7FF */
            can2_TxHead.IDE      = CAN_ID_STD;       /* 指定将要传输消息的标识符类型 */
            can2_TxHead.RTR      = CAN_RTR_DATA;     /* 指定消息传输帧类型 */
            can2_TxHead.DLC      = len;              /* 指定将要传输的帧长度 */

            if(HAL_CAN_AddTxMessage(&hcan2, &can2_TxHead, buf, (uint32_t *)&can2_TxMailbox) == HAL_OK )
            {
            }
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

