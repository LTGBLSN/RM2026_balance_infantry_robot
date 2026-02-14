//
// Created by 21481 on 2025/12/15.
//

#include <math.h>
#include "cmsis_os.h"
#include "dm_motor.h"
#include "main.h"
#include "pid.h"
#include "CHASSIS_TASK.h"
#include "CAN_receive.h"
#include "DM_IMU.h"

struct leg_parameter left_leg;
struct leg_parameter right_leg;


pid_type_def chassis_DM8009_01_speed_pid;
pid_type_def chassis_DM8009_02_speed_pid;
pid_type_def chassis_DM8009_03_speed_pid;
pid_type_def chassis_DM8009_04_speed_pid;

pid_type_def chassis_DM8009_01_angle_pid;
pid_type_def chassis_DM8009_02_angle_pid;
pid_type_def chassis_DM8009_03_angle_pid;
pid_type_def chassis_DM8009_04_angle_pid;

pid_type_def chassis_gyro_speed_pid;
pid_type_def chassis_gyro_angle_pid;




void CHASSIS_TASK()
{
    while (1)
    {
        chassis_vx_compute_loop();//整车速度估计

        chassis_stand_loop();//轮毂直立


        //腿部目标位置计算//2025.12.29直接给的值
        chassis_leg_target_position_compute();


        //关节电机角度逆解//2025.12.29主要工作部分
        chassis_leg_angle_compute_loop();


        chassis_DM_motor_pid_loop();//腿部关节闭环




        osDelay(1);
    }
}


//整车速度估计
void chassis_vx_compute_loop()
{
//    chassis_vx_real_speed = ( ( (float )motor_can1_data[0].speed_rpm - (float )motor_can1_data[1].speed_rpm ) / 2.0f ) ;


}



//轮毂直立
void chassis_stand_loop()
{

//    if(rc_s1 == 3)
//    {
//        angular_speed = chassis_gyro_pitch_angle_pid_loop((float )rc_ch3*0.02f) ;
//    } else
//    {
//        angular_speed = chassis_gyro_pitch_angle_pid_loop(angular_angle) ;
//
//    }
//
//
//    chassis_left_3508_id2_stand_current = (int16_t )chassis_gyro_pitch_speed_pid_loop(angular_speed);
//    chassis_right_3508_id1_stand_current = (int16_t )-chassis_gyro_pitch_speed_pid_loop(angular_speed);







}


//腿部目标位置计算
//2025.12.29主要工作部分
void chassis_leg_target_position_compute()
{
    //给虚拟腿长和虚拟腿的目标摆角赋值(在区间范围内)
    left_leg.virtual_leg_length = MIN_VIRTUAL_LEG_LENGTH ;
    left_leg.virtual_leg_angle_047 = M_PI_2 ;

    right_leg.virtual_leg_length = MIN_VIRTUAL_LEG_LENGTH ;
    right_leg.virtual_leg_angle_047 = M_PI_2 ;







}



