//
// Created by 21481 on 2025/3/17.
//

#ifndef BUBING_RM2025_CAN_SENT_H
#define BUBING_RM2025_CAN_SENT_H

#include "main.h"
#include "cmsis_os.h"
#include "can.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

#include <stdio.h>
#include "board_LED.h"
#include "uart_printf.h"
#include "uart_sent.h"
#include "bsp_can.h"
#include "CAN_receive.h"

void DM_ALL_STOP();

#endif //BUBING_RM2025_CAN_SENT_H
