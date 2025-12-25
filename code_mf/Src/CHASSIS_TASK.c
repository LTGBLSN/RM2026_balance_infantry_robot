//
// Created by 21481 on 2025/12/15.
//

#include "cmsis_os.h"
#include "dm_motor.h"
#include "main.h"
#include "pid.h"
#include "CHASSIS_TASK.h"


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
    osDelay(2000);
    dm_motor_mode_set(CMD_MOTOR_MODE,DM8009P_01_LEFT_FRONT);
    osDelay(1);
    dm_motor_mode_set(CMD_MOTOR_MODE,DM8009P_02_RIGHT_FRONT);
    osDelay(1);
    dm_motor_mode_set(CMD_MOTOR_MODE,DM8009P_03_RIGHT_BEHIND);
    osDelay(1);
    dm_motor_mode_set(CMD_MOTOR_MODE,DM8009P_04_LEFT_BEHIND);
    osDelay(1);
    DM8009P_01_LEFT_FRONT.give_angle =   0.15f ;
    DM8009P_02_RIGHT_FRONT.give_angle = -0.15f ;
    DM8009P_03_RIGHT_BEHIND.give_angle = 0.15f ;
    DM8009P_04_LEFT_BEHIND.give_angle = -0.15f ;
    while (1)
    {

        chassis_stand_loop();
        chassis_DM_motor_pid_loop();



        osDelay(1);
    }
}

//直立环pid计算
void chassis_stand_loop()
{
    angular_angle = (float )rc_ch3*0.02f ;
    angular_speed = chassis_gyro_pitch_angle_pid_loop(angular_angle) ;

    chassis_left_3508_id2_given_current = (int16_t )chassis_gyro_pitch_speed_pid_loop(angular_speed);
    chassis_right_3508_id1_given_current = (int16_t )-chassis_gyro_pitch_speed_pid_loop(angular_speed);
}



//关节电机pid计算循环
void chassis_DM_motor_pid_loop()
{
//    DM8009P_01_LEFT_FRONT.give_angle = DM8009P_01_LEFT_FRONT.give_angle + (0.000005f)*(float )rc_ch0 ;
    DM8009P_01_LEFT_FRONT.give_speed = chassis_DM8009_01_angle_pid_loop(DM8009P_01_LEFT_FRONT.give_angle) ;
    DM8009P_01_LEFT_FRONT.give_tor = chassis_DM8009_01_speed_pid_loop(DM8009P_01_LEFT_FRONT.give_speed) ;

//    DM8009P_02_RIGHT_FRONT.give_angle = DM8009P_02_RIGHT_FRONT.give_angle + (0.000005f)*(float )rc_ch2 ;
    DM8009P_02_RIGHT_FRONT.give_speed = chassis_DM8009_02_angle_pid_loop(DM8009P_02_RIGHT_FRONT.give_angle) ;
    DM8009P_02_RIGHT_FRONT.give_tor = chassis_DM8009_02_speed_pid_loop(DM8009P_02_RIGHT_FRONT.give_speed) ;

//    DM8009P_03_RIGHT_BEHIND.give_angle = DM8009P_03_RIGHT_BEHIND.give_angle + (0.000005f)*(float )rc_ch3 ;
    DM8009P_03_RIGHT_BEHIND.give_speed = chassis_DM8009_03_angle_pid_loop(DM8009P_03_RIGHT_BEHIND.give_angle) ;
    DM8009P_03_RIGHT_BEHIND.give_tor = chassis_DM8009_03_speed_pid_loop(DM8009P_03_RIGHT_BEHIND.give_speed) ;

//    DM8009P_04_LEFT_BEHIND.give_angle = DM8009P_04_LEFT_BEHIND.give_angle + (0.000005f)*(float )rc_ch1 ;
    DM8009P_04_LEFT_BEHIND.give_speed = chassis_DM8009_04_angle_pid_loop(DM8009P_04_LEFT_BEHIND.give_angle) ;
    DM8009P_04_LEFT_BEHIND.give_tor = chassis_DM8009_04_speed_pid_loop(DM8009P_04_LEFT_BEHIND.give_speed) ;
}




void chassis_gyro_pitch_angle_pid_init(void)
{
    static fp32 chassis_gyro_angle_kpkikd[3] = {CHASSIS_GYRO_ANGLE_PID_KP, CHASSIS_GYRO_ANGLE_PID_KI, CHASSIS_GYRO_ANGLE_PID_KD};
    PID_init(&chassis_gyro_angle_pid, PID_POSITION, chassis_gyro_angle_kpkikd, CHASSIS_GYRO_ANGLE_PID_OUT_MAX, CHASSIS_GYRO_ANGLE_PID_KI_MAX);

}

float chassis_gyro_pitch_angle_pid_loop(float chassis_gyro_angle_set_loop)
{
    PID_calc(&chassis_gyro_angle_pid, pitch_angle_from_bmi088, chassis_gyro_angle_set_loop);
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
    PID_calc(&chassis_gyro_speed_pid, pitch_speed_from_bmi088, chassis_gyro_speed_set_loop);
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
    PID_calc(&chassis_DM8009_01_speed_pid, DM8009P_01_LEFT_FRONT.return_speed_rpm, chassis_DM8009_01_speed_set_loop);
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
    PID_calc(&chassis_DM8009_01_angle_pid, DM8009P_01_LEFT_FRONT.return_angle, chassis_DM8009_01_angle_set_loop);
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
    PID_calc(&chassis_DM8009_02_speed_pid, DM8009P_02_RIGHT_FRONT.return_speed_rpm, chassis_DM8009_02_speed_set_loop);
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
    PID_calc(&chassis_DM8009_02_angle_pid, DM8009P_02_RIGHT_FRONT.return_angle, chassis_DM8009_02_angle_set_loop);
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
    PID_calc(&chassis_DM8009_03_speed_pid, DM8009P_03_RIGHT_BEHIND.return_speed_rpm, chassis_DM8009_03_speed_set_loop);
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
    PID_calc(&chassis_DM8009_03_angle_pid, DM8009P_03_RIGHT_BEHIND.return_angle, chassis_DM8009_03_angle_set_loop);
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
    PID_calc(&chassis_DM8009_04_speed_pid, DM8009P_04_LEFT_BEHIND.return_speed_rpm, chassis_DM8009_04_speed_set_loop);
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
    PID_calc(&chassis_DM8009_04_angle_pid, DM8009P_04_LEFT_BEHIND.return_angle, chassis_DM8009_04_angle_set_loop);
    float chassis_DM8009_04_given_speed_loop = (float )(chassis_DM8009_04_angle_pid.out);

    return chassis_DM8009_04_given_speed_loop ;

}