//关节电机角度逆解
void chassis_leg_angle_compute_loop()
{
    //已经有了虚拟腿长和虚拟腿的目标摆角

    //利用余弦函数计算04角度
    left_leg.angle_04 = calculate_opposite_angle(LEG_SMALL_LENGTH, LEG_BIG_LENGTH, left_leg.virtual_leg_length);
    right_leg.angle_04 = calculate_opposite_angle(LEG_SMALL_LENGTH, LEG_BIG_LENGTH, right_leg.virtual_leg_length);


    if(left_leg.angle_04 < 0.0f | right_leg.angle_04 < 0.0f)
    {
        //解算失败
        DM8009P_01_RIGHT_BIG_LEG_FRONT.give_angle =   0.15f ;
        DM8009P_02_LEFT_BIG_LEG_FRONT.give_angle = -0.15f ;
        DM8009P_03_LEFT_SMALL_LEG_BEHIND.give_angle = 0.15f ;
        DM8009P_04_RIGHT_SMALL_LEG_BEHIND.give_angle = -0.15f ;
    } else
    {
        //得出07角度(047-04)
        left_leg.angle_07 = left_leg.virtual_leg_angle_047 - left_leg.angle_04 ;
        right_leg.angle_07 = right_leg.virtual_leg_angle_047 - right_leg.angle_04 ;

        //得出0474角度(047+04)
        left_leg.angle_0474 = left_leg.virtual_leg_angle_047 + left_leg.angle_04 ;
        right_leg.angle_0474 = right_leg.virtual_leg_angle_047 + right_leg.angle_04 ;


        //赋值到电机参数结构体中
        DM8009P_02_LEFT_BIG_LEG_FRONT.give_angle = -(float )(M_PI - left_leg.angle_0474) ;
        DM8009P_03_LEFT_SMALL_LEG_BEHIND.give_angle = left_leg.angle_07 ;

        DM8009P_01_RIGHT_BIG_LEG_FRONT.give_angle = (float )(M_PI - right_leg.angle_0474) ;
        DM8009P_04_RIGHT_SMALL_LEG_BEHIND.give_angle = -right_leg.angle_07 ;

    }



}


//腿部关节闭环
void chassis_DM_motor_pid_loop()
{
//    DM8009P_01_LEFT_FRONT.give_angle = DM8009P_01_LEFT_FRONT.give_angle + (0.000005f)*(float )rc_ch0 ;
    DM8009P_01_RIGHT_BIG_LEG_FRONT.give_speed = chassis_DM8009_01_angle_pid_loop(DM8009P_01_RIGHT_BIG_LEG_FRONT.give_angle) ;
    DM8009P_01_RIGHT_BIG_LEG_FRONT.give_tor = chassis_DM8009_01_speed_pid_loop(DM8009P_01_RIGHT_BIG_LEG_FRONT.give_speed) ;

//    DM8009P_02_RIGHT_FRONT.give_angle = DM8009P_02_RIGHT_FRONT.give_angle + (0.000005f)*(float )rc_ch2 ;
    DM8009P_02_LEFT_BIG_LEG_FRONT.give_speed = chassis_DM8009_02_angle_pid_loop(DM8009P_02_LEFT_BIG_LEG_FRONT.give_angle) ;
    DM8009P_02_LEFT_BIG_LEG_FRONT.give_tor = chassis_DM8009_02_speed_pid_loop(DM8009P_02_LEFT_BIG_LEG_FRONT.give_speed) ;

//    DM8009P_03_RIGHT_BEHIND.give_angle = DM8009P_03_RIGHT_BEHIND.give_angle + (0.000005f)*(float )rc_ch3 ;
    DM8009P_03_LEFT_SMALL_LEG_BEHIND.give_speed = chassis_DM8009_03_angle_pid_loop(DM8009P_03_LEFT_SMALL_LEG_BEHIND.give_angle) ;
    DM8009P_03_LEFT_SMALL_LEG_BEHIND.give_tor = chassis_DM8009_03_speed_pid_loop(DM8009P_03_LEFT_SMALL_LEG_BEHIND.give_speed) ;

//    DM8009P_04_LEFT_BEHIND.give_angle = DM8009P_04_LEFT_BEHIND.give_angle + (0.000005f)*(float )rc_ch1 ;
    DM8009P_04_RIGHT_SMALL_LEG_BEHIND.give_speed = chassis_DM8009_04_angle_pid_loop(DM8009P_04_RIGHT_SMALL_LEG_BEHIND.give_angle) ;
    DM8009P_04_RIGHT_SMALL_LEG_BEHIND.give_tor = chassis_DM8009_04_speed_pid_loop(DM8009P_04_RIGHT_SMALL_LEG_BEHIND.give_speed) ;
}








