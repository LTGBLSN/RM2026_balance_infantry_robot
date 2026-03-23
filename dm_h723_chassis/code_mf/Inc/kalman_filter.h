//
// Created by 21481 on 2026/3/23.
//

#ifndef DM_H723_LIB_KALMAN_FILTER_H
#define DM_H723_LIB_KALMAN_FILTER_H

/* 定义 KF 结构体 */
typedef struct {
    float x[2];      // 状态量：[0]速度 v, [1]加速度 a
    float P[2][2];   // 协方差矩阵
    float Q_sig;     // 过程噪声 (建议初始值: 10.0f)
    float R_v;       // 速度测量噪声 (建议初始值: 0.1f)
    float R_a;       // 加速度测量噪声 (建议初始值: 0.5f)
    float dt;        // 周期 (0.001f)
} VelocityKF_t;


extern VelocityKF_t chassis_kf_left;
extern VelocityKF_t chassis_kf_right;


void VelocityKF_Init(VelocityKF_t *kf, float dt);
float VelocityKF_Update(VelocityKF_t *kf, float v_meas, float a_meas);


#endif //DM_H723_LIB_KALMAN_FILTER_H
