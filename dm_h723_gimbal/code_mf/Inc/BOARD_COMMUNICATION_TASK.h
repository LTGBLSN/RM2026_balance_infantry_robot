//
// Created by 21481 on 2026/3/2.
//

#ifndef DM_H723_LIB_BOARD_COMMUNICATION_TASK_H
#define DM_H723_LIB_BOARD_COMMUNICATION_TASK_H

#include "main.h"

uint8_t state_convert(int16_t data_01 ,int16_t data_02 , int16_t data_03 ,int16_t data_04 );

void gimbal_2_chassis_A(int16_t yaw_6020,int16_t rc_01 ,int16_t rc_02 ,uint8_t state_01 ,uint8_t state_02, uint16_t id, uint8_t can_channel);

void gimbal_rc_2_chassis(int16_t data , int16_t rc_s , int16_t rc_ch_A , int16_t rc_ch_B , uint16_t id, uint8_t can_channel);

void gimbal_rc_2_chassis_uint(uint16_t data , int16_t rc_s , uint16_t rc_ch_A , uint16_t rc_ch_B , uint16_t id, uint8_t can_channel);

#endif //DM_H723_LIB_BOARD_COMMUNICATION_TASK_H
