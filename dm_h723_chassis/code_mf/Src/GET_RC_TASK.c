//
// Created by 21481 on 2026/1/31.
//



#include "cmsis_os.h"
#include "GET_RC_TASK.h"
#include "remote_control.h"

struct rc_data rcData ;

void GET_RC_TASK()
{
    while (1)
    {
#if REMOTE_TYPE == DBUS
        rcData.rc.ch[0] = dbus_remoter.rc.ch[0];
        rcData.rc.ch[1] = dbus_remoter.rc.ch[1];
        rcData.rc.ch[2] = dbus_remoter.rc.ch[2];
        rcData.rc.ch[3] = dbus_remoter.rc.ch[3];
        rcData.rc.ch[4] = dbus_remoter.rc.ch[4];
        rcData.rc.s[0] = dbus_remoter.rc.s[0];
        rcData.rc.s[1] = dbus_remoter.rc.s[1];
        rcData.mouse.x = dbus_remoter.mouse.x;
        rcData.mouse.y = dbus_remoter.mouse.y;
        rcData.mouse.z = dbus_remoter.mouse.z;
        rcData.mouse.press_l = dbus_remoter.mouse.press_l;
        rcData.mouse.press_r = dbus_remoter.mouse.press_r;
        rcData.key.v = dbus_remoter.key.v;
#endif

#if REMOTE_TYPE == SBUS
        if(sbus_remoter.online == 1)
        {
            rcData.rc.ch[0] = ((float )sbus_remoter.rc.ch[0] - SBUS_CHANNEL_MID) / SBUS_CHANNEL_HALF_RANGE * DBUS_CHANNEL_HALF_RANGE;
            rcData.rc.ch[1] = ((float )sbus_remoter.rc.ch[1] - SBUS_CHANNEL_MID) / SBUS_CHANNEL_HALF_RANGE * DBUS_CHANNEL_HALF_RANGE;
            rcData.rc.ch[2] = ((float )sbus_remoter.rc.ch[2] - SBUS_CHANNEL_MID) / SBUS_CHANNEL_HALF_RANGE * DBUS_CHANNEL_HALF_RANGE;
            rcData.rc.ch[3] = ((float )sbus_remoter.rc.ch[3] - SBUS_CHANNEL_MID) / SBUS_CHANNEL_HALF_RANGE * DBUS_CHANNEL_HALF_RANGE;
            rcData.rc.ch[4] = ((float )sbus_remoter.rc.ch[4] - SBUS_CHANNEL_MID) / SBUS_CHANNEL_HALF_RANGE * DBUS_CHANNEL_HALF_RANGE;

            if(sbus_remoter.rc.ch[6] == 1792)
            {
                rcData.rc.s[0] = 2;
            }
            else if(sbus_remoter.rc.ch[6] == 997)
            {
                rcData.rc.s[0] = 3;
            }
            else if(sbus_remoter.rc.ch[6] == 191)
            {
                rcData.rc.s[0] = 1;
            }
            else
            {
                rcData.rc.s[0] = 0 ;
            }

            if(sbus_remoter.rc.ch[5] == 1792)
            {
                rcData.rc.s[1] = 2;
            }
            else if(sbus_remoter.rc.ch[5] == 997)
            {
                rcData.rc.s[1] = 3;
            }
            else if(sbus_remoter.rc.ch[5] == 191)
            {
                rcData.rc.s[1] = 1;
            }
            else
            {
                rcData.rc.s[1] = 0 ;
            }

        }


#endif



        osDelay(1);
    }
}



