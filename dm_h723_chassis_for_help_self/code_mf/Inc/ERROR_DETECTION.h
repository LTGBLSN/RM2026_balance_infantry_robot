//
// Created by 21481 on 2026/2/12.
//

#ifndef DM_H723_LIB_ERROR_DETECTION_H
#define DM_H723_LIB_ERROR_DETECTION_H


#define GM6020_TEMP_MAX 60
#define GM6020_SAFE 1
#define GM6020_DIE 0

#define DM_MOTOR_CHECK_TIME 500
#define DM_MOTOR_SAFE 1
#define DM_MOTOR_DIE 0

void rc_connection_status();
void yaw_6020_status();
void DM_motor_status();


#endif //DM_H723_LIB_ERROR_DETECTION_H
