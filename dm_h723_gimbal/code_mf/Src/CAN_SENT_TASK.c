//
// Created by 21481 on 2026/1/30.
//



#include "cmsis_os.h"
#include "DJI_motors.h"
#include "remote_control.h"
#include "can_receive.h"
#include "dm_motor.h"
#include "GET_RC_TASK.h"
#include "ERROR_DETECTION.h"

void CAN_SENT_TASK()
{

    while (1)
    {
    if(rcData.rc.s[0] == 2 )
    {
        FDCAN_DJI_motors(0, 0, 0, 0, 0x200, CAN_CHANNEL_1);
        FDCAN_DJI_motors(0, 0, 0, 0, 0x1FF, CAN_CHANNEL_1);
    }
    else if (rcData.rc.s[0] == 3 | rcData.rc.s[0] == 1)
    {
        if(pitch_6020_state == GM6020_SAFE)
        {
            //Ä¦²ÁÂÖ
            FDCAN_DJI_motors(FRICTION_WHEEL_3508_ID1_GIVEN_CURRENT, FRICTION_WHEEL_3508_ID2_GIVEN_CURRENT, 0, 0, 0x200, CAN_CHANNEL_1);
//            //pitchµç»ú
            FDCAN_DJI_motors(0, PITCH_6020_ID2_GIVEN_CURRENT, 0, 0, 0x1FF, CAN_CHANNEL_1);
        } else
        {
            FDCAN_DJI_motors(0, 0, 0, 0, 0x200, CAN_CHANNEL_1);
            FDCAN_DJI_motors(0, 0, 0, 0, 0x1FF, CAN_CHANNEL_1);
        }



    }
    else
    {
        FDCAN_DJI_motors(0, 0, 0, 0, 0x200, CAN_CHANNEL_1);
        FDCAN_DJI_motors(0, 0, 0, 0, 0x1FF, CAN_CHANNEL_1);
    }



        osDelay(1);
    }

}



