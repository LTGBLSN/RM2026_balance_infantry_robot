//
// Created by 21481 on 2025/12/22.
//

#include "cmsis_os.h"
#include "main.h"
#include "error_detection.h"

void error_detection()
{
    while (1)
    {

        rc_connection_status();//遥控器离线判断
        osDelay(1);

    }
}


void rc_connection_status()
{
    if(HAL_GetTick() - rc_receive_time > RC_NO_DATA_TIMEOUT)
    {
        rc_receive_state = RC_OFFLINE ;//遥控器离线
    }
    else
    {
        rc_receive_state = RC_ONLINE ;//遥控器在线
    }
}