void chassis_gyro_pitch_angle_pid_init(void)
{
    static fp32 chassis_gyro_angle_kpkikd[3] = {CHASSIS_GYRO_ANGLE_PID_KP, CHASSIS_GYRO_ANGLE_PID_KI, CHASSIS_GYRO_ANGLE_PID_KD};
    PID_init(&chassis_gyro_angle_pid, PID_POSITION, chassis_gyro_angle_kpkikd, CHASSIS_GYRO_ANGLE_PID_OUT_MAX, CHASSIS_GYRO_ANGLE_PID_KI_MAX);

}

float chassis_gyro_pitch_angle_pid_loop(float chassis_gyro_angle_set_loop)
{
    PID_calc(&chassis_gyro_angle_pid,DM_imu.pitch, chassis_gyro_angle_set_loop);
    float chassis_gyro_given_speed_loop = (float )(chassis_gyro_angle_pid.out);

    return chassis_gyro_given_speed_loop ;

}

void chassis_gyro_pitch_speed_pid_init(void)
{
    static fp32 chassis_gyro_speed_kpkikd[3] = {CHASSIS_GYRO_SPEED_PID_KP, CHASSIS_GYRO_SPEED_PID_KI, CHASSIS_GYRO_SPEED_PID_KD};
    PID_init(&chassis_gyro_speed_pid, PID_POSITION, chassis_gyro_speed_kpkikd, CHASSIS_GYRO_SPEED_PID_OUT_MAX, CHASSIS_GYRO_SPEED_PID_KI_MAX);

}

float chassis_gyro_pitch_speed_pid_loop(float chassis_gyro_speed_set_loop)
{
    PID_calc(&chassis_gyro_speed_pid, DM_imu.gyro[PITCH], chassis_gyro_speed_set_loop);
    float chassis_gyro_given_current_loop = (float )(chassis_gyro_speed_pid.out);

    return chassis_gyro_given_current_loop ;

}






//1号电机
void chassis_DM8009_01_speed_pid_init(void)
{
    static fp32 chassis_DM8009_01_speed_kpkikd[3] = {CHASSIS_DM8009_01_SPEED_PID_KP, CHASSIS_DM8009_01_SPEED_PID_KI, CHASSIS_DM8009_01_SPEED_PID_KD};
    PID_init(&chassis_DM8009_01_speed_pid, PID_POSITION, chassis_DM8009_01_speed_kpkikd, CHASSIS_DM8009_SPEED_PID_OUT_MAX, CHASSIS_DM8009_SPEED_PID_KI_MAX);

}

float chassis_DM8009_01_speed_pid_loop(float chassis_DM8009_01_speed_set_loop)
{
    PID_calc(&chassis_DM8009_01_speed_pid, DM8009P_01_RIGHT_BIG_LEG_FRONT.return_speed, chassis_DM8009_01_speed_set_loop);
    float chassis_DM8009_01_given_current_loop = (float )(chassis_DM8009_01_speed_pid.out);

    return chassis_DM8009_01_given_current_loop ;

}

//1号电机
void chassis_DM8009_01_angle_pid_init(void)
{
    static fp32 chassis_DM8009_01_angle_kpkikd[3] = {CHASSIS_DM8009_01_ANGLE_PID_KP, CHASSIS_DM8009_01_ANGLE_PID_KI, CHASSIS_DM8009_01_ANGLE_PID_KD};
    PID_init(&chassis_DM8009_01_angle_pid, PID_POSITION, chassis_DM8009_01_angle_kpkikd, CHASSIS_DM8009_ANGLE_PID_OUT_MAX, CHASSIS_DM8009_ANGLE_PID_KI_MAX);

}

float chassis_DM8009_01_angle_pid_loop(float chassis_DM8009_01_angle_set_loop)
{
    PID_calc(&chassis_DM8009_01_angle_pid, DM8009P_01_RIGHT_BIG_LEG_FRONT.return_angle, chassis_DM8009_01_angle_set_loop);
    float chassis_DM8009_01_given_speed_loop = (float )(chassis_DM8009_01_angle_pid.out);

    return chassis_DM8009_01_given_speed_loop ;

}




