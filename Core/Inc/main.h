/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "remote_control.h"
#include "cmsis_os.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern const RC_ctrl_t *local_rc_ctrl ;
extern int16_t rc_ch0 ;
extern int16_t rc_ch1 ;
extern int16_t rc_ch2 ;
extern int16_t rc_ch3 ;
extern int16_t rc_ch4 ;
extern int16_t rc_s0 ;
extern int16_t rc_s1 ;
extern int16_t key_W ;
extern int16_t key_S ;
extern int16_t key_A ;
extern int16_t key_D ;
extern int16_t key_SHIFT ;
extern int16_t key_CTRL ;
extern int16_t key_Q ;
extern int16_t key_E ;
extern int16_t key_R ;
extern int16_t key_F ;
extern int16_t key_G ;
extern int16_t key_Z ;
extern int16_t key_X ;
extern int16_t key_C ;
extern int16_t key_V ;
extern int16_t key_B ;

extern int16_t mouse_vx ;
extern int16_t mouse_vy ;
extern int16_t mouse_press_l ;
extern int16_t mouse_press_r ;

extern uint32_t rc_receive_time  ;

extern int16_t rc_receive_state ;//遥控器状态 0为离线，1为在线

extern int16_t chassis_right_3508_id1_given_current ;
extern int16_t chassis_left_3508_id2_given_current ;

extern float gyro[3];
extern float acce[3];
extern float temp;
extern float INS_quat[4] ;
extern float INS_angle[3] ;
extern float INS_degree[3] ;

extern float pitch_speed_from_bmi088 ;
extern float yaw_speed_from_bmi088 ;
extern float roll_speed_from_bmi088 ;

extern float pitch_angle_from_bmi088 ;
extern float yaw_angle_from_bmi088 ;
extern float roll_angle_from_bmi088 ;

extern float pitch_radian_from_bmi088 ;
extern float yaw_radian_from_bmi088 ;
extern float roll_radian_from_bmi088 ;


extern uint32_t check_a ;

extern float angular_speed ;//摆角速度
extern float angular_angle ;//摆角目标位置



/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_R_Pin GPIO_PIN_12
#define LED_R_GPIO_Port GPIOH
#define LED_G_Pin GPIO_PIN_11
#define LED_G_GPIO_Port GPIOH
#define LED_B_Pin GPIO_PIN_10
#define LED_B_GPIO_Port GPIOH
#define CS1_ACCEL_Pin GPIO_PIN_4
#define CS1_ACCEL_GPIO_Port GPIOA
#define INT1_ACCEL_Pin GPIO_PIN_4
#define INT1_ACCEL_GPIO_Port GPIOC
#define INT1_GRYO_Pin GPIO_PIN_5
#define INT1_GRYO_GPIO_Port GPIOC
#define CS1_GYRO_Pin GPIO_PIN_0
#define CS1_GYRO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
