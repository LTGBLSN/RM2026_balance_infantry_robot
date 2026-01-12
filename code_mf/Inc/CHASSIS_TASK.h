//
// Created by 21481 on 2025/12/15.
//

#ifndef BUBING_RM2025_CHASSIS_TASK_H
#define BUBING_RM2025_CHASSIS_TASK_H





#define CHASSIS_GYRO_ANGLE_PID_KP 0.15f
#define CHASSIS_GYRO_ANGLE_PID_KI 0.0f
#define CHASSIS_GYRO_ANGLE_PID_KD 0.0f
#define CHASSIS_GYRO_ANGLE_PID_OUT_MAX 2.0f
#define CHASSIS_GYRO_ANGLE_PID_KI_MAX 0.0f//不建议给

#define CHASSIS_GYRO_SPEED_PID_KP 6000.0f
#define CHASSIS_GYRO_SPEED_PID_KI 0.00f
#define CHASSIS_GYRO_SPEED_PID_KD 0.00f
#define CHASSIS_GYRO_SPEED_PID_OUT_MAX 16384.0f
#define CHASSIS_GYRO_SPEED_PID_KI_MAX 0.0f//不能给


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

//机械腿部参数
//长度单位mm
//角度单位rad

#define LEG_SMALL_LENGTH 250.0f         //小腿长
#define LEG_BIG_LENGTH   210.0f         //大腿长
#define WHEEL_RADIUS     54.0f          //轮毂半径
#define MAX_VIRTUAL_LEG_LENGTH 386.55f     //虚拟腿最大长度
#define MIN_VIRTUAL_LEG_LENGTH 159.69f     //虚拟腿最小长度
#define REDUCTION_RATIO (17.0f/268.0f)  //轮毂电机减速比



struct leg_parameter {
    float angle_07;     //前腿大腿角度(x轴正方向->前大腿)
    float virtual_leg_angle_047; //虚拟腿的角度(x轴正方向->虚拟腿)= 07+04
    float angle_04 ;    //中间角度04（前大腿->虚拟腿，虚拟腿->后大腿）
    float angle_0474 ;   //后大腿角度(x轴正方向->后大腿)

    float virtual_leg_length;//虚拟腿长度


    float leg_x;//腿距离正中心y轴距离
    float leg_z;//腿距离正中心z轴距离
};

extern struct leg_parameter left_leg;
extern struct leg_parameter right_leg;


void chassis_vx_compute_loop();

void chassis_stand_loop();
void chassis_leg_angle_compute_loop();
void chassis_leg_target_position_compute();
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

float calculate_opposite_angle(float a, float b, float c);

#endif //BUBING_RM2025_CHASSIS_TASK_H
