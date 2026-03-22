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
#include "SHOOT_TASK.h"

uint32_t time_chassis = 0;

struct  chassis_lqr_state_input chassis_LQR_compute_left_finial_state;
struct  chassis_lqr_state_input chassis_LQR_compute_right_finial_state;

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

pid_type_def left_leg_compute ;
pid_type_def right_leg_compute ;

pid_type_def left_virtual_leg_target_angle47_compute ;
pid_type_def right_virtual_leg_target_angle47_compute ;

pid_type_def chassis_yaw_compute;
pid_type_def chassis_two_leg_compute;

pid_type_def gimbal_follow ;



void CHASSIS_TASK()
{
//    chassis_DM8009_01_speed_pid_init();
//    chassis_DM8009_02_speed_pid_init();
//    chassis_DM8009_03_speed_pid_init();
//    chassis_DM8009_04_speed_pid_init();
//
//    chassis_DM8009_01_angle_pid_init();
//    chassis_DM8009_02_angle_pid_init();
//    chassis_DM8009_03_angle_pid_init();
//    chassis_DM8009_04_angle_pid_init();
//
//    chassis_gyro_pitch_speed_pid_init();
//    chassis_gyro_pitch_angle_pid_init();

    left_leg_pid_init();
    right_leg_pid_init();
    left_virtual_leg_target_angle47_pid_init();
    right_virtual_leg_target_angle47_pid_init();
    chassis_yaw_pid_init();
    chassis_two_leg_pid_init();
    gimbal_follow_pid_init();
    left_leg_joint_2_leg_parameters.goal_virtual_leg_length = MIN_VIRTUAL_LEG_LENGTH ;
    right_leg_joint_2_leg_parameters.goal_virtual_leg_length = MIN_VIRTUAL_LEG_LENGTH ;

    while (1)
    {
        chassis_goal_speed_compute();


        //整车状态变量更新
        chassis_all_state_update_loop();



        ///////////////////////////////////////////////////////////////////////轮毂部分控制
        //底盘yaw转向环
//        chassis_follow_gimbal_given_speed = -gimbal_follow_pid_loop(GIMBAL_MID_ANGLE);
//        chassis_yaw_turn_corrent = chassis_yaw_pid_loop(chassis_follow_gimbal_given_speed);
        chassis_yaw_turn_corrent = chassis_yaw_pid_loop(YAW_RC_KP * ((float )rcData.rc.ch[2]/660.0f));

        //轮子lqr计算
        wheel_torque_LQR_compute_loop();


        //轮毂限幅
        wheel_tor_limit(left_wheel_tor_compute, right_wheel_tor_compute);



        /////////////////////////////////////////////////////////////////////////腿部控制

        //目标腿长设定
        // 2026.3.8 roll轴控制在此处控制
        virtual_leg_goal_compute();

        //计算出腿长控制目标虚拟腿推力扭矩
        //2026.2.25此该由腿长pid进行输出
        //2026.2.26待添加前馈以补偿重力
        virtual_leg_give_tor_compute();


        //计算关节47度力矩(此处设置小板凳还是轮腿)
        leg_torque_47_compute();



        //传入虚拟关节目标扭矩和出腿长控制目标虚拟腿推力扭矩，通过雅可比矩阵转换为两个关节扭矩
        joint_vmc_compute(left_leg_joint_2_leg_parameters.virtual_joint_theta47_tor,
                          left_leg_joint_2_leg_parameters.virtual_leg_give_tor,
                          right_leg_joint_2_leg_parameters.virtual_joint_theta47_tor,
                          right_leg_joint_2_leg_parameters.virtual_leg_give_tor);

        //转换为达妙控制区间
        joint_tor_Limit(
                right_leg_joint_2_leg_parameters.tor2,
                left_leg_joint_2_leg_parameters.tor2,
                left_leg_joint_2_leg_parameters.tor7,
                right_leg_joint_2_leg_parameters.tor7);

        fly_state_compute();









        osDelay(CONTROL_LOOP_PERIOD_MS);
    }
}

void chassis_goal_speed_compute()
{
    chassis_vx_given_speed = CHASSIS_VX_MAX_SPEED * ((float )rcData.rc.ch[1]/660.0f) ;
}

