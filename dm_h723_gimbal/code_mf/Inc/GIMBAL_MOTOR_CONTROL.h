//
// Created by 21481 on 2026/2/12.
//

#ifndef DM_H723_LIB_GIMBAL_MOTOR_CONTROL_H
#define DM_H723_LIB_GIMBAL_MOTOR_CONTROL_H



#define YAW_6020_ID1_ANGLE_PID_KP        0.3f//0.2f
#define YAW_6020_ID1_ANGLE_PID_KI        0.0f
#define YAW_6020_ID1_ANGLE_PID_KD        0.8f
#define YAW_6020_ID1_ANGLE_PID_OUT_MAX   30.0f
#define YAW_6020_ID1_ANGLE_PID_KI_MAX    0.0f

#define YAW_6020_ID1_SPEED_PID_KP        15000.0f//20000.0f
#define YAW_6020_ID1_SPEED_PID_KI        40.0f
#define YAW_6020_ID1_SPEED_PID_KD        0.0f
#define YAW_6020_ID1_SPEED_PID_OUT_MAX   25000.0f
#define YAW_6020_ID1_SPEED_PID_KI_MAX    20000.0f


#define PITCH_6020_ID2_SPEED_PID_KP        10000.0f//15000.0f带上测速模块
#define PITCH_6020_ID2_SPEED_PID_KI        80.0f
#define PITCH_6020_ID2_SPEED_PID_KD        0.0f
#define PITCH_6020_ID2_SPEED_PID_OUT_MAX   25000.0f
#define PITCH_6020_ID2_SPEED_PID_KI_MAX    13000.0f

#define PITCH_6020_ID2_ANGLE_PID_KP        0.5f
#define PITCH_6020_ID2_ANGLE_PID_KI        0.0f
#define PITCH_6020_ID2_ANGLE_PID_KD        1.0f
#define PITCH_6020_ID2_ANGLE_PID_OUT_MAX   10.0f
#define PITCH_6020_ID2_ANGLE_PID_KI_MAX    0.0f



#define FRICTION_WHEEL_3510_ID1_SPEED_PID_KP        8.0f
#define FRICTION_WHEEL_3510_ID1_SPEED_PID_KI        0.1f
#define FRICTION_WHEEL_3510_ID1_SPEED_PID_KD        0.0f
#define FRICTION_WHEEL_3510_ID1_SPEED_PID_OUT_MAX   16000.0f
#define FRICTION_WHEEL_3510_ID1_SPEED_PID_KI_MAX    10000.0f

#define FRICTION_WHEEL_3510_ID2_SPEED_PID_KP        8.0f
#define FRICTION_WHEEL_3510_ID2_SPEED_PID_KI        0.1f
#define FRICTION_WHEEL_3510_ID2_SPEED_PID_KD        0.0f
#define FRICTION_WHEEL_3510_ID2_SPEED_PID_OUT_MAX   16000.0f
#define FRICTION_WHEEL_3510_ID2_SPEED_PID_KI_MAX    10000.0f


#define FRICTION_WHEEL_SHOOT_SPEED 7400

#define PITCH_RC_IN_KP (-0.0005f)
#define YAW_RC_IN_KP (-0.001f)

#define PITCH_ANGLE_MAX (-20.0f)
#define PITCH_ANGLE_MIN (25.0f)


#define MOUSE_IN_PITCH_KP (-0.001f)
#define MOUSE_IN_YAW_KP (-0.001f)


void mouse_pitch_input_limiter();

void mouse_yaw_input_normalization();


void motor_gimbal_angle_compute();//云台角度计算
void rc_pitch_input_limiter();//云台pitch输入限幅
void rc_yaw_input_normalization();//yaw轴归一化

void pid_preprocess();//pid预处理

void motor_gimbal_pid_compute();//云台pid计算


void pitch_speed_from_bmi88_pid_init(void);
float pitch_speed_from_bmi088_pid_loop(float PITCH_6020_ID2_speed_set_loop);
void pitch_angle_pid_init(void);
float pitch_angle_from_bmi088_pid_loop(float PITCH_6020_ID2_angle_set_loop);

void yaw_speed_pid_init(void);
float yaw_speed_pid_loop(float YAW_6020_ID1_speed_set_loop);
void yaw_angle_pid_init(void);
float yaw_angle_pid_loop(float YAW_6020_ID1_angle_set_loop);





void friction_wheel_speed_control();
void friction_wheel_pid_control();

void friction_wheel_3510_id1_speed_pid_init(void);
int16_t friction_wheel_3510_id1_speed_pid_loop(int16_t friction_wheel_3510_id1_speed_set_loop);
void friction_wheel_3510_id2_speed_pid_init(void);
int16_t friction_wheel_3510_id2_speed_pid_loop(int16_t friction_wheel_3510_id2_speed_set_loop);



#endif //DM_H723_LIB_GIMBAL_MOTOR_CONTROL_H
