//
// Created by 21481 on 2026/1/30.
//



#include "cmsis_os.h"
#include "BMI088driver.h"
#include "MahonyAHRS.h"
#include "IMU_DATA_GET.h"
#include "DJI_motors.h"

INS_t imu_data_from_board_BMI088_mahony;//板子xt30为后，灯的位置为前，正面朝上，遵循右手坐标系

void IMU_DATA_GET()
{
    while (1)
    {
        BMI088_read(gyro, accel, &temp);
        MahonyAHRSupdate(INS_quat,gyro[0],gyro[1],gyro[2],accel[0],accel[1],accel[2],0.0f,0.0f,0.0f);
        get_angle(INS_quat,&INS_angle[0],&INS_angle[1],&INS_angle[2]);
        INS_degree[0] = INS_angle[0] * (180.0f / 3.14159265358979323846f);
        INS_degree[1] = INS_angle[1] * (180.0f / 3.14159265358979323846f);
        INS_degree[2] = INS_angle[2] * (180.0f / 3.14159265358979323846f);

        //弧度制角速度
        imu_data_from_board_BMI088_mahony.roll_radian_vel = -gyro[1];
        imu_data_from_board_BMI088_mahony.yaw_radian_vel = gyro[2];
        imu_data_from_board_BMI088_mahony.pitch_radian_vel = gyro[0];
        //弧度制角度
        imu_data_from_board_BMI088_mahony.roll_radian_angle = -INS_angle[1];
        imu_data_from_board_BMI088_mahony.yaw_radian_angle = INS_angle[0];
        imu_data_from_board_BMI088_mahony.pitch_radian_angle = INS_angle[2];
        //角度制角度
        imu_data_from_board_BMI088_mahony.roll_degree_angle = -INS_degree[1];
        imu_data_from_board_BMI088_mahony.yaw_degree_angle = INS_degree[0];
        imu_data_from_board_BMI088_mahony.pitch_degree_angle = INS_degree[2];





        osDelay(1);
    }
}

