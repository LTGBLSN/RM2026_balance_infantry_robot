//
// Created by 21481 on 2026/3/23.
//
#include "kalman_filter.h"



VelocityKF_t chassis_kf_left;
VelocityKF_t chassis_kf_right;

/* 初始化 */
void VelocityKF_Init(VelocityKF_t *kf, float dt)
{
    kf->dt = dt;
    kf->x[0] = 0.0f; kf->x[1] = 0.0f;
    kf->P[0][0] = 1.0f; kf->P[0][1] = 0.0f;
    kf->P[1][0] = 0.0f; kf->P[1][1] = 1.0f;

    kf->Q_sig = 0.1f;   // 调大则跟踪更灵敏，调小则更平滑
    kf->R_v = 0.05f;     // 你之前的运动学解算挺准的，给个较小值
    kf->R_a = 0.6f;      // IMU比较噪，给个较大值
}


float VelocityKF_Update(VelocityKF_t *kf, float v_meas, float a_meas)
{
    float dt = kf->dt;

    // --- 1. 预测步 (Prediction) ---
    // x = F * x
    float v_pre = kf->x[0] + kf->x[1] * dt;
    float a_pre = kf->x[1];

    // P = F*P*F' + Q (手动展开)
    // 这一步增加了 P01 的更新，是跟踪不掉队的关键
    kf->P[0][0] += dt * (kf->P[0][1] + kf->P[1][0] + dt * kf->P[1][1]);
    kf->P[0][1] += dt * kf->P[1][1];
    kf->P[1][0] += dt * kf->P[1][1];
    kf->P[1][1] += kf->Q_sig * dt;

    // --- 2. 测量更新步 (Update) ---
    // 计算卡尔曼增益 K = P / (P + R)
    float K0 = kf->P[0][0] / (kf->P[0][0] + kf->R_v);
    float K1 = kf->P[1][1] / (kf->P[1][1] + kf->R_a);

    // 更新状态量
    kf->x[0] = v_pre + K0 * (v_meas - v_pre);
    kf->x[1] = a_pre + K1 * (a_meas - a_pre);

    // 更新协方差矩阵 P = (I - K) * P
    kf->P[0][0] *= (1.0f - K0);
    kf->P[0][1] *= (1.0f - K0);
    kf->P[1][0] *= (1.0f - K1);
    kf->P[1][1] *= (1.0f - K1);

    return kf->x[0];
}


