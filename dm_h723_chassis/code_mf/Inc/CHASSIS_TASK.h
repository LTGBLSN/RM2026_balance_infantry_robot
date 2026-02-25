//
// Created by 21481 on 2025/12/15.
//

#ifndef BUBING_RM2025_CHASSIS_TASK_H
#define BUBING_RM2025_CHASSIS_TASK_H


#define CONTROL_LOOP_PERIOD_MS 1
#define CONTROL_LOOP_DT (CONTROL_LOOP_PERIOD_MS/1000.0f)





#define CHASSIS_GYRO_ANGLE_PID_KP 0.35f
#define CHASSIS_GYRO_ANGLE_PID_KI 0.0f
#define CHASSIS_GYRO_ANGLE_PID_KD 0.0f
#define CHASSIS_GYRO_ANGLE_PID_OUT_MAX 2.0f
#define CHASSIS_GYRO_ANGLE_PID_KI_MAX 0.0f//不建议给

#define CHASSIS_GYRO_SPEED_PID_KP 5000.0f
#define CHASSIS_GYRO_SPEED_PID_KI 0.00f
#define CHASSIS_GYRO_SPEED_PID_KD 0.00f
#define CHASSIS_GYRO_SPEED_PID_OUT_MAX 16384.0f
#define CHASSIS_GYRO_SPEED_PID_KI_MAX 0.0f//不能给


#define CHASSIS_DM8009_ANGLE_PID_OUT_MAX 2.0f//3.0f
#define CHASSIS_DM8009_ANGLE_PID_KI_MAX 0.0f

#define CHASSIS_DM8009_SPEED_PID_OUT_MAX   34.0f//54.0f
#define CHASSIS_DM8009_SPEED_PID_KI_MAX   10.0f


#define CHASSIS_DM8009_01_SPEED_PID_KP   10.0f
#define CHASSIS_DM8009_01_SPEED_PID_KI   0.03f
#define CHASSIS_DM8009_01_SPEED_PID_KD   0.0f
#define CHASSIS_DM8009_01_ANGLE_PID_KP   30.0f
#define CHASSIS_DM8009_01_ANGLE_PID_KI   0.0f
#define CHASSIS_DM8009_01_ANGLE_PID_KD   0.0f


#define CHASSIS_DM8009_02_SPEED_PID_KP   10.0f
#define CHASSIS_DM8009_02_SPEED_PID_KI   0.03f
#define CHASSIS_DM8009_02_SPEED_PID_KD   0.0f
#define CHASSIS_DM8009_02_ANGLE_PID_KP   30.0f
#define CHASSIS_DM8009_02_ANGLE_PID_KI   0.0f
#define CHASSIS_DM8009_02_ANGLE_PID_KD   0.0f

#define CHASSIS_DM8009_03_SPEED_PID_KP   9.0f
#define CHASSIS_DM8009_03_SPEED_PID_KI   0.03f
#define CHASSIS_DM8009_03_SPEED_PID_KD   0.0f
#define CHASSIS_DM8009_03_ANGLE_PID_KP   30.0f
#define CHASSIS_DM8009_03_ANGLE_PID_KI   0.0f
#define CHASSIS_DM8009_03_ANGLE_PID_KD   0.0f


#define CHASSIS_DM8009_04_SPEED_PID_KP   9.0f
#define CHASSIS_DM8009_04_SPEED_PID_KI   0.03f
#define CHASSIS_DM8009_04_SPEED_PID_KD   0.0f
#define CHASSIS_DM8009_04_ANGLE_PID_KP   30.0f
#define CHASSIS_DM8009_04_ANGLE_PID_KI   0.0f
#define CHASSIS_DM8009_04_ANGLE_PID_KD   0.0f


#define LEG_PID_OUT_MAX  150.0f//注意这里要跟前馈配合
#define LEG_PID_KI_MAX   0.0f


#define LEFT_LEG_PID_KP       3000.0f
#define LEFT_LEG_PID_KI       0.0f
#define LEFT_LEG_PID_KD       100000.0f

#define RIGHT_LEG_PID_KP       3000.0f
#define RIGHT_LEG_PID_KI       0.0f
#define RIGHT_LEG_PID_KD       100000.0f



//机械腿部参数
//长度单位mm
//角度单位rad

