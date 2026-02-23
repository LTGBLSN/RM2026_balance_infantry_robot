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
#include "GET_RC_TASK.h"
#include "DJI_motors.h"
#include "IMU_DATA_GET.h"

struct  chassis_lqr_state_input chassis_all_finial_state;

struct leg_parameter left_leg_goal_2_joint;
struct leg_parameter right_leg_goal_2_joint;

struct leg_parameter left_leg_joint_2_leg_parameters;
struct leg_parameter right_leg_joint_2_leg_parameters;


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
        //整车状态变量更新
        chassis_all_state_update_loop();

        //lqr计算
        if((rcData.rc.s[0]) == 1)
        {
            compute_current = (0.5f * MOTOR_GIVE_TORQUE_KP * calculate_lqr_control_loop(chassis_all_finial_state.leg_length, chassis_all_finial_state));

//            if(compute_current < -MAX_CMD)
//            {
//                compute_current = -MAX_CMD ;
//            }
//            else if(compute_current > MAX_CMD)
//            {
//                compute_current = MAX_CMD ;
//            } else
//            {
//                compute_current = compute_current ;
//            }

            chassis_right_3508_id1_stand_current = (int16_t) compute_current ;
            chassis_left_3508_id2_stand_current = (int16_t) -compute_current ;



//            chassis_left_3508_id2_stand_current = (int16_t)(-0.5f * MOTOR_GIVE_TORQUE_KP * calculate_lqr_control_loop(chassis_all_finial_state.leg_length, chassis_all_finial_state));
//            chassis_right_3508_id1_stand_current = (int16_t)(0.5f * MOTOR_GIVE_TORQUE_KP * calculate_lqr_control_loop(chassis_all_finial_state.leg_length, chassis_all_finial_state));
        }





        //执行控制


//        chassis_pid_stand_loop();//轮毂直立


        //腿部目标位置计算//2025.12.29直接给的值
        chassis_leg_target_position_compute();
        //关节电机角度逆解//2025.12.29主要工作部分
        chassis_joint_angle_compute_loop();

        if((rcData.rc.s[0]) == 1)
        {
            chassis_DM_motor_pid_loop();//腿部关节闭环
        }
        if((rcData.rc.s[0]) == 3)
        {
            chassis_DM_motor_pid_loop();//腿部关节闭环
        }





        osDelay(CONTROL_LOOP_PERIOD_MS);
    }
}

void chassis_all_state_update_loop()
{
    //车身倾角计算，注意正方向！
    chassis_all_finial_state.chassis_pitch_speed_rad_s = -imu_data_from_external_BMI088_mahony.pitch_radian_vel;
    chassis_all_finial_state.pitch_angle_rad = -imu_data_from_external_BMI088_mahony.pitch_radian_angle;


    //关节电机正解，得到相对车身的角度和摆杆长度
    chassis_leg_angle_compute_loop();
    //计算结果更新在如下变量里面
    //left_leg_joint_2_leg_parameters.virtual_leg_length
    //left_leg_joint_2_leg_parameters.virtual_leg_angle_047
    //right_leg_joint_2_leg_parameters.virtual_leg_length
    //right_leg_joint_2_leg_parameters.virtual_leg_angle_047

    //计算摆杆长度（作为整车的k矩阵选取因素）
    // 2026.2.21：不确定直接取两个腿的平均长度是否可以，板凳及平地没有问题，单边桥可能要在这地方做些文章
    chassis_all_finial_state.leg_length =
            (left_leg_joint_2_leg_parameters.virtual_leg_length + right_leg_joint_2_leg_parameters.virtual_leg_length) / 2.0f;

    //摆杆角度 = 车身角度+虚拟腿相对车身角度-90度（因为virtual_leg_angle_047是从水平向下向后旋转到虚拟腿）
    // 2026.2.21：不确定直接取两个腿的平均摆角是否可以，板凳及平地没有问题，单边桥可能要在这地方做些文章
    chassis_all_finial_state.virtual_leg_angle_rad =
            - chassis_all_finial_state.pitch_angle_rad
            + (left_leg_joint_2_leg_parameters.virtual_leg_angle_047 + right_leg_joint_2_leg_parameters.virtual_leg_angle_047) / 2.0f
            - (float )M_PI_2;


    //摆杆角速度先不管了，那车身角速度先用，反正lqr小板凳
    chassis_all_finial_state.virtual_leg_speed_rad_s = - chassis_all_finial_state.chassis_pitch_speed_rad_s;





    //整车速度计算2026.2.21:暂时仅使用轮毂速度，可能需要做滤波
    chassis_all_finial_state.chassis_speed_m_s = chassis_vx_compute_loop();

    //整车位移计算2026.2.21:使用速度积分得到位置，不保证100%可用，待确认
    chassis_all_finial_state.chassis_move_x_m =
            chassis_all_finial_state.chassis_move_x_m
            + chassis_all_finial_state.chassis_speed_m_s * CONTROL_LOOP_DT;



}




