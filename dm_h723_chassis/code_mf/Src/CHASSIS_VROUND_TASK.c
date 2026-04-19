//
// Created by 21481 on 2026/3/26.
//
#include "CHASSIS_VROUND_TASK.h"
#include "main.h"
#include "cmsis_os.h"
#include "GET_RC_TASK.h"
#include "remote_control.h"


void CHASSIS_VROUND_TASK()
{
    while (1)
    {
        if(rcData.key.v & KEY_PRESSED_OFFSET_Q)
        {
            if(CHASSIS_ROUND_STATE_COMPUTE == CHASSIS_NO_TURN_ROUND)
            {
                CHASSIS_ROUND_STATE_COMPUTE = CHASSIS_TURN_ROUND;
            }
            else
            {
                CHASSIS_ROUND_STATE_COMPUTE = CHASSIS_NO_TURN_ROUND;
            }
            osDelay(200);
        }


        osDelay(1);
    }
}