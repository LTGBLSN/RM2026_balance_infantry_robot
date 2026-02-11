//
// Created by 21481 on 2026/1/29.
//


#include "main.h"
#include "ws2812.h"
#include "cmsis_os.h"

void LED_TASK()
{
    while (1)
    {
        WS2812_Ctrl(255, 0, 0);
        osDelay(1000);
        WS2812_Ctrl(0, 255, 0);
        osDelay(1000);
        WS2812_Ctrl(0, 0, 255);
        osDelay(1000);
    }
}