//整车速度估计
float chassis_vx_compute_loop()
{
    //取两个轮子平均速度除减速比得到轮毂转速单位RPM，再通过轮子直径算出速度，单位m/s
    //此部分可优化，浮点计算多，可优化为直接×单一系数仅经过一次运算
    float angular_motor_speed_rpm = ( ( (float)motor_can2_data[0].speed_rpm - (float)motor_can2_data[1].speed_rpm ) / 2.0f ) ;
    float angular_motor_speed_rad_s = (angular_motor_speed_rpm * 2.0f * (float )M_PI ) / 60.0f ;
    float angular_wheel_speed_rad_s = angular_motor_speed_rad_s / REDUCTION_RATIO;
    float angular_wheel_linear_velocity = angular_wheel_speed_rad_s * WHEEL_RADIUS;

    float chassis_vx_real_speed = angular_wheel_linear_velocity;

    return chassis_vx_real_speed;


}



//轮毂直立
void chassis_pid_stand_loop()
{


    angular_speed = chassis_gyro_pitch_angle_pid_loop(angular_angle) ;
    chassis_left_3508_id2_stand_current = (int16_t )chassis_gyro_pitch_speed_pid_loop(angular_speed);
    chassis_right_3508_id1_stand_current = (int16_t )-chassis_gyro_pitch_speed_pid_loop(angular_speed);

}


float calculate_lqr_control_loop(float L, struct chassis_lqr_state_input state)
{
    // 1. 根据当前腿长 L 计算 K 矩阵的 12 个动态系数
    // 下面的系数直接从你 MATLAB 截图的输出里复制粘贴
    float k[2][6];

//    k[0][0] = -247.7816f*powf(L,3) + 267.9377f*powf(L,2)-112.8998f*L-1.4886f;
//    k[0][1] = -3.4904f*powf(L,3) + 4.4966f*powf(L,2)-5.5442f*L + 0.1037f;
//    k[0][2] = -20.0942f*powf(L,3) + 18.7760f*powf(L,2)-5.7409f*L-0.9319f;
//    k[0][3] = -24.9415f*powf(L,3) + 23.0400f*powf(L,2)-7.1109f*L-1.4258f;
//    k[0][4] = -136.1708f*powf(L,3) + 160.5996f*powf(L,2)-73.1661f*L + 14.1498f;
//    k[0][5] = -28.3437f*powf(L,3) + 33.9547f*powf(L,2)-15.7881f*L + 3.2395f;
//    k[1][0] = 152.0441f*powf(L,3)-119.0059f*powf(L,2) + 19.5002f*L + 10.2128f;
//    k[1][1] = 11.7341f*powf(L,3)-11.1762f*powf(L,2) + 2.0932f*L + 0.2489f;
//    k[1][2] = -41.9119f*powf(L,3) + 46.5706f*powf(L,2)-19.3386f*L + 2.7677f;
//    k[1][3] = -59.2179f*powf(L,3) + 65.3132f*powf(L,2)-26.9231f*L + 3.6180f;
//    k[1][4] = 368.7994f*powf(L,3)-361.2896f*powf(L,2) + 122.1209f*L + 5.8361f;
//    k[1][5] = 85.7342f*powf(L,3)-84.5140f*powf(L,2) + 28.9233f*L + 1.0225f;

    k[0][0] = -219.3718f*powf(L,3) + 243.0449f*powf(L,2)-132.0522f*L-6.9257f;
    k[0][1] = 1.4498f*powf(L,3)-3.7768f*powf(L,2)-10.7115f*L-0.1304f;
    k[0][2] = 14.2642f*powf(L,3)-13.7595f*powf(L,2) + 4.0356f*L-4.9344f;
    k[0][3] = 26.2735f*powf(L,3)-24.7413f*powf(L,2) + 5.2853f*L-6.7745f;
    k[0][4] = -295.7442f*powf(L,3) + 297.4953f*powf(L,2)-103.0525f*L + 7.7601f;
    k[0][5] = -67.0782f*powf(L,3) + 67.9627f*powf(L,2)-23.8217f*L + 2.2809f;
    k[1][0] = 13.1504f*powf(L,3)-13.1199f*powf(L,2) + 6.8429f*L + 2.3043f;
    k[1][1] = 0.2630f*powf(L,3) + 0.5939f*powf(L,2)-2.1725f*L + 0.0945f;
    k[1][2] = -18.2084f*powf(L,3) + 17.4918f*powf(L,2)-5.1184f*L-0.6229f;
    k[1][3] = -23.7653f*powf(L,3) + 22.9963f*powf(L,2)-6.8760f*L-1.2238f;
    k[1][4] = 10.6520f*powf(L,3)-11.4815f*powf(L,2) + 6.1620f*L + 18.7338f;
    k[1][5] = 4.9454f*powf(L,3)-5.1720f*powf(L,2) + 2.3292f*L + 4.0905f;













    // 2. 计算误差项 (x - x_target)
    // 假设目标：theta=0, d_theta=0, x=target_x, d_x=0, phi=0, d_phi=0
    float e0 = state.virtual_leg_angle_rad - 0.0f;
    float e1 = state.virtual_leg_speed_rad_s - 0.0f;
    float e2 = state.chassis_move_x_m - 0.0f;
    float e3 = state.chassis_speed_m_s - 0.0f;
    float e4 = state.pitch_angle_rad - 0.0f;
    float e5 = state.chassis_pitch_speed_rad_s - 0.0f;

    // 3. 计算输出 u = -K * e
    // 注意：这里是否加负号取决于你 MATLAB 中 K 的计算定义。
    // 如果 MATLAB 里的 K 是由 lqr(A,B,Q,R) 直接生成的，标准控制律是 u = -Kx。
    float wheel_torque = -(k[0][0]*e0 + k[0][1]*e1 + k[0][2]*e2 + k[0][3]*e3 + k[0][4]*e4 + k[0][5]*e5);
//    *out_joint_torque = -(k[1][0]*e0 + k[1][1]*e1 + k[1][2]*e2 + k[1][3]*e3 + k[1][4]*e4 + k[1][5]*e5);

    return wheel_torque;

}












