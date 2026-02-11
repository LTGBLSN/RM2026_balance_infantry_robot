//
// Created by 21481 on 2026/1/30.
//

#ifndef DM_H723_LIB_IMU_DATA_GET_H
#define DM_H723_LIB_IMU_DATA_GET_H


typedef struct
{
    //弧度制角速度
    float roll_radian_vel;
    float yaw_radian_vel;
    float pitch_radian_vel;

    //弧度制角度
    float roll_radian_angle;
    float yaw_radian_angle;
    float pitch_radian_angle;


    float roll_degree_angle;
    float yaw_degree_angle;
    float pitch_degree_angle;
} INS_t;
extern INS_t imu_data_from_board_BMI088_mahony;




#endif //DM_H723_LIB_IMU_DATA_GET_H
