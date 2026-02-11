//
// Created by 21481 on 2025/12/29.
//

#ifndef BUBING_RM2025_CHASSIS_VX_H
#define BUBING_RM2025_CHASSIS_VX_H


#define CHASSIS_VX_PID_KP 0.002f
#define CHASSIS_VX_PID_KI 0.002f
#define CHASSIS_VX_PID_KD 0.0f
#define CHASSIS_VX_PID_OUT_MAX 14.0f
#define CHASSIS_VX_PID_KI_MAX 8.0f//±ØÐë¸ø


void chassis_vx_pid_init(void);
float chassis_vx_pid_loop(float chassis_vx_set_loop);


#endif //BUBING_RM2025_CHASSIS_VX_H
