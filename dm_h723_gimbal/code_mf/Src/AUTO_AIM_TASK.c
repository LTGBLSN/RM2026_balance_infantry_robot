//
// Created by 21481 on 2026/2/12.
//



#include <string.h>
#include "cmsis_os.h"
#include "AUTO_AIM_TASK.h"
#include "IMU_DATA_GET.h"
#include "usart.h"
#include "uart_printf.h"

void AUTO_AIM_TASK()
{
    while (1)
    {
        //自动瞄准任务
        //自瞄接在uart7上面
//        usart7_printf("hello world \r\n");
        sent_data_update();







        osDelay(5);
    }
}




struct SentPacket auto_aim_tx_packet;//下位机要发出去的
static uint8_t tx_buffer[sizeof(struct SentPacket)];    // 自瞄数据发送缓冲区，大小由结构体决定

struct ReceivePacket auto_aim_rx_packet;//上位机发过来的
uint8_t auto_aim_rx_buffer[RX_BUF_SIZE];




//发送
void sent_data_update()
{


    auto_aim_tx_packet.header = 0xff;//包头
    auto_aim_tx_packet.mod = RED;//敌方颜色
    auto_aim_tx_packet.roll = imu_data_from_board_BMI088_mahony.roll_degree_angle;
    auto_aim_tx_packet.pitch = -imu_data_from_board_BMI088_mahony.pitch_degree_angle;
    auto_aim_tx_packet.yaw = imu_data_from_board_BMI088_mahony.yaw_degree_angle;
    auto_aim_tx_packet.editor = 0x0d;//包尾



    memcpy(tx_buffer, &auto_aim_tx_packet, sizeof(auto_aim_tx_packet)); //拷贝到缓冲区

    HAL_UART_Transmit_IT(&huart7, tx_buffer, sizeof(auto_aim_tx_packet));

}