void chassis_all_state_update_loop()
{
    //车身倾角计算，注意正方向！
    //imu优先算好，后续要用
    chassis_LQR_compute_left_finial_state.chassis_pitch_speed_rad_s = -imu_data_from_external_BMI088_mahony.pitch_radian_vel;
    chassis_LQR_compute_right_finial_state.chassis_pitch_speed_rad_s = -imu_data_from_external_BMI088_mahony.pitch_radian_vel;

    chassis_LQR_compute_left_finial_state.pitch_angle_rad = -imu_data_from_external_BMI088_mahony.pitch_radian_angle;
    chassis_LQR_compute_right_finial_state.pitch_angle_rad = -imu_data_from_external_BMI088_mahony.pitch_radian_angle;



    //关节电机正解，得到相对车身的角度和摆杆长度
    chassis_leg_angle_compute_loop();
    //计算结果更新在如下变量里面

    //left_leg_joint_2_leg_parameters.virtual_leg_length
    //left_leg_joint_2_leg_parameters.virtual_leg_angle_047
    //right_leg_joint_2_leg_parameters.virtual_leg_length
    //right_leg_joint_2_leg_parameters.virtual_leg_angle_047
    //left_leg_joint_2_leg_parameters.angle_04
    //right_leg_joint_2_leg_parameters.angle_04


    //计算雅可比矩阵
    get_jacobian();
    //计算结果更新在如下变量里面
//    left_leg_joint_2_leg_parameters.jacobian[2][2]
//    right_leg_joint_2_leg_parameters.jacobian[2][2]
//    left_leg_joint_2_leg_parameters.jacobian_T[2][2]
//    right_leg_joint_2_leg_parameters.jacobian_T[2][2]

    // 计算腿部运动速度 (正向速度运动学)
    get_leg_velocity() ;
    //计算结果更新在如下变量里面
//    left_leg_joint_2_leg_parameters.L_dt
//    left_leg_joint_2_leg_parameters.theta47_dt
//    right_leg_joint_2_leg_parameters.L_dt
//    right_leg_joint_2_leg_parameters.theta47_dt

    //摆杆角速度赋值
    chassis_LQR_compute_left_finial_state.virtual_leg_speed_rad_s =
            + left_leg_joint_2_leg_parameters.theta47_dt
            - chassis_LQR_compute_left_finial_state.chassis_pitch_speed_rad_s;

    chassis_LQR_compute_right_finial_state.virtual_leg_speed_rad_s =
            + right_leg_joint_2_leg_parameters.theta47_dt
            - chassis_LQR_compute_right_finial_state.chassis_pitch_speed_rad_s;




    //计算摆杆长度（作为整车的k矩阵选取因素）
    // 2026.2.21：不确定直接取两个腿的平均长度是否可以，板凳及平地没有问题，单边桥可能要在这地方做些文章
    chassis_LQR_compute_left_finial_state.finial_lqr_compute_leg_length = left_leg_joint_2_leg_parameters.return_virtual_leg_length ;
    chassis_LQR_compute_right_finial_state.finial_lqr_compute_leg_length = right_leg_joint_2_leg_parameters.return_virtual_leg_length ;



    //摆杆角度 = 90度-车身角度-虚拟腿相对车身角度（因为virtual_leg_angle_047是从水平向下向后旋转到虚拟腿）
    // 2026.2.21：不确定直接取两个腿的平均摆角是否可以，板凳及平地没有问题，单边桥可能要在这地方做些文章
    //2026.2.25我靠原来极性在这错的
    chassis_LQR_compute_left_finial_state.virtual_leg_angle_rad =
            - chassis_LQR_compute_left_finial_state.pitch_angle_rad
            + left_leg_joint_2_leg_parameters.virtual_leg_angle_047
            - (float )M_PI_2;

    chassis_LQR_compute_right_finial_state.virtual_leg_angle_rad =
            - chassis_LQR_compute_right_finial_state.pitch_angle_rad
            + right_leg_joint_2_leg_parameters.virtual_leg_angle_047
            - (float )M_PI_2;



    //整车速度计算2026.2.21:暂时仅使用轮毂速度，可能需要做滤波
    chassis_LQR_compute_left_finial_state.chassis_speed_m_s = chassis_left_vx_compute_loop();
    chassis_LQR_compute_right_finial_state.chassis_speed_m_s = chassis_right_vx_compute_loop();

    //整车位移计算2026.2.21:使用速度积分得到位置，不保证100%可用，待确认
    if((rcData.rc.s[0]) == 1)
    {
        if(chassis_vx_given_speed == 0.0f)
        {
            chassis_LQR_compute_left_finial_state.chassis_move_x_m =
                    chassis_LQR_compute_left_finial_state.chassis_move_x_m
                    + chassis_LQR_compute_left_finial_state.chassis_speed_m_s * CONTROL_LOOP_DT;

            chassis_LQR_compute_right_finial_state.chassis_move_x_m =
                    chassis_LQR_compute_right_finial_state.chassis_move_x_m
                    + chassis_LQR_compute_right_finial_state.chassis_speed_m_s * CONTROL_LOOP_DT;
        }
        else
        {
            chassis_LQR_compute_left_finial_state.chassis_move_x_m = 0.0f ;
            chassis_LQR_compute_right_finial_state.chassis_move_x_m = 0.0f ;
        }

    }
    else
    {
        chassis_LQR_compute_left_finial_state.chassis_move_x_m = 0.0f ;
        chassis_LQR_compute_right_finial_state.chassis_move_x_m = 0.0f ;
    }




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


float chassis_left_vx_compute_loop()
{
    float angular_motor_speed_rad_s = ( ( -(float)motor_can2_data[1].speed_rpm) * 2.0f * (float )M_PI ) / 60.0f ;
    float angular_wheel_speed_rad_s = angular_motor_speed_rad_s / REDUCTION_RATIO + chassis_LQR_compute_left_finial_state.virtual_leg_speed_rad_s;

    float angular_wheel_linear_velocity = angular_wheel_speed_rad_s * WHEEL_RADIUS;



    float left_finial_speed_m_s =
            + angular_wheel_linear_velocity
            + left_leg_joint_2_leg_parameters.return_virtual_leg_length * chassis_LQR_compute_left_finial_state.virtual_leg_speed_rad_s * cosf(chassis_LQR_compute_left_finial_state.virtual_leg_angle_rad)
            + left_leg_joint_2_leg_parameters.L_dt * sinf(chassis_LQR_compute_left_finial_state.virtual_leg_angle_rad);

    return left_finial_speed_m_s;
}



float chassis_right_vx_compute_loop()
{
    float angular_motor_speed_rad_s = ( ((float)motor_can2_data[0].speed_rpm) * 2.0f * (float )M_PI ) / 60.0f ;
    float angular_wheel_speed_rad_s = angular_motor_speed_rad_s / REDUCTION_RATIO + chassis_LQR_compute_right_finial_state.virtual_leg_speed_rad_s;

    float angular_wheel_linear_velocity = angular_wheel_speed_rad_s * WHEEL_RADIUS;



    float right_finial_speed_m_s =
            + angular_wheel_linear_velocity
            + right_leg_joint_2_leg_parameters.return_virtual_leg_length * chassis_LQR_compute_right_finial_state.virtual_leg_speed_rad_s * cosf(chassis_LQR_compute_right_finial_state.virtual_leg_angle_rad)
            + right_leg_joint_2_leg_parameters.L_dt * sinf(chassis_LQR_compute_right_finial_state.virtual_leg_angle_rad);

    return right_finial_speed_m_s;
}





void wheel_torque_LQR_compute_loop()
{
    if((rcData.rc.s[0]) == 1)
    {
        if(left_leg_joint_2_leg_parameters.fly_state == FLY_YES || rcData.rc.ch[4] > 200 || rcData.rc.ch[4] < -200)//这里写的上台阶
        {
            left_wheel_tor_compute = 0.0f ;
            chassis_LQR_compute_left_finial_state.chassis_move_x_m = 0.0f ;

        }
        else
        {
            left_wheel_tor_compute =
                    -(MATLAB_CHASSIS * MOTOR_GIVE_TORQUE_KP * wheel_calculate_lqr_control_loop(chassis_LQR_compute_left_finial_state)) + chassis_yaw_turn_corrent;
        }

        if(right_leg_joint_2_leg_parameters.fly_state == FLY_YES || rcData.rc.ch[4] > 200 || rcData.rc.ch[4] < -200)//这里写的上台阶
        {
            right_wheel_tor_compute = 0.0f ;
            chassis_LQR_compute_right_finial_state.chassis_move_x_m = 0.0f ;

        }
        else
        {
            right_wheel_tor_compute =
                    (MATLAB_CHASSIS * MOTOR_GIVE_TORQUE_KP * wheel_calculate_lqr_control_loop(chassis_LQR_compute_right_finial_state)) + chassis_yaw_turn_corrent;
        }
    }

    else
    {
        left_wheel_tor_compute = 0.0f ;
        right_wheel_tor_compute = 0.0f ;
    }

}


void chassis_yaw_pid_init(void)
{
    static fp32 chassis_yaw_kpkikd[3] = {CHASSIS_YAW_PID_KP, CHASSIS_YAW_PID_KI, CHASSIS_YAW_PID_KD};
    PID_init(&chassis_yaw_compute, PID_POSITION, chassis_yaw_kpkikd, CHASSIS_YAW_PID_MAX, CHASSIS_YAW_PID_KI_MAX);

}

float chassis_yaw_pid_loop(float chassis_yaw_set_loop)
{
    PID_calc(&chassis_yaw_compute, DM_imu.speed_gyro[YAW], chassis_yaw_set_loop);
    float chassis_yaw_given_tor = (float )(chassis_yaw_compute.out);
    return chassis_yaw_given_tor ;

}


void gimbal_follow_pid_init(void)
{
    static fp32 gimbal_follow_kpkikd[3] = {GIMBAL_FOLLOW_PID_KP, GIMBAL_FOLLOW_PID_KI, GIMBAL_FOLLOW_PID_KD};
    PID_init(&gimbal_follow, PID_POSITION, gimbal_follow_kpkikd, GIMBAL_FOLLOW_PID_MAX, GIMBAL_FOLLOW_PID_KI_MAX);

}

float gimbal_follow_pid_loop(float gimbal_follow_set_loop)
{
    PID_calc(&gimbal_follow, motor_can2_data[4].ecd, gimbal_follow_set_loop);
    float gimbal_follow_given_speed = (float )(gimbal_follow.out);
    return gimbal_follow_given_speed ;

}


void chassis_two_leg_pid_init(void)
{
    static fp32 chassis_two_leg_kpkikd[3] = {CHASSIS_TWO_LEG_PID_KP, CHASSIS_TWO_LEG_PID_KI, CHASSIS_TWO_LEG_PID_KD};
    PID_init(&chassis_two_leg_compute, PID_POSITION, chassis_two_leg_kpkikd, CHASSIS_TWO_LEG_PID_MAX, CHASSIS_TWO_LEG_PID_KI_MAX);

}

float chassis_two_leg_pid_loop(float chassis_two_leg_set_loop)
{
    PID_calc(&chassis_two_leg_compute,(left_leg_joint_2_leg_parameters.virtual_leg_angle_047 - right_leg_joint_2_leg_parameters.virtual_leg_angle_047) , chassis_two_leg_set_loop);
    float chassis_two_leg_given_tor = (float )(chassis_two_leg_compute.out);
    return chassis_two_leg_given_tor ;

}



void wheel_tor_limit(float left_wheel_tor, float right_wheel_tor)
{
    if(left_wheel_tor < -DJI3508_MAX_CMD)
    {
        chassis_left_3508_id2_given_current = (int16_t)-DJI3508_MAX_CMD ;
    }
    else if(left_wheel_tor > DJI3508_MAX_CMD)
    {
        chassis_left_3508_id2_given_current = (int16_t)DJI3508_MAX_CMD ;
    } else
    {
        chassis_left_3508_id2_given_current = (int16_t)left_wheel_tor ;
    }

    if(right_wheel_tor < -DJI3508_MAX_CMD)
    {
        chassis_right_3508_id1_given_current = (int16_t)-DJI3508_MAX_CMD ;
    }
    else if(right_wheel_tor > DJI3508_MAX_CMD)
    {
        chassis_right_3508_id1_given_current = (int16_t)DJI3508_MAX_CMD ;
    } else
    {
        chassis_right_3508_id1_given_current = (int16_t)right_wheel_tor ;
    }
}



/*
 * 计算雅可比矩阵
 * 公式：
 * J =  [-1/2K  1/2K]
 *      [ 1/2   1/2 ]
 *
 *
 * 先由compute_jacobian_K()函数求出雅可比矩阵第一行所需的K
 * 再由get_jacobian()计算最终雅可比矩阵
 * */
void get_jacobian()
{
    // 计算最终雅可比矩阵

    // 1. 首先计算中间变量 K
    left_leg_joint_2_leg_parameters.K = compute_jacobian_K(left_leg_joint_2_leg_parameters.angle_04, LEG_BIG_LENGTH, LEG_SMALL_LENGTH);
    right_leg_joint_2_leg_parameters.K = compute_jacobian_K(right_leg_joint_2_leg_parameters.angle_04, LEG_BIG_LENGTH, LEG_SMALL_LENGTH);

    // 2. 计算左腿雅可比矩阵 jacobian [行][列]
    left_leg_joint_2_leg_parameters.jacobian[0][0] = -0.5f * left_leg_joint_2_leg_parameters.K;
    left_leg_joint_2_leg_parameters.jacobian[0][1] =  0.5f * left_leg_joint_2_leg_parameters.K;
    left_leg_joint_2_leg_parameters.jacobian[1][0] =  0.5f;
    left_leg_joint_2_leg_parameters.jacobian[1][1] =  0.5f;

    // 3. 计算右腿雅可比矩阵 jacobian [行][列]
    right_leg_joint_2_leg_parameters.jacobian[0][0] = -0.5f * right_leg_joint_2_leg_parameters.K;
    right_leg_joint_2_leg_parameters.jacobian[0][1] =  0.5f * right_leg_joint_2_leg_parameters.K;
    right_leg_joint_2_leg_parameters.jacobian[1][0] =  0.5f;
    right_leg_joint_2_leg_parameters.jacobian[1][1] =  0.5f;

    // 4. 计算左腿雅可比矩阵的转置 jacobian_T
    left_leg_joint_2_leg_parameters.jacobian_T[0][0] = left_leg_joint_2_leg_parameters.jacobian[0][0]; // -0.5K
    left_leg_joint_2_leg_parameters.jacobian_T[0][1] = left_leg_joint_2_leg_parameters.jacobian[1][0]; //  0.5
    left_leg_joint_2_leg_parameters.jacobian_T[1][0] = left_leg_joint_2_leg_parameters.jacobian[0][1]; //  0.5K
    left_leg_joint_2_leg_parameters.jacobian_T[1][1] = left_leg_joint_2_leg_parameters.jacobian[1][1]; //  0.5

    // 5. 计算右腿雅可比矩阵的转置 jacobian_T
    right_leg_joint_2_leg_parameters.jacobian_T[0][0] = right_leg_joint_2_leg_parameters.jacobian[0][0];
    right_leg_joint_2_leg_parameters.jacobian_T[0][1] = right_leg_joint_2_leg_parameters.jacobian[1][0];
    right_leg_joint_2_leg_parameters.jacobian_T[1][0] = right_leg_joint_2_leg_parameters.jacobian[0][1];
    right_leg_joint_2_leg_parameters.jacobian_T[1][1] = right_leg_joint_2_leg_parameters.jacobian[1][1];

}


/*
 * 计算中间变量K
 * */
float compute_jacobian_K(float theta4, float L1, float L2)
{
    //计算雅可比矩阵里面的K,参考jacobian_readme

    // 预计算三角函数，减少重复调用
    float s4 = sinf(theta4);
    float c4 = cosf(theta4);

    // 平方直接相乘，比 powf 快
    float L1_sq = L1 * L1;
    float s4_sq = s4 * s4;

    // 计算分母根号部分
    float sqrt_part = sqrtf(L2 * L2 - L1_sq * s4_sq);

    // 检查分母是否过小（防止除零错误，在奇异位形附近很危险）
    if (sqrt_part < 1e-6f) {
        return 0.0f; // 或者根据逻辑处理奇异点
    }

    // 最终计算 K
    float K = -L1 * s4 - (L1_sq * s4 * c4) / sqrt_part;

    return K;
}


// 计算腿部运动速度 (正向速度运动学)
// 传入参数：w7 和 w2 分别为两个电机的当前实时角速度 (rad/s)

void get_leg_velocity()
{
    // 1. 计算左腿末端速度
    // L_dt = J[0][0]*∠7速度 + J[0][1]*∠2速度
    left_leg_joint_2_leg_parameters.L_dt = left_leg_joint_2_leg_parameters.jacobian[0][0] * (1.0f) * DM8009P_03_LEFT_SMALL_LEG_BEHIND.return_speed +
                                           left_leg_joint_2_leg_parameters.jacobian[0][1] * (1.0f) * DM8009P_02_LEFT_BIG_LEG_FRONT.return_speed;

    // theta47_dt = J[1][0]*w7 + J[1][1]*w2
    left_leg_joint_2_leg_parameters.theta47_dt = left_leg_joint_2_leg_parameters.jacobian[1][0] * (1.0f) * DM8009P_03_LEFT_SMALL_LEG_BEHIND.return_speed +
                                                 left_leg_joint_2_leg_parameters.jacobian[1][1] * (1.0f) * DM8009P_02_LEFT_BIG_LEG_FRONT.return_speed;

    // 2. 计算右腿末端速度
    // L_dt = J[0][0]*∠7速度 + J[0][1]*∠2速度
    right_leg_joint_2_leg_parameters.L_dt = right_leg_joint_2_leg_parameters.jacobian[0][0] * (-1.0f) * DM8009P_04_RIGHT_SMALL_LEG_BEHIND.return_speed +
                                            right_leg_joint_2_leg_parameters.jacobian[0][1] * (-1.0f) * DM8009P_01_RIGHT_BIG_LEG_FRONT.return_speed;

    right_leg_joint_2_leg_parameters.theta47_dt = right_leg_joint_2_leg_parameters.jacobian[1][0] * (-1.0f) * DM8009P_04_RIGHT_SMALL_LEG_BEHIND.return_speed +
                                                  right_leg_joint_2_leg_parameters.jacobian[1][1] * (-1.0f) * DM8009P_01_RIGHT_BIG_LEG_FRONT.return_speed;
}

void virtual_leg_goal_compute()
{
    if(rcData.rc.ch[4] > 200)
    {
        if( left_leg_joint_2_leg_parameters.virtual_leg_angle_047  >  (M_PI - (M_PI_2/2.0f) ) )
        {
            left_leg_joint_2_leg_parameters.goal_virtual_leg_length = MIN_VIRTUAL_LEG_LENGTH ;
        }
        else
        {
            left_leg_joint_2_leg_parameters.goal_virtual_leg_length = MAX_VIRTUAL_LEG_LENGTH ;
        }

        if( right_leg_joint_2_leg_parameters.virtual_leg_angle_047  >  (M_PI - (M_PI_2/2.0f) ) )
        {
            right_leg_joint_2_leg_parameters.goal_virtual_leg_length = MIN_VIRTUAL_LEG_LENGTH;
        }
        else
        {
            right_leg_joint_2_leg_parameters.goal_virtual_leg_length = MAX_VIRTUAL_LEG_LENGTH ;
        }


    }
    else if(rcData.rc.ch[4] < -200)
    {
//        left_leg_joint_2_leg_parameters.goal_virtual_leg_length = left_leg_joint_2_leg_parameters.goal_virtual_leg_length + (float )rcData.rc.ch[0] * LEG_GOAL_RC_CONTROL_KP;
//        right_leg_joint_2_leg_parameters.goal_virtual_leg_length = right_leg_joint_2_leg_parameters.goal_virtual_leg_length + (float )rcData.rc.ch[0] * LEG_GOAL_RC_CONTROL_KP;
        left_leg_joint_2_leg_parameters.goal_virtual_leg_length = MIN_VIRTUAL_LEG_LENGTH ;
        right_leg_joint_2_leg_parameters.goal_virtual_leg_length = MIN_VIRTUAL_LEG_LENGTH;
    }
    else
    {
        if(rcData.rc.s[1] == 1)
        {
            left_leg_joint_2_leg_parameters.goal_virtual_leg_length = MIN_VIRTUAL_LEG_LENGTH ;
            right_leg_joint_2_leg_parameters.goal_virtual_leg_length = MIN_VIRTUAL_LEG_LENGTH ;
        }
        left_leg_joint_2_leg_parameters.goal_virtual_leg_length = left_leg_joint_2_leg_parameters.goal_virtual_leg_length + (float )rcData.rc.ch[0] * LEG_GOAL_RC_CONTROL_KP;
        right_leg_joint_2_leg_parameters.goal_virtual_leg_length = right_leg_joint_2_leg_parameters.goal_virtual_leg_length + (float )rcData.rc.ch[0] * LEG_GOAL_RC_CONTROL_KP;
    }


    if(left_leg_joint_2_leg_parameters.goal_virtual_leg_length > MAX_VIRTUAL_LEG_LENGTH)
    {
        left_leg_joint_2_leg_parameters.goal_virtual_leg_length = MAX_VIRTUAL_LEG_LENGTH;
    }
    else if(left_leg_joint_2_leg_parameters.goal_virtual_leg_length < MIN_VIRTUAL_LEG_LENGTH)
    {
        left_leg_joint_2_leg_parameters.goal_virtual_leg_length = MIN_VIRTUAL_LEG_LENGTH;
    }

    if(right_leg_joint_2_leg_parameters.goal_virtual_leg_length > MAX_VIRTUAL_LEG_LENGTH)
    {
        right_leg_joint_2_leg_parameters.goal_virtual_leg_length = MAX_VIRTUAL_LEG_LENGTH;
    }
    else if(right_leg_joint_2_leg_parameters.goal_virtual_leg_length < MIN_VIRTUAL_LEG_LENGTH)
    {
        right_leg_joint_2_leg_parameters.goal_virtual_leg_length = MIN_VIRTUAL_LEG_LENGTH;
    }



}



void virtual_leg_give_tor_compute()
{
    if(rcData.rc.s[1] == 1)
    {
        left_leg_joint_2_leg_parameters.virtual_leg_give_tor = JUMP_TOR ;

        right_leg_joint_2_leg_parameters.virtual_leg_give_tor = JUMP_TOR ;


    }
    else
    {
        //此处由腿长pid进行输出
        left_leg_joint_2_leg_parameters.virtual_leg_give_tor =
                left_leg_pid_loop(left_leg_joint_2_leg_parameters.goal_virtual_leg_length) + 0.0f ;

        right_leg_joint_2_leg_parameters.virtual_leg_give_tor =
                right_leg_pid_loop(right_leg_joint_2_leg_parameters.goal_virtual_leg_length) + 0.0f ;
    }

    if(rcData.rc.s[1] == 2)
    {

    }

}


void leg_torque_47_compute()
{
    if(rcData.rc.s[0] == 1)
    {
        if(rcData.rc.ch[4] > 200)
        {
            leg_torque_47_pid_loop();
        }
        else if(rcData.rc.ch[4] < -200)
        {
            leg_torque_47_pid_loop();
        }
        else
        {
            leg_torque_LQR_compute_loop();
        }
    }
    else
    {
//        leg_torque_LQR_compute_loop();

        leg_torque_47_pid_loop();

    }
}

void leg_torque_47_pid_loop()
{
    float virtual_leg_target_angle47 ;

    if(rcData.rc.ch[4] > 200)
    {
        virtual_leg_target_angle47 = M_PI ;
    }
    else
    {
        virtual_leg_target_angle47 = M_PI_2 ;
    }

    left_leg_joint_2_leg_parameters.virtual_joint_theta47_tor = left_leg_target_angle47_pid_loop(virtual_leg_target_angle47) ;
    right_leg_joint_2_leg_parameters.virtual_joint_theta47_tor = right_leg_target_angle47_pid_loop(virtual_leg_target_angle47) ;
}

void left_virtual_leg_target_angle47_pid_init(void)
{
    static fp32 left_virtual_leg_kpkikd[3] = {LEFT_LEG_TARGET_ANGLE47_PID_KP, LEFT_LEG_TARGET_ANGLE47_PID_KI, LEFT_LEG_TARGET_ANGLE47_PID_KD};
    PID_init(&left_virtual_leg_target_angle47_compute, PID_POSITION, left_virtual_leg_kpkikd, LEG_TARGET_ANGLE47_PID_MAX, LEG_TARGET_ANGLE47_PID_KI_MAX);

}

float left_leg_target_angle47_pid_loop(float left_virtual_leg_target_angle47_set_loop)
{
    PID_calc(&left_virtual_leg_target_angle47_compute, left_leg_joint_2_leg_parameters.virtual_leg_angle_047, left_virtual_leg_target_angle47_set_loop);
    float left_virtual_leg_target47_given_tor = (float )(left_virtual_leg_target_angle47_compute.out);
    return left_virtual_leg_target47_given_tor ;

}

void right_virtual_leg_target_angle47_pid_init(void)
{
    static fp32 right_virtual_leg_kpkikd[3] = {RIGHT_LEG_TARGET_ANGLE47_PID_KP, RIGHT_LEG_TARGET_ANGLE47_PID_KI, RIGHT_LEG_TARGET_ANGLE47_PID_KD};
    PID_init(&right_virtual_leg_target_angle47_compute, PID_POSITION, right_virtual_leg_kpkikd, LEG_TARGET_ANGLE47_PID_MAX, LEG_TARGET_ANGLE47_PID_KI_MAX);

}

float right_leg_target_angle47_pid_loop(float right_virtual_leg_target_angle47_set_loop)
{
    PID_calc(&right_virtual_leg_target_angle47_compute, right_leg_joint_2_leg_parameters.virtual_leg_angle_047, right_virtual_leg_target_angle47_set_loop);
    float right_virtual_leg_target47_given_tor = (float )(right_virtual_leg_target_angle47_compute.out);
    return right_virtual_leg_target47_given_tor ;

}


void leg_torque_LQR_compute_loop()
{
    if((rcData.rc.s[0]) == 1)
    {
        left_leg_joint_2_leg_parameters.virtual_joint_theta47_tor =
                (MATLAB_CHASSIS * leg_calculate_lqr_control_loop(chassis_LQR_compute_left_finial_state)) + chassis_two_leg_pid_loop(0.0f);
//        left_leg_joint_2_leg_parameters.virtual_joint_theta47_tor = 5.0f ;



        right_leg_joint_2_leg_parameters.virtual_joint_theta47_tor =
                (MATLAB_CHASSIS * leg_calculate_lqr_control_loop(chassis_LQR_compute_right_finial_state)) - chassis_two_leg_pid_loop(0.0f) ;
//        right_leg_joint_2_leg_parameters.virtual_joint_theta47_tor = 5.0f ;
    }
    else
    {
        left_leg_joint_2_leg_parameters.virtual_joint_theta47_tor = chassis_two_leg_pid_loop(0.0f) ;
        right_leg_joint_2_leg_parameters.virtual_joint_theta47_tor = - chassis_two_leg_pid_loop(0.0f) ;
    }

}




void joint_vmc_compute(float left_tor_47, float left_virtual_leg_tor , float right_tor_47, float right_virtual_leg_tor)
{
    // 利用雅可比矩阵的转置 J_T 进行力矩分配
    // 公式  [tau7] = J_T * [F_L ]
    //      [tau2]         [Tp47]
    //两行1列矩阵

    // 1. 计算左腿电机目标力矩
    // tau7 = J_T[0][0] * F_L + J_T[0][1] * Tp47
    left_leg_joint_2_leg_parameters.tor7 = left_leg_joint_2_leg_parameters.jacobian_T[0][0] * left_virtual_leg_tor +
                                           left_leg_joint_2_leg_parameters.jacobian_T[0][1] * left_tor_47;

    // tau2 = J_T[1][0] * F_L + J_T[1][1] * Tp47
    left_leg_joint_2_leg_parameters.tor2 = left_leg_joint_2_leg_parameters.jacobian_T[1][0] * left_virtual_leg_tor +
                                           left_leg_joint_2_leg_parameters.jacobian_T[1][1] * left_tor_47;


    // 2. 计算右腿电机目标力矩 (假设左右腿使用相同的 VMC 目标输入)
    right_leg_joint_2_leg_parameters.tor7 = -(right_leg_joint_2_leg_parameters.jacobian_T[0][0] * right_virtual_leg_tor +
                                            right_leg_joint_2_leg_parameters.jacobian_T[0][1] * right_tor_47);

    right_leg_joint_2_leg_parameters.tor2 = -(right_leg_joint_2_leg_parameters.jacobian_T[1][0] * right_virtual_leg_tor +
                                            right_leg_joint_2_leg_parameters.jacobian_T[1][1] * right_tor_47);
}


//限幅
void joint_tor_Limit(float motor1 , float motor2 , float motor3 , float motor4)
{
    //没有对起跳的时候超额定部分进行处理，目前只能在额定20nm内，加个起跳标志位切换一下
    if( motor1 > DM8009_MAX_TOR)
    {
        DM8009P_01_RIGHT_BIG_LEG_FRONT.give_tor = DM8009_MAX_TOR ;
    }
    else if(motor1 < -DM8009_MAX_TOR)
    {
        DM8009P_01_RIGHT_BIG_LEG_FRONT.give_tor = -DM8009_MAX_TOR ;
    }
    else
    {
        DM8009P_01_RIGHT_BIG_LEG_FRONT.give_tor = motor1 ;
    }


    if( motor2 > DM8009_MAX_TOR)
    {
        DM8009P_02_LEFT_BIG_LEG_FRONT.give_tor = DM8009_MAX_TOR ;
    }
    else if(motor2 < -DM8009_MAX_TOR)
    {
        DM8009P_02_LEFT_BIG_LEG_FRONT.give_tor = -DM8009_MAX_TOR ;
    }
    else
    {
        DM8009P_02_LEFT_BIG_LEG_FRONT.give_tor = motor2 ;
    }

    if( motor3 > DM8009_MAX_TOR)
    {
        DM8009P_03_LEFT_SMALL_LEG_BEHIND.give_tor = DM8009_MAX_TOR ;
    }
    else if(motor3 < -DM8009_MAX_TOR)
    {
        DM8009P_03_LEFT_SMALL_LEG_BEHIND.give_tor = -DM8009_MAX_TOR ;
    }
    else
    {
        DM8009P_03_LEFT_SMALL_LEG_BEHIND.give_tor = motor3 ;
    }

    if( motor4 > DM8009_MAX_TOR)
    {
        DM8009P_04_RIGHT_SMALL_LEG_BEHIND.give_tor = DM8009_MAX_TOR ;
    }
    else if(motor4 < -DM8009_MAX_TOR)
    {
        DM8009P_04_RIGHT_SMALL_LEG_BEHIND.give_tor = -DM8009_MAX_TOR ;
    }
    else
    {
        DM8009P_04_RIGHT_SMALL_LEG_BEHIND.give_tor = motor4 ;
    }


}

/*离地检测
 * 计算当前腿支持力，如果小于一定值判断为离地*/
void fly_state_compute()
{
    // --- 1. 左腿支撑力推算 ---
    // 计算雅可比转置矩阵的行列式: det = J_T[0][0]*J_T[1][1] - J_T[0][1]*J_T[1][0]
    float det_L = left_leg_joint_2_leg_parameters.jacobian_T[0][0] * left_leg_joint_2_leg_parameters.jacobian_T[1][1] -
                  left_leg_joint_2_leg_parameters.jacobian_T[0][1] * left_leg_joint_2_leg_parameters.jacobian_T[1][0];

    if (fabsf(det_L) > 1e-6f) // 防止除零
    {
        // 根据矩阵求逆公式，求出 F_L (沿腿长方向的力)
        // F_L = (tau7 * J_T[1][1] - tau2 * J_T[0][1]) / det
        left_leg_joint_2_leg_parameters.support_force = (DM8009P_03_LEFT_SMALL_LEG_BEHIND.return_tor * left_leg_joint_2_leg_parameters.jacobian_T[1][1] -
                                  DM8009P_02_LEFT_BIG_LEG_FRONT.return_tor * left_leg_joint_2_leg_parameters.jacobian_T[0][1]) / det_L;
    }

    // --- 2. 右腿支撑力推算 ---
    float det_R = right_leg_joint_2_leg_parameters.jacobian_T[0][0] * right_leg_joint_2_leg_parameters.jacobian_T[1][1] -
                  right_leg_joint_2_leg_parameters.jacobian_T[0][1] * right_leg_joint_2_leg_parameters.jacobian_T[1][0];

    if (fabsf(det_R) > 1e-6f)
    {
        // 注意：参考你的 joint_vmc_compute，右腿力矩方向是取反的
        // 所以这里反馈值也要带负号参与计算，以保证计算出的支撑力为正
        right_leg_joint_2_leg_parameters.support_force = ((-DM8009P_04_RIGHT_SMALL_LEG_BEHIND.return_tor) * right_leg_joint_2_leg_parameters.jacobian_T[1][1] -
                                   (-DM8009P_01_RIGHT_BIG_LEG_FRONT.return_tor) * right_leg_joint_2_leg_parameters.jacobian_T[0][1]) / det_R;
    }

    // --- 3. 状态判定逻辑 ---
    // 这里调用下一步的判定函数
    if(rcData.rc.s[0] != 2)
    {
        //离地检测标志位改变
        fly_state_logic_judgment();
//        left_leg_joint_2_leg_parameters.fly_state = FLY_NO; // 着地
//        right_leg_joint_2_leg_parameters.fly_state = FLY_NO; // 着地
    }

}


void fly_state_logic_judgment(void)
{
    // 如果支撑力小于阈值，认为离地
    if (left_leg_joint_2_leg_parameters.support_force < GROUND_FORCE_THRESHOLD)
    {
        left_leg_joint_2_leg_parameters.fly_state = FLY_YES; // 离地
    }
    else
    {
        left_leg_joint_2_leg_parameters.fly_state = FLY_NO; // 着地
    }

    if (right_leg_joint_2_leg_parameters.support_force < GROUND_FORCE_THRESHOLD)
    {
        right_leg_joint_2_leg_parameters.fly_state = FLY_YES; // 离地
    }
    else
    {
        right_leg_joint_2_leg_parameters.fly_state = FLY_NO; // 着地
    }
}

void update_LQR_K(float t3 ,float t2 ,float t1 )
{

//    半车模型
//    Q = diag([320, 2500, 1200, 500, 200000, 1])
//    R = [40 0; 0 10]

    k[0][0] = 53.3050f*t3 + 77.4558f*t2-127.7172f*t1 + 3.2080f;
    k[0][1] = 4.9315f*t3 + 28.7556f*t2-40.7482f*t1 + 2.0472f;
    k[0][2] = 9.6749f*t3 + 8.9068f*t2-14.9383f*t1 + 0.2604f;
    k[0][3] = 6.8404f*t3 + 19.1435f*t2-23.5602f*t1 + 0.3346f;
    k[0][4] = -383.3230f*t3 + 476.1177f*t2-240.5658f*t1 + 64.3867f;
    k[0][5] = -28.6370f*t3 + 31.3752f*t2-14.0151f*t1 + 3.9334f;

    k[1][0] = 554.0254f*t3-608.9887f*t2 + 221.2033f*t1-11.3337f;
    k[1][1] = 162.0825f*t3-173.9428f*t2 + 60.9298f*t1-2.0591f;
    k[1][2] = 106.3492f*t3-110.0545f*t2 + 37.0762f*t1-2.3413f;
    k[1][3] = 167.0680f*t3-169.6172f*t2 + 55.7525f*t1-3.2592f;
    k[1][4] = 994.3888f*t3-1045.6830f*t2 + 398.2053f*t1 + 80.0820f;
    k[1][5] = 9.2901f*t3-13.4794f*t2 + 7.7959f*t1 + 5.5141f;


}





float wheel_calculate_lqr_control_loop(struct chassis_lqr_state_input state)
{

    float t3 = state.finial_lqr_compute_leg_length * state.finial_lqr_compute_leg_length * state.finial_lqr_compute_leg_length;
    float t2 = state.finial_lqr_compute_leg_length * state.finial_lqr_compute_leg_length;
    float t1 = state.finial_lqr_compute_leg_length;
    update_LQR_K(t3,t2,t1);


    // 2. 计算误差项 (x - x_target)
    // 假设目标：theta=0, d_theta=0, x=target_x, d_x=0, phi=0, d_phi=0
    e0 = state.virtual_leg_angle_rad - 0.0f;
    e1 = state.virtual_leg_speed_rad_s - 0.0f;
    e2 = state.chassis_move_x_m - (0.7f);
    e3 = state.chassis_speed_m_s - chassis_vx_given_speed;
    e4 = state.pitch_angle_rad - (0.0f);
    e5 = state.chassis_pitch_speed_rad_s - 0.0f;

    // 3. 计算输出 u = -K * e
    // 注意：这里是否加负号取决于你 MATLAB 中 K 的计算定义。
    // 如果 MATLAB 里的 K 是由 lqr(A,B,Q,R) 直接生成的，标准控制律是 u = -Kx。
    float wheel_torque = -(k[0][0]*e0 + k[0][1]*e1 + k[0][2]*e2 + k[0][3]*e3 + k[0][4]*e4 + k[0][5]*e5);
//    wheel_torque = 0.0f ;
//    *out_joint_torque = -(k[1][0]*e0 + k[1][1]*e1 + k[1][2]*e2 + k[1][3]*e3 + k[1][4]*e4 + k[1][5]*e5);

    return wheel_torque;

}



float leg_calculate_lqr_control_loop(struct chassis_lqr_state_input state)
{
    float t3 = state.finial_lqr_compute_leg_length * state.finial_lqr_compute_leg_length * state.finial_lqr_compute_leg_length;
    float t2 = state.finial_lqr_compute_leg_length * state.finial_lqr_compute_leg_length;
    float t1 = state.finial_lqr_compute_leg_length;

    update_LQR_K(t3,t2,t1);


    // 2. 计算误差项 (x - x_target)
    // 假设目标：theta=0, d_theta=0, x=target_x, d_x=0, phi=0, d_phi=0
    e0 = state.virtual_leg_angle_rad - 0.0f;
//    e0 = 0.0f ;
    e1 = state.virtual_leg_speed_rad_s - 0.0f;
//    e1 = 0.0f ;
    e2 = state.chassis_move_x_m - (0.7f);
//    e2 = 0.0f ;
    e3 = state.chassis_speed_m_s - chassis_vx_given_speed;
//    e3 = 0.0f ;
    e4 = (state.pitch_angle_rad - (-0.0f));
//    e4 = 0.0f ;
    e5 = (state.chassis_pitch_speed_rad_s - 0.0f);
//    e5 = 0.0f;
    float joint_torque ;

    // 3. 计算输出 u = -K * e
    // 注意：这里是否加负号取决于你 MATLAB 中 K 的计算定义。
    // 如果 MATLAB 里的 K 是由 lqr(A,B,Q,R) 直接生成的，标准控制律是 u = -Kx。
    if(left_leg_joint_2_leg_parameters.fly_state == FLY_YES || right_leg_joint_2_leg_parameters.fly_state == FLY_YES)
    {
        joint_torque = -(k[1][0]*e0 + k[1][1]*e1 );

    }
    else
    {
        joint_torque = -(k[1][0]*e0 + k[1][1]*e1 + k[1][2]*e2 + k[1][3]*e3 + k[1][4]*e4 + k[1][5]*e5);
    }



    return joint_torque;

}


void left_leg_pid_init(void)
{
    static fp32 left_leg_kpkikd[3] = {LEFT_LEG_PID_KP, LEFT_LEG_PID_KI, LEFT_LEG_PID_KD};
    PID_init(&left_leg_compute, PID_POSITION, left_leg_kpkikd, LEG_PID_OUT_MAX, LEG_PID_KI_MAX);

}

float left_leg_pid_loop(float left_leg_set_loop)
{
    PID_calc(&left_leg_compute, left_leg_joint_2_leg_parameters.return_virtual_leg_length, left_leg_set_loop);
    float left_leg_given_tor = (float )(left_leg_compute.out);
    return left_leg_given_tor ;

}


void right_leg_pid_init(void)
{
    static fp32 right_leg_kpkikd[3] = {RIGHT_LEG_PID_KP, RIGHT_LEG_PID_KI, RIGHT_LEG_PID_KD};
    PID_init(&right_leg_compute, PID_POSITION, right_leg_kpkikd, LEG_PID_OUT_MAX, LEG_PID_KI_MAX);

}

float right_leg_pid_loop(float right_leg_set_loop)
{
    PID_calc(&right_leg_compute, right_leg_joint_2_leg_parameters.return_virtual_leg_length, right_leg_set_loop);
    float right_leg_given_tor = (float )(right_leg_compute.out);
    return right_leg_given_tor ;

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

    left_leg_joint_2_leg_parameters.return_virtual_leg_length = calculate_side_c(LEG_BIG_LENGTH, LEG_SMALL_LENGTH, left_leg_joint_2_leg_parameters.angle_04) ;
    right_leg_joint_2_leg_parameters.return_virtual_leg_length = calculate_side_c(LEG_BIG_LENGTH, LEG_SMALL_LENGTH, right_leg_joint_2_leg_parameters.angle_04) ;

}





//腿部目标位置计算
//2025.12.29主要工作部分
void chassis_leg_target_position_compute()
{
    //给虚拟腿长和虚拟腿的目标摆角赋值(在区间范围内)
    left_leg_goal_2_joint.return_virtual_leg_length = MIN_VIRTUAL_LEG_LENGTH ;
    left_leg_goal_2_joint.virtual_leg_angle_047 = M_PI_2 ;

    right_leg_goal_2_joint.return_virtual_leg_length = MIN_VIRTUAL_LEG_LENGTH ;
    right_leg_goal_2_joint.virtual_leg_angle_047 = M_PI_2 ;







}



//关节电机角度逆解
void chassis_joint_angle_compute_loop()
{
    //已经有了虚拟腿长和虚拟腿的目标摆角

    //利用余弦函数计算04角度
    left_leg_goal_2_joint.angle_04 = calculate_opposite_angle(LEG_SMALL_LENGTH, LEG_BIG_LENGTH, left_leg_goal_2_joint.return_virtual_leg_length);
    right_leg_goal_2_joint.angle_04 = calculate_opposite_angle(LEG_SMALL_LENGTH, LEG_BIG_LENGTH, right_leg_goal_2_joint.return_virtual_leg_length);


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
