//
// Created by 21481 on 2026/1/27.
//

#include "struct_typedef.h"

#ifndef BUBING_RM2025_SHOOT_CONTROL_H
#define BUBING_RM2025_SHOOT_CONTROL_H

#endif //BUBING_RM2025_SHOOT_CONTROL_H




#define SHOOT_2006_ID3_SPEED_PID_KP        10.0f
#define SHOOT_2006_ID3_SPEED_PID_KI        0.5f
#define SHOOT_2006_ID3_SPEED_PID_KD        2.0f
#define SHOOT_2006_ID3_SPEED_PID_OUT_MAX   4000.0f
#define SHOOT_2006_ID3_SPEED_PID_KI_MAX    2000.0f


#define SHOOT_TURN_ON_SPEED  3500
#define SHOOT_TURN_OFF_SPEED (-5000)

#define SHOOT_SPEED_CHECK_TIME 100
#define SHOOT_STOP_CHECK_SPEED 100  //+-500,卡弹检测速度区间，区间内都视为卡弹
#define SHOOT_TURN_OFF_TIME 100


#define SHOOT_MAX_STOP_TIME 500 //ms

void shoot_stop_check();



void shoot_2006_id3_speed_pid_init(void);
int16_t shoot_2006_id3_speed_pid_loop(float shoot_2006_ID3_speed_set_loop);



