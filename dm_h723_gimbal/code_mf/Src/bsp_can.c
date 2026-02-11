//
// Created by 21481 on 2026/1/30.
//

#include "main.h"
#include "fdcan.h"




void BSP_FDCAN_Init(void)
{

    FDCAN_FilterTypeDef FDCAN1_FilterConfig;

    FDCAN1_FilterConfig.IdType = FDCAN_STANDARD_ID;
    FDCAN1_FilterConfig.FilterIndex = 0;
    FDCAN1_FilterConfig.FilterType = FDCAN_FILTER_MASK;
    FDCAN1_FilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    FDCAN1_FilterConfig.FilterID1 = 0x00000000;
    FDCAN1_FilterConfig.FilterID2 = 0x00000000;

    if(HAL_FDCAN_ConfigFilter(&hfdcan1, &FDCAN1_FilterConfig) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
    {
        Error_Handler();
    }


    FDCAN_FilterTypeDef FDCAN2_FilterConfig;

    FDCAN2_FilterConfig.IdType = FDCAN_STANDARD_ID;
    FDCAN2_FilterConfig.FilterIndex = 0;
    FDCAN2_FilterConfig.FilterType = FDCAN_FILTER_MASK;
    FDCAN2_FilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    FDCAN2_FilterConfig.FilterID1 = 0x00000000;
    FDCAN2_FilterConfig.FilterID2 = 0x00000000;

    if(HAL_FDCAN_ConfigFilter(&hfdcan2, &FDCAN2_FilterConfig) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan2, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_FDCAN_Start(&hfdcan2) != HAL_OK)
    {
        Error_Handler();
    }



    FDCAN_FilterTypeDef FDCAN3_FilterConfig;

    FDCAN3_FilterConfig.IdType = FDCAN_STANDARD_ID;
    FDCAN3_FilterConfig.FilterIndex = 0;
    FDCAN3_FilterConfig.FilterType = FDCAN_FILTER_MASK;
    FDCAN3_FilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    FDCAN3_FilterConfig.FilterID1 = 0x00000000;
    FDCAN3_FilterConfig.FilterID2 = 0x00000000;

    if(HAL_FDCAN_ConfigFilter(&hfdcan3, &FDCAN3_FilterConfig) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan3, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_FDCAN_Start(&hfdcan3) != HAL_OK)
    {
        Error_Handler();
    }
}

