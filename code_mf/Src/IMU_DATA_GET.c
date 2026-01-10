//
// Created by 21481 on 2025/7/20.
//
#include "main.h"
#include "cmsis_os.h"
#include "BMI088driver.h"
#include "IMU_DATA_GET.h"
#include "MahonyAHRS.h"
#include "math.h"
#include "DM_IMU.h"

void IMU_DATA_GET()
{
    while (1)
    {
        roll_speed_from_bmi088 = imu.gyro[0];
        pitch_speed_from_bmi088 = imu.gyro[1];
        yaw_speed_from_bmi088 = imu.gyro[2];

        yaw_angle_from_bmi088 = imu.yaw ;
        pitch_angle_from_bmi088 = imu.pitch ;
        roll_angle_from_bmi088 = imu.roll ;





        osDelay(1);
    }
}

