//
// Created by 21481 on 2026/1/30.
//

#ifndef DM_H723_LIB_DJI_MOTORS_H
#define DM_H723_LIB_DJI_MOTORS_H

#include "main.h"



//rm motor data
typedef struct
{
    uint16_t ecd;
    int16_t speed_rpm;
    int16_t given_current;
    uint8_t temperate;
    int16_t last_ecd;
} motor_measure_t;
extern motor_measure_t motor_can1_data[7];
extern motor_measure_t motor_can2_data[7];


void FDCAN_DJI_motors(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4, uint16_t id, uint8_t can_channel);

#endif //DM_H723_LIB_DJI_MOTORS_H