#define LEG_SMALL_LENGTH 0.2500f            //小腿长单位（m）
#define LEG_BIG_LENGTH   0.2100f            //大腿长单位（m）
#define WHEEL_RADIUS     0.054f             //轮毂半径,单位(m)
#define MAX_VIRTUAL_LEG_LENGTH 0.38655f     //虚拟腿最大长度单位（m）
#define MIN_VIRTUAL_LEG_LENGTH 0.15969f     //虚拟腿最小长度单位（m）
#define REDUCTION_RATIO (268.0f/17.0f)      //轮毂电机减速比
#define motor_max_torque (3.0f/19.0f)       //裸电机最大扭矩单位（N·m）
#define MOTOR_MAX_TORQUE (motor_max_torque * REDUCTION_RATIO) // 最大扭矩
#define MAX_CMD 16384.0f                    // 控制器满量程
#define MOTOR_GIVE_TORQUE_KP (MAX_CMD / MOTOR_MAX_TORQUE) // 扭矩系数转换

#define DM8009_MAX_TOR 20.0f



struct chassis_lqr_state_input {
    float finial_lqr_compute_leg_length;                   //腿长 单位m

    float chassis_pitch_speed_rad_s ;   //机体俯仰角速度（由 IMU 获取）单位rad/s      方向：后仰为正！
    float pitch_angle_rad ;             //机体与水平面夹角（机体俯仰角，由 IMU 获取）单位rad     方向：后仰为正！

    float virtual_leg_speed_rad_s ;     //腿部角速度(imu+正解的角速度，同摆杆角) 单位rad/s      方向：同摆杆角
    float virtual_leg_angle_rad ;       //摆杆与竖直方向夹角（腿部角度，imu+编码器得到的相对竖直向上的摆角，原点为轮子，查哈工程建模那张图）单位rad 零点：竖直向上   方向：从竖直向上->轮子和车身转轴连线

    float chassis_speed_m_s ;           //整车速度 单位m/s      方向：前进为正！
    float chassis_move_x_m ;            //驱动轮位移     方向：前进为正！

};



struct leg_parameter {
    float angle_07;     //前腿大腿角度(x轴正方向->前大腿)
    float virtual_leg_angle_047; //虚拟腿的角度(x轴正方向->虚拟腿)= 07+04
    float angle_04 ;    //中间角度04（前大腿->虚拟腿，虚拟腿->后大腿）
    float angle_0474 ;   //后大腿角度(x轴正方向->后大腿)

    float return_virtual_leg_length;//当前用于计算lqr的虚拟腿长度
    float goal_virtual_leg_length;//腿长控制的目标腿长

    float K;            //计算中间变量
    float jacobian[2][2];     //雅可比矩阵
    float jacobian_T[2][2];   //雅可比矩阵的转置

    float L_dt;        // 腿部伸缩速度
    float theta47_dt;  // 虚线转动角速度

    float virtual_leg_give_tor;//虚拟腿目标推力

    float tor7;         // 电机7目标力矩
    float tor2;         // 电机2目标力矩
};



extern struct  chassis_lqr_state_input chassis_LQR_compute_left_finial_state;
extern struct  chassis_lqr_state_input chassis_LQR_compute_right_finial_state;

extern struct leg_parameter left_leg_goal_2_joint;
extern struct leg_parameter right_leg_goal_2_joint;

extern struct leg_parameter left_leg_joint_2_leg_parameters;
extern struct leg_parameter right_leg_joint_2_leg_parameters;


void chassis_all_state_update_loop();
float chassis_vx_compute_loop();

void chassis_pid_stand_loop();
float calculate_lqr_control_loop(float L, struct chassis_lqr_state_input state);
void chassis_joint_angle_compute_loop();
void chassis_leg_angle_compute_loop();
void chassis_leg_target_position_compute();
void chassis_DM_motor_pid_loop();


void get_jacobian();
float compute_jacobian_K(float theta4, float L1, float L2);
void get_leg_velocity();
void virtual_leg_goal_compute();
void virtual_leg_give_tor_compute();
void joint_vmc_compute(float left_tor_47, float left_virtual_leg_tor , float right_tor_47, float right_virtual_leg_tor);
void joint_tor_Limit(float motor1 , float motor2 , float motor3 , float motor4);

float calculate_opposite_angle(float a, float b, float c);
float calculate_side_c(float a, float b, float angleB_rad);

void left_leg_pid_init(void);
float left_leg_pid_loop(float left_leg_set_loop);
void right_leg_pid_init(void);
float right_leg_pid_loop(float right_leg_set_loop);




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
