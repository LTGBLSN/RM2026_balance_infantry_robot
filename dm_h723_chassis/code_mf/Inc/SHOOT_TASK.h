//
// Created by 21481 on 2026/1/31.
//

#ifndef DM_H723_LIB_SHOOT_TASK_H
#define DM_H723_LIB_SHOOT_TASK_H


#include "struct_typedef.h"


#define SHOOT_2006_ID3_SPEED_PID_KP        8.0f
#define SHOOT_2006_ID3_SPEED_PID_KI        0.9f
#define SHOOT_2006_ID3_SPEED_PID_KD        0.0f
#define SHOOT_2006_ID3_SPEED_PID_OUT_MAX   9000.0f
#define SHOOT_2006_ID3_SPEED_PID_KI_MAX    7000.0f


#define SHOOT_TURN_ON_SPEED  2500
#define SHOOT_TURN_OFF_SPEED (-5000)

#define SHOOT_SPEED_CHECK_TIME 100
#define SHOOT_STOP_CHECK_SPEED 100  //+-500,卡弹检测速度区间，区间内都视为卡弹
#define SHOOT_TURN_OFF_TIME 100


#define SHOOT_MAX_STOP_TIME 500 //ms

#define ERR_STOP 0
#define NORMAL_ON 1

#define NO_JUMP 0
#define YES_JUMP 1

void shoot_stop_check();
void shoot_pid_control();

void shoot_2006_id3_speed_pid_init(void);
int16_t shoot_2006_id3_speed_pid_loop(float shoot_2006_ID3_speed_set_loop);




#endif //DM_H723_LIB_SHOOT_TASK_H
