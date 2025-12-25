/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       can_receive.c/h
  * @brief      there is CAN interrupt function  to receive motor data,
  *             and CAN send function to send motor current to control motor.
  *             这里是CAN中断接收函数，接收电机数据,CAN发送函数发送电机电流控制电机.
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */

#include <string.h>
#include "CAN_receive.h"
#include "main.h"
#include "dm_motor.h"
#include "math.h"
#include "DM_IMU.h"


extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
//motor data read
#define get_motor_measure(ptr, data)                                    \
    {                                                                   \
        (ptr)->last_ecd = (ptr)->ecd;                                   \
        (ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]);            \
        (ptr)->speed_rpm = (uint16_t)((data)[2] << 8 | (data)[3]);      \
        (ptr)->given_current = (uint16_t)((data)[4] << 8 | (data)[5]);  \
        (ptr)->temperate = (data)[6];                                   \
    }
/*
motor data,  0:chassis motor1 3508;1:chassis motor3 3508;2:chassis motor3 3508;3:chassis motor4 3508;
4:yaw gimbal_vx motor 6020;5:pitch gimbal_vx motor 6020;6:trigger motor 2006;
电机数据, 0:底盘电机1 3508电机,  1:底盘电机2 3508电机,2:底盘电机3 3508电机,3:底盘电机4 3508电机;
4:yaw云台电机 6020电机; 5:pitch云台电机 6020电机; 6:拨弹电机 2006电机*/
motor_measure_t motor_can1_data[7];
motor_measure_t motor_can2_data[7];

static CAN_TxHeaderTypeDef  shoot_tx_message;
static uint8_t              shoot_can_send_data[8];
static CAN_TxHeaderTypeDef  chassis_tx_message;
static uint8_t              chassis_can_send_data[8];
static CAN_TxHeaderTypeDef  yaw_tx_message;
static uint8_t              yaw_can_send_data[8];

/**
  * @brief          hal CAN fifo call back, receive motor data
  * @param[in]      hcan, the point to CAN handle
  * @retval         none
  */
