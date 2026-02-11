#include "BMI088Middleware.h"
#include "main.h"

#define BMI088_USING_SPI_UNIT   hspi2

extern SPI_HandleTypeDef BMI088_USING_SPI_UNIT;

#ifndef DWT_LAR_UNLOCK_VALUE
#define DWT_LAR_UNLOCK_VALUE 0xC5ACCE55
#endif


/**
 * @brief  初始化 DWT 计数器
 */
void DWT_Init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->LAR = DWT_LAR_UNLOCK_VALUE; // H7 需要解锁 LAR
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

/**
************************************************************************
* @brief:      	BMI088_GPIO_init(void)
* @param:       void
* @retval:     	void
* @details:    	BMI088传感器GPIO初始化函数
************************************************************************
**/
void BMI088_GPIO_init(void)
{

}
/**
************************************************************************
* @brief:      	BMI088_com_init(void)
* @param:       void
* @retval:     	void
* @details:    	BMI088传感器通信初始化函数
************************************************************************
**/
void BMI088_com_init(void)
{
    DWT_Init();



}
/**
************************************************************************
* @brief:      	BMI088_delay_ms(uint16_t ms)
* @param:       ms - 要延迟的毫秒数
* @retval:     	void
* @details:    	延迟指定毫秒数的函数，基于微秒延迟实现
************************************************************************
**/
void BMI088_delay_ms(uint16_t ms)
{
    while(ms--)
    {
        BMI088_delay_us(1000);
    }
}
///**
//************************************************************************
//* @brief:      	BMI088_delay_us(uint16_t us)
//* @param:       us - 要延迟的微秒数
//* @retval:     	void
//* @details:    	微秒级延迟函数，使用SysTick定时器实现
//************************************************************************
//**/
//void BMI088_delay_us(uint16_t us)
//{
//
//    uint32_t ticks = 0;
//    uint32_t told = 0;
//    uint32_t tnow = 0;
//    uint32_t tcnt = 0;
//    uint32_t reload = 0;
//    reload = SysTick->LOAD;
//    ticks = us * 480;
//    told = SysTick->VAL;
//    while (1)
//    {
//        tnow = SysTick->VAL;
//        if (tnow != told)
//        {
//            if (tnow < told)
//            {
//                tcnt += told - tnow;
//            }
//            else
//            {
//                tcnt += reload - tnow + told;
//            }
//            told = tnow;
//            if (tcnt >= ticks)
//            {
//                break;
//            }
//        }
//    }
//}


/**
************************************************************************
* @brief:      	BMI088_delay_us(uint16_t us)
* @details:    	使用 DWT 实现的高精度微秒延迟，适配 H7 480MHz
************************************************************************
**/
void BMI088_delay_us(uint16_t us)
{
    uint32_t startTick = DWT->CYCCNT;
    // 计算需要的 CPU 周期数：us * (SystemCoreClock / 1000000)
    // 对于 H723 (480MHz)，1us = 480 周期
    uint32_t delayTicks = us * (SystemCoreClock / 1000000);

    while ((DWT->CYCCNT - startTick) < delayTicks);
}


/**
************************************************************************
* @brief:      	BMI088_ACCEL_NS_L(void)
* @param:       void
* @retval:     	void
* @details:    	将BMI088加速度计片选信号置低，使其处于选中状态
************************************************************************
**/
void BMI088_ACCEL_NS_L(void)
{
    HAL_GPIO_WritePin(ACC_CS_GPIO_Port, ACC_CS_Pin, GPIO_PIN_RESET);
}
/**
************************************************************************
* @brief:      	BMI088_ACCEL_NS_H(void)
* @param:       void
* @retval:     	void
* @details:    	将BMI088加速度计片选信号置高，使其处于非选中状态
************************************************************************
**/
void BMI088_ACCEL_NS_H(void)
{
    HAL_GPIO_WritePin(ACC_CS_GPIO_Port, ACC_CS_Pin, GPIO_PIN_SET);
}
/**
************************************************************************
* @brief:      	BMI088_GYRO_NS_L(void)
* @param:       void
* @retval:     	void
* @details:    	将BMI088陀螺仪片选信号置低，使其处于选中状态
************************************************************************
**/
void BMI088_GYRO_NS_L(void)
{
    HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_RESET);
}
/**
************************************************************************
* @brief:      	BMI088_GYRO_NS_H(void)
* @param:       void
* @retval:     	void
* @details:    	将BMI088陀螺仪片选信号置高，使其处于非选中状态
************************************************************************
**/
void BMI088_GYRO_NS_H(void)
{
    HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_SET);
}
/**
************************************************************************
* @brief:      	BMI088_read_write_byte(uint8_t txdata)
* @param:       txdata - 要发送的数据
* @retval:     	uint8_t - 接收到的数据
* @details:    	通过BMI088使用的SPI总线进行单字节的读写操作
************************************************************************
**/
uint8_t BMI088_read_write_byte(uint8_t txdata)
{
    uint8_t rx_data;
    HAL_SPI_TransmitReceive(&BMI088_USING_SPI_UNIT, &txdata, &rx_data, 1, 1000);
    return rx_data;
}

