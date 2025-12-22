//
// Created by 21481 on 2025/12/15.
//

#ifndef BUBING_RM2025_DM_MOTOR_H
#define BUBING_RM2025_DM_MOTOR_H

#include "struct_typedef.h"
#include "main.h"


#define LIMIT_MIN_MAX(x,min,max) (x) = (((x)<=(min))?(min):(((x)>=(max))?(max):(x)))




#define DM8009P 0x00
#define DM4310 0x01
#define DM3507 0x02

#define DM8009P_MIT_FIRMWARE 0x00
#define DM8009P_1_4_FIRMWARE 0x01

#define can1 0x01
#define can2 0x02

#define CMD_MOTOR_MODE      0x01
#define CMD_RESET_MODE      0x02
#define CMD_ZERO_POSITION   0x03

#define DM8009P_1_4_FIRMWARE_MAX_RETURN_ANGLE 8191.0f


#define DM8009P_P_MIN -12.5f    // Radians
#define DM8009P_P_MAX 12.5f
#define DM8009P_V_MIN -45.0f    // Rad/s
#define DM8009P_V_MAX 45.0f
#define DM8009P_KP_MIN 0.0f     // N-m/rad
#define DM8009P_KP_MAX 500.0f
#define DM8009P_KD_MIN 0.0f     // N-m/rad/s
#define DM8009P_KD_MAX 5.0f
#define DM8009P_T_MIN -54.0f
#define DM8009P_T_MAX 54.0f


#define DM4310_P_MIN -12.5f    // Radians
#define DM4310_P_MAX 12.5f
#define DM4310_V_MIN -30.0f    // Rad/s
#define DM4310_V_MAX 30.0f
#define DM4310_KP_MIN 0.0f     // N-m/rad
#define DM4310_KP_MAX 500.0f
#define DM4310_KD_MIN 0.0f     // N-m/rad/s
#define DM4310_KD_MAX 5.0f
#define DM4310_T_MIN -10.0f
#define DM4310_T_MAX 10.0f

#define DM3507_P_MIN -12.5f    // Radians
#define DM3507_P_MAX 12.5f
#define DM3507_V_MIN -30.0f    // Rad/s
#define DM3507_V_MAX 30.0f
#define DM3507_KP_MIN 0.0f     // N-m/rad
#define DM3507_KP_MAX 500.0f
#define DM3507_KD_MIN 0.0f     // N-m/rad/s
#define DM3507_KD_MAX 5.0f
#define DM3507_T_MIN -10.0f
#define DM3507_T_MAX 10.0f

extern struct dm_motor DM8009P_01_LEFT_FRONT ;
extern struct dm_motor DM8009P_02_RIGHT_FRONT ;
extern struct dm_motor DM8009P_03_RIGHT_BEHIND ;
extern struct dm_motor DM8009P_04_LEFT_BEHIND ;


struct dm_motor {
    //电机参数
    uint8_t motor_type;
    uint8_t motor_firmware;
    uint8_t can_id;
    uint8_t can_master_id;
    uint8_t can_channel;

    //返回帧
    uint8_t state;
    float last_angle;
    float return_angle;
    float return_speed_rpm;
    float return_tor;
    float Tmos;
    float Tcoil;//线圈温度

    //数据处理相关
    int32_t laps ;//编码器计圈

    //控制相关
    float give_angle;
    float give_speed;
    float give_tor;
};

void dm_motor_init();
void dm_motor_mode_set(uint8_t cmd , struct dm_motor dmMotor_cmd);
HAL_StatusTypeDef Dm_Can_Send(struct dm_motor dm_motor_para);

HAL_StatusTypeDef DMCanTransmit(uint8_t *buf, uint8_t len, uint8_t can_channel, uint8_t motor_id);

uint16_t float_to_uint(float x, float x_min, float x_max, uint8_t bits);
float uint_to_float(int x_int, float x_min, float x_max, int bits);

HAL_StatusTypeDef dm_1_4_can1_cmd(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);

#endif //BUBING_RM2025_DM_MOTOR_H
