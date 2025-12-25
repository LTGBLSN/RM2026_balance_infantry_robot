//
// Created by 21481 on 2025/12/15.
//

#ifndef BUBING_RM2025_CHASSIS_TASK_H
#define BUBING_RM2025_CHASSIS_TASK_H


#define CHASSIS_GYRO_ANGLE_PID_KP 0.3f
#define CHASSIS_GYRO_ANGLE_PID_KI 0.0f
#define CHASSIS_GYRO_ANGLE_PID_KD 0.0f
#define CHASSIS_GYRO_ANGLE_PID_OUT_MAX 2.0f
#define CHASSIS_GYRO_ANGLE_PID_KI_MAX 0.0f

#define CHASSIS_GYRO_SPEED_PID_KP 5000.0f
#define CHASSIS_GYRO_SPEED_PID_KI 0.00f
#define CHASSIS_GYRO_SPEED_PID_KD 0.00f
#define CHASSIS_GYRO_SPEED_PID_OUT_MAX 16384.0f
#define CHASSIS_GYRO_SPEED_PID_KI_MAX 0.0f


#define CHASSIS_DM8009_ANGLE_PID_OUT_MAX 3.0f
#define CHASSIS_DM8009_ANGLE_PID_KI_MAX 0.0f

#define CHASSIS_DM8009_SPEED_PID_OUT_MAX   54.0f
#define CHASSIS_DM8009_SPEED_PID_KI_MAX   20.0f


#define CHASSIS_DM8009_01_SPEED_PID_KP   5.0f
#define CHASSIS_DM8009_01_SPEED_PID_KI   0.03f
#define CHASSIS_DM8009_01_SPEED_PID_KD   0.0f
#define CHASSIS_DM8009_01_ANGLE_PID_KP   30.0f
#define CHASSIS_DM8009_01_ANGLE_PID_KI   0.0f
#define CHASSIS_DM8009_01_ANGLE_PID_KD   0.0f


#define CHASSIS_DM8009_02_SPEED_PID_KP   5.0f
#define CHASSIS_DM8009_02_SPEED_PID_KI   0.03f
#define CHASSIS_DM8009_02_SPEED_PID_KD   0.0f
#define CHASSIS_DM8009_02_ANGLE_PID_KP   30.0f
#define CHASSIS_DM8009_02_ANGLE_PID_KI   0.0f
#define CHASSIS_DM8009_02_ANGLE_PID_KD   0.0f

#define CHASSIS_DM8009_03_SPEED_PID_KP   4.0f
#define CHASSIS_DM8009_03_SPEED_PID_KI   0.03f
#define CHASSIS_DM8009_03_SPEED_PID_KD   0.0f
#define CHASSIS_DM8009_03_ANGLE_PID_KP   30.0f
#define CHASSIS_DM8009_03_ANGLE_PID_KI   0.0f
#define CHASSIS_DM8009_03_ANGLE_PID_KD   0.0f


#define CHASSIS_DM8009_04_SPEED_PID_KP   4.0f
#define CHASSIS_DM8009_04_SPEED_PID_KI   0.03f
#define CHASSIS_DM8009_04_SPEED_PID_KD   0.0f
#define CHASSIS_DM8009_04_ANGLE_PID_KP   30.0f
#define CHASSIS_DM8009_04_ANGLE_PID_KI   0.0f
#define CHASSIS_DM8009_04_ANGLE_PID_KD   0.0f


void chassis_stand_loop();
void chassis_DM_motor_pid_loop();




void chassis_gyro_pitch_angle_pid_init(void);
float chassis_gyro_pitch_angle_pid_loop(float chassis_gyro_angle_set_loop);
void chassis_gyro_pitch_speed_pid_init(void);
float chassis_gyro_pitch_speed_pid_loop(float chassis_gyro_speed_set_loop);





void chassis_DM8009_01_speed_pid_init(void);
float chassis_DM8009_01_speed_pid_loop(float chassis_DM8009_01_speed_set_loop);
void chassis_DM8009_01_angle_pid_init(void);
float chassis_DM8009_01_angle_pid_loop(float chassis_DM8009_01_angle_set_loop);

void chassis_DM8009_02_speed_pid_init(void);
float chassis_DM8009_02_speed_pid_loop(float chassis_DM8009_02_speed_set_loop);
void chassis_DM8009_02_angle_pid_init(void);
float chassis_DM8009_02_angle_pid_loop(float chassis_DM8009_02_angle_set_loop);

void chassis_DM8009_03_speed_pid_init(void);
float chassis_DM8009_03_speed_pid_loop(float chassis_DM8009_03_speed_set_loop);
void chassis_DM8009_03_angle_pid_init(void);
float chassis_DM8009_03_angle_pid_loop(float chassis_DM8009_03_angle_set_loop);

void chassis_DM8009_04_speed_pid_init(void);
float chassis_DM8009_04_speed_pid_loop(float chassis_DM8009_04_speed_set_loop);
void chassis_DM8009_04_angle_pid_init(void);
float chassis_DM8009_04_angle_pid_loop(float chassis_DM8009_04_angle_set_loop);

#endif //BUBING_RM2025_CHASSIS_TASK_H