/**
  * @brief          hal库CAN回调函数,接收电机数据
  * @param[in]      hcan:CAN句柄指针
  * @retval         none
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if(hcan == &hcan1)
    {
        CAN_RxHeaderTypeDef can1_RxHead; /**!< can通信协议头 */
        uint8_t can1_data[8];
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &can1_RxHead, can1_data);

        switch (can1_RxHead.StdId)
        {
            case CAN_3508_M1_ID:
            case CAN_3508_M2_ID:
            case CAN_3508_M3_ID:
            case CAN_3508_M4_ID:
            case CAN_YAW_MOTOR_ID:
            case CAN_PIT_MOTOR_ID:
            case CAN_TRIGGER_MOTOR_ID:
            {
                static uint8_t i = 0;
                //get motor id
                i = can1_RxHead.StdId - CAN_3508_M1_ID;
                get_motor_measure(&motor_can1_data[i], can1_data);
                break;
            }

            case (0x11):
            {
                int p_int;
                int v_int;
                int t_int;

                DM8009P_01_LEFT_FRONT.state = (can1_data[0])>>4;
                p_int = (can1_data[1] << 8) | can1_data[2];
                v_int = (can1_data[3] << 4) | (can1_data[4] >> 4);
                t_int = ((can1_data[4] & 0xF) << 8) | can1_data[5];
                DM8009P_01_LEFT_FRONT.return_angle = uint_to_float(p_int, DM8009P_P_MIN, DM8009P_P_MAX, 16);
                DM8009P_01_LEFT_FRONT.return_speed_rpm = uint_to_float(v_int, DM8009P_V_MIN, DM8009P_V_MAX, 12);
                DM8009P_01_LEFT_FRONT.return_tor = uint_to_float(t_int, DM8009P_T_MIN, DM8009P_T_MAX, 12);
                DM8009P_01_LEFT_FRONT.Tmos = (float )(can1_data[6]);
                DM8009P_01_LEFT_FRONT.Tcoil = (float )(can1_data[7]);
                break;
            }
            case (0x12):
            {
                int p_int;
                int v_int;
                int t_int;

                DM8009P_02_RIGHT_FRONT.state = (can1_data[0])>>4;
                p_int = (can1_data[1] << 8) | can1_data[2];
                v_int = (can1_data[3] << 4) | (can1_data[4] >> 4);
                t_int = ((can1_data[4] & 0xF) << 8) | can1_data[5];
                DM8009P_02_RIGHT_FRONT.return_angle = uint_to_float(p_int, DM8009P_P_MIN, DM8009P_P_MAX, 16);
                DM8009P_02_RIGHT_FRONT.return_speed_rpm = uint_to_float(v_int, DM8009P_V_MIN, DM8009P_V_MAX, 12);
                DM8009P_02_RIGHT_FRONT.return_tor = uint_to_float(t_int, DM8009P_T_MIN, DM8009P_T_MAX, 12);
                DM8009P_02_RIGHT_FRONT.Tmos = (float )(can1_data[6]);
                DM8009P_02_RIGHT_FRONT.Tcoil = (float )(can1_data[7]);
                break;
            }
            case (0x13):
            {
                int p_int;
                int v_int;
                int t_int;

                DM8009P_03_RIGHT_BEHIND.state = (can1_data[0])>>4;
                p_int = (can1_data[1] << 8) | can1_data[2];
                v_int = (can1_data[3] << 4) | (can1_data[4] >> 4);
                t_int = ((can1_data[4] & 0xF) << 8) | can1_data[5];
                DM8009P_03_RIGHT_BEHIND.return_angle = uint_to_float(p_int, DM8009P_P_MIN, DM8009P_P_MAX, 16);
                DM8009P_03_RIGHT_BEHIND.return_speed_rpm = uint_to_float(v_int, DM8009P_V_MIN, DM8009P_V_MAX, 12);
                DM8009P_03_RIGHT_BEHIND.return_tor = uint_to_float(t_int, DM8009P_T_MIN, DM8009P_T_MAX, 12);
                DM8009P_03_RIGHT_BEHIND.Tmos = (float )(can1_data[6]);
                DM8009P_03_RIGHT_BEHIND.Tcoil = (float )(can1_data[7]);
                break;
            }
            case (0x14):
            {
                int p_int;
                int v_int;
                int t_int;

                DM8009P_04_LEFT_BEHIND.state = (can1_data[0])>>4;
                p_int = (can1_data[1] << 8) | can1_data[2];
                v_int = (can1_data[3] << 4) | (can1_data[4] >> 4);
                t_int = ((can1_data[4] & 0xF) << 8) | can1_data[5];
                DM8009P_04_LEFT_BEHIND.return_angle = uint_to_float(p_int, DM8009P_P_MIN, DM8009P_P_MAX, 16);
                DM8009P_04_LEFT_BEHIND.return_speed_rpm = uint_to_float(v_int, DM8009P_V_MIN, DM8009P_V_MAX, 12);
                DM8009P_04_LEFT_BEHIND.return_tor = uint_to_float(t_int, DM8009P_T_MIN, DM8009P_T_MAX, 12);
                DM8009P_04_LEFT_BEHIND.Tmos = (float )(can1_data[6]);
                DM8009P_04_LEFT_BEHIND.Tcoil = (float )(can1_data[7]);
                break;
            }
//            case 0x301:
//            {
//                DM8009P_01_LEFT_FRONT.last_angle = DM8009P_01_LEFT_FRONT.return_angle ;
//
//                uint16_t position = ((uint16_t)can1_data[0] << 8) | can1_data[1] ;
//                DM8009P_01_LEFT_FRONT.return_angle = (float )(((float)position / DM8009P_1_4_FIRMWARE_MAX_RETURN_ANGLE) * M_PI) ;
//
//                int16_t raw_speed = (int16_t)(((int16_t)can1_data[2] << 8) | can1_data[3]);
//                DM8009P_01_LEFT_FRONT.return_speed_rpm = (float )raw_speed / 100.0f;
//
//                DM8009P_01_LEFT_FRONT.return_tor = (int16_t)(((int16_t)can1_data[4] << 8) | can1_data[5]);
//
//                DM8009P_01_LEFT_FRONT.Tcoil = can1_data[6] ;
//                DM8009P_01_LEFT_FRONT.state = can1_data[7] ;
//            }


//MIT模式返回帧
//            case (0x11):
//            {
//                int p_int;
//                int v_int;
//                int t_int;
//
//                DM8009P_01_RIGHT_FRONT.state = (can1_data[0])>>4;
//                p_int = (can1_data[1] << 8) | can1_data[2];
//                v_int = (can1_data[3] << 4) | (can1_data[4] >> 4);
//                t_int = ((can1_data[4] & 0xF) << 8) | can1_data[5];
//                DM8009P_01_RIGHT_FRONT.return_angle = uint_to_float(p_int, DM8009P_P_MIN, DM8009P_P_MAX, 16);
//                DM8009P_01_RIGHT_FRONT.return_speed = uint_to_float(v_int, DM8009P_V_MIN, DM8009P_V_MAX, 12);
//                DM8009P_01_RIGHT_FRONT.return_tor = uint_to_float(t_int, DM8009P_T_MIN, DM8009P_T_MAX, 12);
//                DM8009P_01_RIGHT_FRONT.Tmos = (float )(can1_data[6]);
//                DM8009P_01_RIGHT_FRONT.Tcoil = (float )(can1_data[7]);
//                break;
//            }

            default:
            {
                break;
            }
        }


    } else if(hcan == &hcan2)
    {
        CAN_RxHeaderTypeDef can2_rx_header;
        uint8_t can2_rx_data[8];

        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &can2_rx_header, can2_rx_data);

        switch (can2_rx_header.StdId)
        {
            case CAN_3508_M1_ID:
            case CAN_3508_M2_ID:
            case CAN_3508_M3_ID:
            case CAN_3508_M4_ID:
            case CAN_YAW_MOTOR_ID:
            case CAN_PIT_MOTOR_ID:
            case CAN_TRIGGER_MOTOR_ID:
            {
                static uint8_t i = 0;
                //get motor id
                i = can2_rx_header.StdId - CAN_3508_M1_ID;
                get_motor_measure(&motor_can2_data[i], can2_rx_data); // 注意这里使用了 motor_can2_data 数组
                break;
            }
            case CAN_DM_IMU:
            {
                IMU_UpdateData(can2_rx_data);

                break;
            }

            default:
            {
                break;
            }
        }
    }
}





