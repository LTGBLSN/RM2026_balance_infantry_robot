//
// Created by 21481 on 2026/1/29.
//

#ifndef DM_H723_LIB_REMOTE_CONTROL_H
#define DM_H723_LIB_REMOTE_CONTROL_H



#define DBUS 0
#define SBUS 1
#define GIMBAL_BOARD 3

#define REMOTE_TYPE GIMBAL_BOARD//遥控器协议切换，DBUS或SBUS

#define SBUS_CHANNEL_MIN 174.0f
#define SBUS_CHANNEL_MAX 1811.0f
#define SBUS_CHANNEL_MID ((SBUS_CHANNEL_MAX + SBUS_CHANNEL_MIN) / 2)
#define SBUS_CHANNEL_HALF_RANGE ((SBUS_CHANNEL_MAX - SBUS_CHANNEL_MIN) / 2)

#define DBUS_CHANNEL_HALF_RANGE 660.0f



#if REMOTE_TYPE == SBUS

#include "main.h"

#define SBUS_BUFF_SIZE	25

extern uint8_t rx_subs_buff[SBUS_BUFF_SIZE];




typedef struct
{
    uint16_t online;

    struct
    {
        int16_t ch[10];
    } rc;

    struct
    {
        /* STICK VALUE */
        int16_t left_vert;
        int16_t left_hori;
        int16_t right_vert;
        int16_t right_hori;
    } joy;

    struct
    {
        /* VAR VALUE */
        float a;
        float b;
    } var;

    struct
    {
        /* KEY VALUE */
        uint8_t a;
        uint8_t b;
        uint8_t c;
        uint8_t d;
        uint8_t e;
        uint8_t f;
        uint8_t g;
        uint8_t h;
    } key;
} subs_remoter_t;
extern subs_remoter_t sbus_remoter;

void sbus_frame_parse(subs_remoter_t *remoter, uint8_t *buf);




#endif




#if REMOTE_TYPE == DBUS


#include "main.h"

#define DBUS_FRAME_SIZE      18
#define DBUS_BUFF_SIZE       (DBUS_FRAME_SIZE * 2) // 这里的缓存设大一点防止溢出
#define RC_CH_VALUE_OFFSET   1024

typedef struct {
    struct {
        int16_t ch[5];       // 4个摇杆通道 + 1个左侧拨轮
        uint8_t s[2];        // 2个三档开关
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

    uint16_t online;         // 在线状态
} dbus_ctrl_t;

extern dbus_ctrl_t dbus_remoter;
extern uint8_t rx_dbus_buff[DBUS_BUFF_SIZE];

void dbus_frame_parse(dbus_ctrl_t *remoter, uint8_t *buf);

#endif


/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W            ((uint16_t)1 << 0)
#define KEY_PRESSED_OFFSET_S            ((uint16_t)1 << 1)
#define KEY_PRESSED_OFFSET_A            ((uint16_t)1 << 2)
#define KEY_PRESSED_OFFSET_D            ((uint16_t)1 << 3)
#define KEY_PRESSED_OFFSET_SHIFT        ((uint16_t)1 << 4)
#define KEY_PRESSED_OFFSET_CTRL         ((uint16_t)1 << 5)
#define KEY_PRESSED_OFFSET_Q            ((uint16_t)1 << 6)
#define KEY_PRESSED_OFFSET_E            ((uint16_t)1 << 7)
#define KEY_PRESSED_OFFSET_R            ((uint16_t)1 << 8)
#define KEY_PRESSED_OFFSET_F            ((uint16_t)1 << 9)
#define KEY_PRESSED_OFFSET_G            ((uint16_t)1 << 10)
#define KEY_PRESSED_OFFSET_Z            ((uint16_t)1 << 11)
#define KEY_PRESSED_OFFSET_X            ((uint16_t)1 << 12)
#define KEY_PRESSED_OFFSET_C            ((uint16_t)1 << 13)
#define KEY_PRESSED_OFFSET_V            ((uint16_t)1 << 14)
#define KEY_PRESSED_OFFSET_B            ((uint16_t)1 << 15)



#endif //DM_H723_LIB_REMOTE_CONTROL_H
