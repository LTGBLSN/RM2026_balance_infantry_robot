//
// Created by 21481 on 2026/1/31.
//

#ifndef DM_H723_LIB_GET_RC_TASK_H
#define DM_H723_LIB_GET_RC_TASK_H

#include "main.h"


extern struct rc_data rcData ;

struct rc_data {

    struct {
        int16_t ch[5];       // 4个摇杆通道 + 1个左侧拨轮
        uint8_t s[2];        // 2个三档开关
        int16_t only_sbus_ch[3];  // 3个SBUS附加通道（通道）
    } rc;

    struct {
        int16_t x;
        int16_t y;
        int16_t z;
        uint8_t press_l;
        uint8_t press_r;
    } mouse;


    struct {
        uint16_t v;          // 键盘按键值
    } key;

};





#endif //DM_H723_LIB_GET_RC_TASK_H