//2号电机
void chassis_DM8009_02_speed_pid_init(void)
{
    static fp32 chassis_DM8009_02_speed_kpkikd[3] = {CHASSIS_DM8009_02_SPEED_PID_KP, CHASSIS_DM8009_02_SPEED_PID_KI, CHASSIS_DM8009_02_SPEED_PID_KD};
    PID_init(&chassis_DM8009_02_speed_pid, PID_POSITION, chassis_DM8009_02_speed_kpkikd, CHASSIS_DM8009_SPEED_PID_OUT_MAX, CHASSIS_DM8009_SPEED_PID_KI_MAX);

}

float chassis_DM8009_02_speed_pid_loop(float chassis_DM8009_02_speed_set_loop)
{
    PID_calc(&chassis_DM8009_02_speed_pid, DM8009P_02_LEFT_BIG_LEG_FRONT.return_speed, chassis_DM8009_02_speed_set_loop);
    float chassis_DM8009_02_given_current_loop = (float )(chassis_DM8009_02_speed_pid.out);

    return chassis_DM8009_02_given_current_loop ;

}

//2号电机
void chassis_DM8009_02_angle_pid_init(void)
{
    static fp32 chassis_DM8009_02_angle_kpkikd[3] = {CHASSIS_DM8009_02_ANGLE_PID_KP, CHASSIS_DM8009_02_ANGLE_PID_KI, CHASSIS_DM8009_02_ANGLE_PID_KD};
    PID_init(&chassis_DM8009_02_angle_pid, PID_POSITION, chassis_DM8009_02_angle_kpkikd, CHASSIS_DM8009_ANGLE_PID_OUT_MAX, CHASSIS_DM8009_ANGLE_PID_KI_MAX);

}

float chassis_DM8009_02_angle_pid_loop(float chassis_DM8009_02_angle_set_loop)
{
    PID_calc(&chassis_DM8009_02_angle_pid, DM8009P_02_LEFT_BIG_LEG_FRONT.return_angle, chassis_DM8009_02_angle_set_loop);
    float chassis_DM8009_02_given_speed_loop = (float )(chassis_DM8009_02_angle_pid.out);

    return chassis_DM8009_02_given_speed_loop ;

}


//3号电机
void chassis_DM8009_03_speed_pid_init(void)
{
    static fp32 chassis_DM8009_03_speed_kpkikd[3] = {CHASSIS_DM8009_03_SPEED_PID_KP, CHASSIS_DM8009_03_SPEED_PID_KI, CHASSIS_DM8009_03_SPEED_PID_KD};
    PID_init(&chassis_DM8009_03_speed_pid, PID_POSITION, chassis_DM8009_03_speed_kpkikd, CHASSIS_DM8009_SPEED_PID_OUT_MAX, CHASSIS_DM8009_SPEED_PID_KI_MAX);

}

float chassis_DM8009_03_speed_pid_loop(float chassis_DM8009_03_speed_set_loop)
{
    PID_calc(&chassis_DM8009_03_speed_pid, DM8009P_03_LEFT_SMALL_LEG_BEHIND.return_speed, chassis_DM8009_03_speed_set_loop);
    float chassis_DM8009_03_given_current_loop = (float )(chassis_DM8009_03_speed_pid.out);

    return chassis_DM8009_03_given_current_loop ;

}

//3号电机
void chassis_DM8009_03_angle_pid_init(void)
{
    static fp32 chassis_DM8009_03_angle_kpkikd[3] = {CHASSIS_DM8009_03_ANGLE_PID_KP, CHASSIS_DM8009_03_ANGLE_PID_KI, CHASSIS_DM8009_03_ANGLE_PID_KD};
    PID_init(&chassis_DM8009_03_angle_pid, PID_POSITION, chassis_DM8009_03_angle_kpkikd, CHASSIS_DM8009_ANGLE_PID_OUT_MAX, CHASSIS_DM8009_ANGLE_PID_KI_MAX);

}