// 实现邮箱0的回调
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
    if(hcan == &hcan1)
    {

    }

}

// 让另外两个邮箱的完成回调也指向同一个函数
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
    HAL_CAN_TxMailbox0CompleteCallback(hcan);
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
    HAL_CAN_TxMailbox0CompleteCallback(hcan);
}

//void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
//{
//    if(hcan == &hcan1)
//    {
//        CAN1_cmd_chassis_shoot(400,400,0,0);
//    }
//}

//void HAL_CAN_TxMailboxCompleteCallback(CAN_HandleTypeDef *hcan) {
//    // 发送完成，可以发送下一条消息
//    static uint8_t msgIndex = 0;
//    if (msgIndex < 4) {
//        Dm_Can_Send(nextMessage[msgIndex++]);
//    }
//}


HAL_StatusTypeDef CAN1_cmd_chassis_shoot(int16_t right_motor, int16_t left_motor, int16_t shoot_motor, int16_t motor4)
{
    uint32_t send_mail_box;
    chassis_tx_message.StdId = CAN_CHASSIS_ALL_ID;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;

    chassis_can_send_data[0] = right_motor >> 8;
    chassis_can_send_data[1] = right_motor;
    chassis_can_send_data[2] = left_motor >> 8;
    chassis_can_send_data[3] = left_motor;
    chassis_can_send_data[4] = shoot_motor >> 8;
    chassis_can_send_data[5] = shoot_motor;
    chassis_can_send_data[6] = motor4 >> 8;
    chassis_can_send_data[7] = motor4;

    // 返回 HAL_CAN_AddTxMessage 的执行状态
    return HAL_CAN_AddTxMessage(&hcan1, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}

//yaw电机电流发送函数
void CAN1_cmd_yaw(int16_t yaw, int16_t motor2, int16_t motor3, int16_t motor4)
{
    uint32_t send_mail_box;
    yaw_tx_message.StdId = CAN_GIMBAL_ALL_ID;
    yaw_tx_message.IDE = CAN_ID_STD;
    yaw_tx_message.RTR = CAN_RTR_DATA;
    yaw_tx_message.DLC = 0x08;
    yaw_can_send_data[0] = yaw >> 8;
    yaw_can_send_data[1] = yaw;
    yaw_can_send_data[2] = motor2 >> 8;
    yaw_can_send_data[3] = motor2;
    yaw_can_send_data[4] = motor3 >> 8;
    yaw_can_send_data[5] = motor3;
    yaw_can_send_data[6] = motor4 >> 8;
    yaw_can_send_data[7] = motor4;

    HAL_CAN_AddTxMessage(&hcan1, &yaw_tx_message, yaw_can_send_data, &send_mail_box);
}




