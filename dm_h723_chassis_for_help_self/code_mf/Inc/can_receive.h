//
// Created by 21481 on 2026/1/30.
//

#ifndef DM_H723_LIB_CAN_RECEIVE_H
#define DM_H723_LIB_CAN_RECEIVE_H

#include "dm_motor.h"

#define CAN_CHANNEL_1 1
#define CAN_CHANNEL_2 2
#define CAN_CHANNEL_3 3

void DM_motors_parse(struct dm_motor *motor_ptr, const uint8_t rx_DM_data[8]);



#endif //DM_H723_LIB_CAN_RECEIVE_H