float chassis_DM8009_03_angle_pid_loop(float chassis_DM8009_03_angle_set_loop)
{
    PID_calc(&chassis_DM8009_03_angle_pid, DM8009P_03_LEFT_SMALL_LEG_BEHIND.return_angle, chassis_DM8009_03_angle_set_loop);
    float chassis_DM8009_03_given_speed_loop = (float )(chassis_DM8009_03_angle_pid.out);

    return chassis_DM8009_03_given_speed_loop ;

}

//4号电机
void chassis_DM8009_04_speed_pid_init(void)
{
    static fp32 chassis_DM8009_04_speed_kpkikd[3] = {CHASSIS_DM8009_04_SPEED_PID_KP, CHASSIS_DM8009_04_SPEED_PID_KI, CHASSIS_DM8009_04_SPEED_PID_KD};
    PID_init(&chassis_DM8009_04_speed_pid, PID_POSITION, chassis_DM8009_04_speed_kpkikd, CHASSIS_DM8009_SPEED_PID_OUT_MAX, CHASSIS_DM8009_SPEED_PID_KI_MAX);

}

float chassis_DM8009_04_speed_pid_loop(float chassis_DM8009_04_speed_set_loop)
{
    PID_calc(&chassis_DM8009_04_speed_pid, DM8009P_04_RIGHT_SMALL_LEG_BEHIND.return_speed, chassis_DM8009_04_speed_set_loop);
    float chassis_DM8009_04_given_current_loop = (float )(chassis_DM8009_04_speed_pid.out);

    return chassis_DM8009_04_given_current_loop ;

}

//4号电机
void chassis_DM8009_04_angle_pid_init(void)
{
    static fp32 chassis_DM8009_04_angle_kpkikd[3] = {CHASSIS_DM8009_04_ANGLE_PID_KP, CHASSIS_DM8009_04_ANGLE_PID_KI, CHASSIS_DM8009_04_ANGLE_PID_KD};
    PID_init(&chassis_DM8009_04_angle_pid, PID_POSITION, chassis_DM8009_04_angle_kpkikd, CHASSIS_DM8009_ANGLE_PID_OUT_MAX, CHASSIS_DM8009_ANGLE_PID_KI_MAX);

}

float chassis_DM8009_04_angle_pid_loop(float chassis_DM8009_04_angle_set_loop)
{
    PID_calc(&chassis_DM8009_04_angle_pid, DM8009P_04_RIGHT_SMALL_LEG_BEHIND.return_angle, chassis_DM8009_04_angle_set_loop);
    float chassis_DM8009_04_given_speed_loop = (float )(chassis_DM8009_04_angle_pid.out);

    return chassis_DM8009_04_given_speed_loop ;

}




/**
 * @brief 使用余弦定理计算三角形中指定边的对角
 * @param a 第一条边的长度（对角将被计算）
 * @param b 第二条边的长度
 * @param c 第三条边的长度
 * @return 返回边a的对角，单位为弧度；如果输入不构成有效三角形则返回-1
 */
float calculate_opposite_angle(float a, float b, float c)
{
    // 检查是否构成有效的三角形
    if (a <= 0 || b <= 0 || c <= 0) {
        return -1.0f; // 边长必须为正
    }

    // 三角形不等式检查
    if ((a + b <= c) || (a + c <= b) || (b + c <= a)) {
        return -1.0f; // 不满足三角形不等式
    }

    // 使用余弦定理: cos(A) = (b? + c? - a?) / (2bc)
    float cos_angle = (b * b + c * c - a * a) / (2.0f * b * c);

    // 确保cos值在有效范围内 [-1, 1]，防止浮点数精度问题
    if (cos_angle > 1.0f) {
        cos_angle = 1.0f;
    } else if (cos_angle < -1.0f) {
        cos_angle = -1.0f;
    }

    // 计算角度（弧度）
    float angle_radians = acosf(cos_angle);

    return angle_radians;
}