//腿部目标位置计算
//2025.12.29主要工作部分
void chassis_leg_target_position_compute()
{
    //给虚拟腿长和虚拟腿的目标摆角赋值(在区间范围内)
    left_leg_goal_2_joint.virtual_leg_length = MIN_VIRTUAL_LEG_LENGTH ;
    left_leg_goal_2_joint.virtual_leg_angle_047 = M_PI_2 ;

    right_leg_goal_2_joint.virtual_leg_length = MIN_VIRTUAL_LEG_LENGTH ;
    right_leg_goal_2_joint.virtual_leg_angle_047 = M_PI_2 ;







}



//关节电机角度逆解
void chassis_joint_angle_compute_loop()
{
    //已经有了虚拟腿长和虚拟腿的目标摆角

    //利用余弦函数计算04角度
    left_leg_goal_2_joint.angle_04 = calculate_opposite_angle(LEG_SMALL_LENGTH, LEG_BIG_LENGTH, left_leg_goal_2_joint.virtual_leg_length);
    right_leg_goal_2_joint.angle_04 = calculate_opposite_angle(LEG_SMALL_LENGTH, LEG_BIG_LENGTH, right_leg_goal_2_joint.virtual_leg_length);


    if(left_leg_goal_2_joint.angle_04 < 0.0f | right_leg_goal_2_joint.angle_04 < 0.0f)
    {
        //解算失败
        DM8009P_01_RIGHT_BIG_LEG_FRONT.give_angle =   0.15f ;
        DM8009P_02_LEFT_BIG_LEG_FRONT.give_angle = -0.15f ;
        DM8009P_03_LEFT_SMALL_LEG_BEHIND.give_angle = 0.15f ;
        DM8009P_04_RIGHT_SMALL_LEG_BEHIND.give_angle = -0.15f ;
    } else
    {
        //得出07角度(047-04)
        left_leg_goal_2_joint.angle_07 = left_leg_goal_2_joint.virtual_leg_angle_047 - left_leg_goal_2_joint.angle_04 ;
        right_leg_goal_2_joint.angle_07 = right_leg_goal_2_joint.virtual_leg_angle_047 - right_leg_goal_2_joint.angle_04 ;

        //得出0474角度(047+04)
        left_leg_goal_2_joint.angle_0474 = left_leg_goal_2_joint.virtual_leg_angle_047 + left_leg_goal_2_joint.angle_04 ;
        right_leg_goal_2_joint.angle_0474 = right_leg_goal_2_joint.virtual_leg_angle_047 + right_leg_goal_2_joint.angle_04 ;


        //赋值到电机参数结构体中
        DM8009P_02_LEFT_BIG_LEG_FRONT.give_angle = -(float )(M_PI - left_leg_goal_2_joint.angle_0474) ;
        DM8009P_03_LEFT_SMALL_LEG_BEHIND.give_angle = left_leg_goal_2_joint.angle_07 ;

        DM8009P_01_RIGHT_BIG_LEG_FRONT.give_angle = (float )(M_PI - right_leg_goal_2_joint.angle_0474) ;
        DM8009P_04_RIGHT_SMALL_LEG_BEHIND.give_angle = -right_leg_goal_2_joint.angle_07 ;

    }



}

//腿部正解
void chassis_leg_angle_compute_loop()
{
    left_leg_joint_2_leg_parameters.angle_07 = DM8009P_03_LEFT_SMALL_LEG_BEHIND.return_angle ;
    right_leg_joint_2_leg_parameters.angle_07 = -DM8009P_04_RIGHT_SMALL_LEG_BEHIND.return_angle ;

    left_leg_joint_2_leg_parameters.angle_0474 = (float)M_PI + DM8009P_02_LEFT_BIG_LEG_FRONT.return_angle ;
    right_leg_joint_2_leg_parameters.angle_0474 = ((float)M_PI - DM8009P_01_RIGHT_BIG_LEG_FRONT.return_angle) ;

    left_leg_joint_2_leg_parameters.angle_04 = (left_leg_joint_2_leg_parameters.angle_0474 - left_leg_joint_2_leg_parameters.angle_07) / 2.0f ;
    right_leg_joint_2_leg_parameters.angle_04 = (right_leg_joint_2_leg_parameters.angle_0474 - right_leg_joint_2_leg_parameters.angle_07) / 2.0f ;

    left_leg_joint_2_leg_parameters.virtual_leg_angle_047 = left_leg_joint_2_leg_parameters.angle_07 + left_leg_joint_2_leg_parameters.angle_04 ;
    right_leg_joint_2_leg_parameters.virtual_leg_angle_047 = right_leg_joint_2_leg_parameters.angle_07 + right_leg_joint_2_leg_parameters.angle_04 ;

    left_leg_joint_2_leg_parameters.virtual_leg_length = calculate_side_c(LEG_BIG_LENGTH, LEG_SMALL_LENGTH, left_leg_joint_2_leg_parameters.angle_04) ;
    right_leg_joint_2_leg_parameters.virtual_leg_length = calculate_side_c(LEG_BIG_LENGTH, LEG_SMALL_LENGTH, right_leg_joint_2_leg_parameters.angle_04) ;

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
    PID_calc(&chassis_gyro_angle_pid, imu_data_from_external_BMI088_mahony.pitch_degree_angle, chassis_gyro_angle_set_loop);
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
    PID_calc(&chassis_gyro_speed_pid, imu_data_from_external_BMI088_mahony.pitch_radian_vel, chassis_gyro_speed_set_loop);
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


/**
 * 根据已知边a, b和角B求边长c
 * @param a 边长a (单位: m)
 * @param b 边长b (单位: m)
 * @param angleB_rad 角B (弧度制)
 * @return 边长c (单位: m), 如果无法构成三角形则返回-1.0
 */
float calculate_side_c(float a, float b, float angleB_rad)
{
    // 根据余弦定理构造的关于c的一元二次方程判别式:
    // c = a*cosB + sqrt(b^2 - (a*sinB)^2)

    float sinB = sinf(angleB_rad);
    float cosB = cosf(angleB_rad);

    // 计算根号下的部分: b^2 - (a * sinB)^2
    float delta = (b * b) - (a * sinB * a * sinB);

    // 1. 检查是否有解（根号下不能为负）
    if (delta < 0) {
        return -1.0f; // 表示无法构成三角形
    }

    // 2. 计算 c
    // 因为 b > a，所以 a*cosB - sqrt(delta) 必然是负数，直接取加号即可
    float c = (a * cosB) + sqrtf(delta);

    // 3. 检查结果是否合法（边长必须大于0）
    if (c <= 0) {
        return -1.0f;
    }

    return c;
}
