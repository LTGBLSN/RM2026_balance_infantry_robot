
#include <string.h>
#include "referee.h"

Class_Referee Referee;

uint8_t My_Referee_Rx_Buf[MY_REFEREE_BUF_SIZE];

// CRC8校验码
static const uint8_t crc_8_table[256] =
{
		0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83, 0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
		0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e, 0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,
		0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0, 0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
		0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d, 0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
		0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5, 0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
		0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58, 0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
		0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6, 0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24,
		0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b, 0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9,
		0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f, 0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
		0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92, 0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50,
		0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c, 0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
		0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1, 0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
		0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49, 0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
		0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4, 0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16,
		0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a, 0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
		0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7, 0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35,
};


// CRC16校验码
static const uint16_t crc_16_table[256] =
{
		0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
		0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
		0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
		0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
		0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
		0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
		0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
		0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
		0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
		0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
		0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
		0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
		0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
		0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
		0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
		0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
		0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
		0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
		0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
		0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
		0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
		0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
		0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
		0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
		0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
		0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
		0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
		0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
		0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
		0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
		0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
		0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};
//
//
////裁判系统命令码类型
//void Referee_Command_ID_Structure_Init(Referee_Command_ID_Struct *Referee_Command_ID)
//
//{
//    //游戏全局相关
//	Referee_Command_ID->GAME_STATUS 											= 0x0001;		//0x0001 比赛状态数据
//    Referee_Command_ID->GAME_RESULT 											= 0x0002;		//0x0002 比赛结果数据
//    Referee_Command_ID->GAME_ROBOT_HP 											= 0x0003;		//0x0003 机器人血量数据
//
//    //己方场地事件相关
//    Referee_Command_ID->EVENT_SELF_DATA 										= 0x0101;		//0x0101 场地事件数据
//    Referee_Command_ID->EVENT_SELF_SUPPLY 										= 0x0102;		//0x0102 场地补给数据
//    Referee_Command_ID->EVENT_SELF_REFEREE_WARNING 								= 0x0104;		//0x0104 裁判警告数据
//    Referee_Command_ID->EVENT_SELF_DART_STATUS 									= 0x0105;		//0x0105 飞镖发射相关数据
//
//    //机器人自身状态相关
//    Referee_Command_ID->ROBOT_STATUS 											= 0x0201;		//0x0201 机器人性能体系数据
//    Referee_Command_ID->ROBOT_POWER_HEAT 										= 0x0202;		//0x0202 底盘缓冲能量和射击热量数据
//    Referee_Command_ID->ROBOT_POSITION 											= 0x0203;		//0x0203 机器人位置数据
//    Referee_Command_ID->ROBOT_BUFF 												= 0x0204;		//0x0204 机器人增益和底盘能量数据
//    Referee_Command_ID->ROBOT_AERIAL_STATUS 									= 0x0205;		//0x0205 空中机器人专属状态数据
//    Referee_Command_ID->ROBOT_DAMAGE 											= 0x0206;		//0x0206 机器人伤害状态数据
//    Referee_Command_ID->ROBOT_BOOSTER 											= 0x0207;		//0x0207 实时射击数据
//    Referee_Command_ID->ROBOT_REMAINING_AMMO 									= 0x0208;		//0x0208 允许发弹量数据
//    Referee_Command_ID->ROBOT_RFID 												= 0x0209;		//0x0209 RFID模块状态数据
//    Referee_Command_ID->ROBOT_DART_COMMAND 										= 0x020A;		//0x020A 飞镖选手端指令数据
//    Referee_Command_ID->ROBOT_SENTRY_LOCATION 									= 0x020B;		//0x020B 地面机器人位置数据
//    Referee_Command_ID->ROBOT_RADAR_MARK 										= 0x020C;		//0x020C 雷达标记进度数据
//    Referee_Command_ID->ROBOT_SENTRY_DECISION 									= 0x020D;		//0x020D 哨兵自主决策信息同步
//    Referee_Command_ID->ROBOT_RADAR_DECISION 									= 0x020E;		//0x020E 雷达自主决策信息同步
//
//    //交互相关
//    Referee_Command_ID->INTERACTION 											= 0x0301;		//0x0301 机器人交互数据
//    Referee_Command_ID->INTERACTION_ROBOT_RECEIVE_CUSTOM_CONTROLLER 			= 0x0302;		//0x0302 自定义控制器与机器人交互数据
//    Referee_Command_ID->INTERACTION_ROBOT_RECEIVE_CLIENT_MINIMAP 				= 0x0303;		//0x0303 选手端小地图交互数据
//    Referee_Command_ID->INTERACTION_ROBOT_RECEIVE_CLIENT_REMOTE_CONTROL 		= 0x0304;		//0x0304 键鼠遥控数据
//    Referee_Command_ID->INTERACTION_CLIENT_RECEIVE_RADAR 						= 0x0305;		//0x0305 选手端小地图接收雷达数据
//    Referee_Command_ID->INTERACTION_CLIENT_RECEIVE_CUSTOM_CONTROLLER 			= 0x0306;		//0x0306 自定义控制器与选手端交互数据
//    Referee_Command_ID->INTERACTION_CLIENT_RECEIVE_SENTRY_SEMIAUTOMATIC_MINIMAP = 0x0307;		//0x0307 选手端小地图接收路径数据
//    Referee_Command_ID->INTERACTION_CLIENT_RECEIVE_ROBOT_MINIMAP 				= 0x0308;		//0x0308 选手端小地图接收机器人数据
//
//    //图传链路专属命令码
//    Referee_Command_ID->IMAGE_CHANNEL_SET          								= 0x0F01;		//0x0F01 设置图传出图信道
//    Referee_Command_ID->IMAGE_CHANNEL_QUERY        								= 0x0F02;		//0x0F02 查询图传出图信道
//
//	//雷达无线链路命令码
//    Referee_Command_ID->RADAR_ENEMY_POS            								= 0x0A01;		//0x0A01 敌方位置
//    Referee_Command_ID->RADAR_ENEMY_HP             								= 0x0A02;		//0x0A02 敌方血量
//    Referee_Command_ID->RADAR_ENEMY_AMMO           								= 0x0A03;		//0x0A03 敌方发弹量
//    Referee_Command_ID->RADAR_ENEMY_STATE          								= 0x0A04;		//0x0A04 敌方宏观状态
//    Referee_Command_ID->RADAR_ENEMY_BUFF          								= 0x0A05;		//0x0A05 敌方增益
//    Referee_Command_ID->RADAR_ENEMY_KEY            								= 0x0A06;		//0x0A06 敌方干扰波密钥
//};
//
////裁判系统 0x0301子命令码类型
//void Referee_Interaction_Command_ID_Structure_Init(Referee_Interaction_Command_ID_Struct *Referee_Interaction_Command_ID)
//{
//    //选手端UI交互子命令码
//	Referee_Interaction_Command_ID->UI_LAYER_DELETE 	= 0x0100;		//0x0100 选手端删除图层
//	Referee_Interaction_Command_ID->UI_GRAPHIC_1 		= 0x0101;		//0x0101 选手端绘制1个图形
//	Referee_Interaction_Command_ID->UI_GRAPHIC_2 		= 0x0102;		//0x0102 选手端绘制2个图形
//	Referee_Interaction_Command_ID->UI_GRAPHIC_5 		= 0x0103;		//0x0103 选手端绘制5个图形
//	Referee_Interaction_Command_ID->UI_GRAPHIC_7 		= 0x0104;		//0x0104 选手端绘制7个图形
//	Referee_Interaction_Command_ID->UI_GRAPHIC_STRING 	= 0x0110;		//0x0110 选手端绘制字符图形
//
//    //哨兵/雷达自主决策子命令码
//	Referee_Interaction_Command_ID->SENTRY 				= 0x0120;		//0x0120 哨兵自主决策指令,复活/兑换弹药/远程补血等
//	Referee_Interaction_Command_ID->RADAR 				= 0x0121;		//0x0121 雷达自主决策指令,触发双倍易伤/更新密钥/验证密钥
//};
//
////通用单方机器人ID无阵营区分
//void Referee_Data_Robot_ID_Structure_Init(Referee_Data_Robot_ID_Struct *Referee_Data_Robot_ID)
//{
//	Referee_Data_Robot_ID->NULL_ID 			= 0;		//无效ID
//	Referee_Data_Robot_ID->HERO_1 			= 1;		//1号英雄机器人
//	Referee_Data_Robot_ID->ENGINEER_2 		= 2;		//2号工程机器人
//	Referee_Data_Robot_ID->INFANTRY_3 		= 3;		//3号步兵机器人
//	Referee_Data_Robot_ID->INFANTRY_4 		= 4;		//4号步兵机器人
//	Referee_Data_Robot_ID->INFANTRY_5 		= 5;		//5号步兵机器人
//	Referee_Data_Robot_ID->AERIAL_6 		= 6;		//空中机器人
//	Referee_Data_Robot_ID->SENTRY_7 		= 7;		//哨兵机器人
//	Referee_Data_Robot_ID->DART_8 			= 8;		//飞镖机器人
//	Referee_Data_Robot_ID->RADAR_9 			= 9;		//雷达机器人
//	Referee_Data_Robot_ID->BASE_10 			= 10;		//基地
//	Referee_Data_Robot_ID->OUTPOST_11 		= 11;		//前哨站
//}
////通用双方机器人ID结构体区分红蓝阵营
//void Referee_Data_Robots_ID_Structure_Init(Referee_Data_Robots_ID_Struct *Referee_Data_Robots_ID)
//{
//	Referee_Data_Robots_ID->NULL_ID        		= 0;			//无效ID
//
//	//红方机器人ID
//	Referee_Data_Robots_ID->RED_HERO_1			= 1;			//1 红方1号英雄机器人
//	Referee_Data_Robots_ID->RED_ENGINEER_2  	= 2;     		//2 红方2号工程机器人
//	Referee_Data_Robots_ID->RED_INFANTRY_3  	= 3;     		//3 红方3号步兵机器人
//	Referee_Data_Robots_ID->RED_INFANTRY_4  	= 4;     		//4 红方4号步兵机器人
//	Referee_Data_Robots_ID->RED_INFANTRY_5  	= 5;     		//5 红方5号步兵机器人
//	Referee_Data_Robots_ID->RED_AERIAL_6  		= 6;       		//6 红方6号空中机器人
//	Referee_Data_Robots_ID->RED_SENTRY_7  		= 7;       		//7 红方7号哨兵机器人
//	Referee_Data_Robots_ID->RED_DART_8  		= 8;         	//8 红方8号飞镖机器人
//	Referee_Data_Robots_ID->RED_RADAR_9  		= 9;        	//9 红方9号雷达机器人
//	Referee_Data_Robots_ID->RED_BASE_10  		= 10;        	//10 红方基地
//	Referee_Data_Robots_ID->RED_OUTPOST_11  	= 11;     		//11 红方前哨站
//
//	//蓝方机器人ID
//	Referee_Data_Robots_ID->BLUE_HERO_1  		= 101;        	//101 蓝方1号英雄机器人
//	Referee_Data_Robots_ID->BLUE_ENGINEER_2  	= 102;    		//102 蓝方2号工程机器人
//	Referee_Data_Robots_ID->BLUE_INFANTRY_3  	= 103;    		//103 蓝方3号步兵机器人
//	Referee_Data_Robots_ID->BLUE_INFANTRY_4  	= 104;    		//104 蓝方4号步兵机器人
//	Referee_Data_Robots_ID->BLUE_INFANTRY_5  	= 105;    		//105 蓝方5号步兵机器人
//	Referee_Data_Robots_ID->BLUE_AERIAL_6  		= 106;      	//106 蓝方6号空中机器人
//	Referee_Data_Robots_ID->BLUE_SENTRY_7  		= 107;      	//107 蓝方7号哨兵机器人
//	Referee_Data_Robots_ID->BLUE_DART_8			= 108;        	//108 蓝方8号飞镖机器人
//	Referee_Data_Robots_ID->BLUE_RADAR_9		= 109;       	//109 蓝方9号雷达机器人
//	Referee_Data_Robots_ID->BLUE_BASE_10		= 110;       	//110 蓝方基地
//	Referee_Data_Robots_ID->BLUE_OUTPOST_11		= 111;    		//111 蓝方前哨站
//}
////通用双方机器人选手端ID结构体区分红蓝阵营
//void Referee_Data_Robots_Client_ID_Structure_Init(Referee_Data_Robots_Client_ID_Struct *Referee_Data_Robots_Client_ID)
//{
//	Referee_Data_Robots_Client_ID->NULL_ID      	= 0;		   //无效ID
//
//	//红方选手端ID
//	Referee_Data_Robots_Client_ID->RED_HERO_1		= 0x0101;      //0x0101 红方1号英雄机器人选手端
//	Referee_Data_Robots_Client_ID->RED_ENGINEER_2	= 0x0102;      //0x0102 红方2号工程机器人选手端
//	Referee_Data_Robots_Client_ID->RED_INFANTRY_3	= 0x0103;      //0x0103 红方3号步兵机器人选手端
//	Referee_Data_Robots_Client_ID->RED_INFANTRY_4	= 0x0104;      //0x0104 红方4号步兵机器人选手端
//	Referee_Data_Robots_Client_ID->RED_INFANTRY_5	= 0x0105;      //0x0105 红方5号步兵机器人选手端
//	Referee_Data_Robots_Client_ID->RED_AERIAL_6		= 0x0106;      //0x0106 红方6号空中机器人选手端
//
//	//蓝方选手端ID
//	Referee_Data_Robots_Client_ID->BLUE_HERO_1		= 0x0165;      //0x0165 蓝方1号英雄机器人选手端
//	Referee_Data_Robots_Client_ID->BLUE_ENGINEER_2	= 0x0166;      //0x0166 蓝方2号工程机器人选手端
//	Referee_Data_Robots_Client_ID->BLUE_INFANTRY_3	= 0x0167;      //0x0167 蓝方3号步兵机器人选手端
//	Referee_Data_Robots_Client_ID->BLUE_INFANTRY_4	= 0x0168;      //0x0168 蓝方4号步兵机器人选手端
//	Referee_Data_Robots_Client_ID->BLUE_INFANTRY_5	= 0x0169;      //0x0169 蓝方5号步兵机器人选手端
//	Referee_Data_Robots_Client_ID->BLUE_AERIAL_6	= 0x016A;      //0x016A 蓝方6号空中机器人选手端
//
//	//特殊ID
//	Referee_Data_Robots_Client_ID->SERVER			= 0x8080;      //0x8080 裁判系统服务器ID,用于哨兵/雷达自主决策指令
//}




//void Referee_UART_Data_Structure_Init(void)
//{
//    Struct_Referee_UART_Data referee_data;
//
//    referee_data.Frame_Header = 0xA5;
//
//    referee_data.Data_Length
//    referee_data.Sequence = 0x01;   // 示例：第1包数据
//    referee_data.Referee_Command_ID = REF_CMD_0x0003;  // 示例：选择“机器人血量数据”命令码
//    // CRC_8和CRC_16需按协议附录一的校验算法计算后赋值
//}








/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     裁判系统结构体初始化
// 参数说明     referee				裁判系统结构体
// 返回参数     null
// 使用示例     Referee_Structure_Init(&Referee）;
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
static void Referee_Structure_Init(Class_Referee *referee)
{
	referee->Frame_Header = 0;					//数据包头标

    referee->Flag = 0;							//当前时刻的裁判系统接收flag
    referee->Pre_Flag = 0; 						//前一时刻的裁判系统接收flag

    referee->Sequence = 0;						//发送序列号

    memset(referee->UI_Change_Flag, 0, sizeof(referee->UI_Change_Flag));	//UI是否是初次绘制, 没绘制过是0

    referee->Referee_Status = Referee_Status_DISABLE;			//裁判系统状态

    referee->Referee_Trust_Status = Referee_Data_Status_ENABLE;  //裁判系统是否可信
}





void Referee_Init(UART_HandleTypeDef *huart, uint8_t __Frame_Header)
{
    // 1. 初始化结构体基础变量
    Referee_Structure_Init(&Referee);

    // 2. 直接绑定 HAL 句柄和你的外部缓冲区
    Referee.huart = huart;
    Referee.p_Rx_Buffer = My_Referee_Rx_Buf; // 传入你定义的缓冲区地址
    Referee.Rx_Buffer_Length = MY_REFEREE_BUF_SIZE;
    Referee.Frame_Header = __Frame_Header;

    // 3. 直接调用 HAL 库函数开启 DMA 接收
    HAL_UARTEx_ReceiveToIdle_DMA(Referee.huart, Referee.p_Rx_Buffer, Referee.Rx_Buffer_Length);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART通信发送UI函数 (1个图形)
-------------------------------------------------------------------------------------------------------------------*/
void Referee_UART_Send_Interaction_UI_Graphic_1(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_1)
{
    static uint8_t Tx_Buf[128];
    memset(Tx_Buf, 0, sizeof(Tx_Buf));
    Struct_Referee_UART_Data *tmp_buffer = (Struct_Referee_UART_Data *) Tx_Buf;

    // 1. 填充裁判系统帧头
    tmp_buffer->Referee_Frame_Header = 0xA5;
    tmp_buffer->Data_Length = sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_1) - 2;
    tmp_buffer->Referee_Sequence = Referee.Sequence;
    tmp_buffer->CRC_8 = Referee_Verify_CRC_8((uint8_t *) tmp_buffer, 4);
    tmp_buffer->Referee_Command_ID = INTERACTION;

    // 2. 填充交互帧头
    Struct_Referee_Tx_Data_Interaction_Graphic_1 *tmp_data = (Struct_Referee_Tx_Data_Interaction_Graphic_1 *) tmp_buffer->Data;
    tmp_data->Header = UI_GRAPHIC_1;
    tmp_data->Sender = Referee.Robot_Status.Robot_ID;
    tmp_data->Receiver = (Referee_Data_Robots_Client_ID_Struct)((int) (Referee.Robot_Status.Robot_ID) + 0x100);

    // 3. 填充图形数据
    tmp_data->Graphic[0] = *Graphic_1;

    // 4. 计算全包校验
    tmp_data->CRC_16 = Referee_Verify_CRC_16((uint8_t *) tmp_buffer, 7 + tmp_buffer->Data_Length);

    // 5. 直接调用 HAL 发送
    HAL_UART_Transmit(Referee.huart, (uint8_t *) tmp_buffer, 7 + sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_1), 100);

    Referee.Sequence++;
}

/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART通信发送UI函数 (2个图形)
-------------------------------------------------------------------------------------------------------------------*/
void Referee_UART_Send_Interaction_UI_Graphic_2(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_1,
                                                Struct_Referee_Data_Interaction_Graphic_Config *Graphic_2)
{
    static uint8_t Tx_Buf[128];
    memset(Tx_Buf, 0, sizeof(Tx_Buf));
    Struct_Referee_UART_Data *tmp_buffer = (Struct_Referee_UART_Data *) Tx_Buf;

    tmp_buffer->Referee_Frame_Header = 0xA5;
    tmp_buffer->Data_Length = sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_2) - 2;
    tmp_buffer->Referee_Sequence = Referee.Sequence;
    tmp_buffer->CRC_8 = Referee_Verify_CRC_8((uint8_t *) tmp_buffer, 4);
    tmp_buffer->Referee_Command_ID = INTERACTION;

    Struct_Referee_Tx_Data_Interaction_Graphic_2 *tmp_data = (Struct_Referee_Tx_Data_Interaction_Graphic_2 *) tmp_buffer->Data;
    tmp_data->Header = UI_GRAPHIC_2;
    tmp_data->Sender = Referee.Robot_Status.Robot_ID;
    tmp_data->Receiver = (Referee_Data_Robots_Client_ID_Struct)((int) (Referee.Robot_Status.Robot_ID) + 0x100);

    tmp_data->Graphic[0] = *Graphic_1;
    tmp_data->Graphic[1] = *Graphic_2;

    tmp_data->CRC_16 = Referee_Verify_CRC_16((uint8_t *) tmp_buffer, 7 + tmp_buffer->Data_Length);

    HAL_UART_Transmit(Referee.huart, (uint8_t *) tmp_buffer, 7 + sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_2), 100);

    Referee.Sequence++;
}

/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART通信发送UI函数 (5个图形)
-------------------------------------------------------------------------------------------------------------------*/
void Referee_UART_Send_Interaction_UI_Graphic_5(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_1,
                                                Struct_Referee_Data_Interaction_Graphic_Config *Graphic_2,
                                                Struct_Referee_Data_Interaction_Graphic_Config *Graphic_3,
                                                Struct_Referee_Data_Interaction_Graphic_Config *Graphic_4,
                                                Struct_Referee_Data_Interaction_Graphic_Config *Graphic_5)
{
    static uint8_t Tx_Buf[256];
    memset(Tx_Buf, 0, sizeof(Tx_Buf));
    Struct_Referee_UART_Data *tmp_buffer = (Struct_Referee_UART_Data *) Tx_Buf;

    tmp_buffer->Referee_Frame_Header = 0xA5;
    tmp_buffer->Data_Length = sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_5) - 2;
    tmp_buffer->Referee_Sequence = Referee.Sequence;
    tmp_buffer->CRC_8 = Referee_Verify_CRC_8((uint8_t *) tmp_buffer, 4);
    tmp_buffer->Referee_Command_ID = INTERACTION;

    Struct_Referee_Tx_Data_Interaction_Graphic_5 *tmp_data = (Struct_Referee_Tx_Data_Interaction_Graphic_5 *) tmp_buffer->Data;
    tmp_data->Header = UI_GRAPHIC_5;
    tmp_data->Sender = Referee.Robot_Status.Robot_ID;
    tmp_data->Receiver = (Referee_Data_Robots_Client_ID_Struct)((int) (Referee.Robot_Status.Robot_ID) + 0x100);

    tmp_data->Graphic[0] = *Graphic_1;
    tmp_data->Graphic[1] = *Graphic_2;
    tmp_data->Graphic[2] = *Graphic_3;
    tmp_data->Graphic[3] = *Graphic_4;
    tmp_data->Graphic[4] = *Graphic_5;

    tmp_data->CRC_16 = Referee_Verify_CRC_16((uint8_t *) tmp_buffer, 7 + tmp_buffer->Data_Length);

    HAL_UART_Transmit(Referee.huart, (uint8_t *) tmp_buffer, 7 + sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_5), 100);

    Referee.Sequence++;
}

/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART通信发送UI函数 (7个图形)
-------------------------------------------------------------------------------------------------------------------*/
void Referee_UART_Send_Interaction_UI_Graphic_7(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_1,
                                                Struct_Referee_Data_Interaction_Graphic_Config *Graphic_2,
                                                Struct_Referee_Data_Interaction_Graphic_Config *Graphic_3,
                                                Struct_Referee_Data_Interaction_Graphic_Config *Graphic_4,
                                                Struct_Referee_Data_Interaction_Graphic_Config *Graphic_5,
                                                Struct_Referee_Data_Interaction_Graphic_Config *Graphic_6,
                                                Struct_Referee_Data_Interaction_Graphic_Config *Graphic_7)
{
    static uint8_t Tx_Buf[256];
    memset(Tx_Buf, 0, sizeof(Tx_Buf));
    Struct_Referee_UART_Data *tmp_buffer = (Struct_Referee_UART_Data *) Tx_Buf;

    tmp_buffer->Referee_Frame_Header = 0xA5;
    tmp_buffer->Data_Length = sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_7) - 2;
    tmp_buffer->Referee_Sequence = Referee.Sequence;
    tmp_buffer->CRC_8 = Referee_Verify_CRC_8((uint8_t *) tmp_buffer, 4);
    tmp_buffer->Referee_Command_ID = INTERACTION;

    Struct_Referee_Tx_Data_Interaction_Graphic_7 *tmp_data = (Struct_Referee_Tx_Data_Interaction_Graphic_7 *) tmp_buffer->Data;
    tmp_data->Header = UI_GRAPHIC_7;
    tmp_data->Sender = Referee.Robot_Status.Robot_ID;
    tmp_data->Receiver = (Referee_Data_Robots_Client_ID_Struct)((int) (Referee.Robot_Status.Robot_ID) + 0x100);

    tmp_data->Graphic[0] = *Graphic_1;
    tmp_data->Graphic[1] = *Graphic_2;
    tmp_data->Graphic[2] = *Graphic_3;
    tmp_data->Graphic[3] = *Graphic_4;
    tmp_data->Graphic[4] = *Graphic_5;
    tmp_data->Graphic[5] = *Graphic_6;
    tmp_data->Graphic[6] = *Graphic_7;

    tmp_data->CRC_16 = Referee_Verify_CRC_16((uint8_t *) tmp_buffer, 7 + tmp_buffer->Data_Length);

    HAL_UART_Transmit(Referee.huart, (uint8_t *) tmp_buffer, 7 + sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_7), 100);

    Referee.Sequence++;
}

/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     UART通信发送UI函数 (字符串)
-------------------------------------------------------------------------------------------------------------------*/
void Referee_UART_Send_Interaction_UI_Graphic_String(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_String, const char *String_Content)
{
    static uint8_t Tx_Buf[256];
    memset(Tx_Buf, 0, sizeof(Tx_Buf));
    Struct_Referee_UART_Data *tmp_buffer = (Struct_Referee_UART_Data *) Tx_Buf;

    tmp_buffer->Referee_Frame_Header = 0xA5;
    tmp_buffer->Data_Length = sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_String) - 2;
    tmp_buffer->Referee_Sequence = Referee.Sequence;
    tmp_buffer->CRC_8 = Referee_Verify_CRC_8((uint8_t *) tmp_buffer, 4);
    tmp_buffer->Referee_Command_ID = INTERACTION;

    Struct_Referee_Tx_Data_Interaction_Graphic_String *tmp_data = (Struct_Referee_Tx_Data_Interaction_Graphic_String *) tmp_buffer->Data;
    tmp_data->Header = UI_GRAPHIC_STRING;
    tmp_data->Sender = Referee.Robot_Status.Robot_ID;
    tmp_data->Receiver = (Referee_Data_Robots_Client_ID_Struct)((int) (Referee.Robot_Status.Robot_ID) + 0x100);

    tmp_data->Graphic_String = *Graphic_String;
    // 使用更标准的 memset 替代 bzero
    memset(tmp_data->String, 0, 30);
    strncpy((char *) tmp_data->String, String_Content, 30);

    tmp_data->CRC_16 = Referee_Verify_CRC_16((uint8_t *) tmp_buffer, 7 + tmp_buffer->Data_Length);

    HAL_UART_Transmit(Referee.huart, (uint8_t *) tmp_buffer, 7 + sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_String), 100);

    Referee.Sequence++;
}



/*以下注释部分为原作写的，因为调用的串口经过二次封装了，故改为直接使用HAL库的*/
///*-------------------------------------------------------------------------------------------------------------------
//// 函数简介     UART通信发送UI函数
//// 参数说明     Graphic_1				图形地址
//// 返回参数     null
//// 使用示例     Referee_UART_Send_Interaction_UI_Graphic_1(&Graphic_1);
//// 备注信息
//-------------------------------------------------------------------------------------------------------------------*/
//void Referee_UART_Send_Interaction_UI_Graphic_1(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_1)
//{
//    Struct_Referee_UART_Data *tmp_buffer = (Struct_Referee_UART_Data *) Referee.UART_Manage_Object->Tx_Buffer;
//
//    //裁判系统帧头
//    tmp_buffer->Referee_Frame_Header = 0xA5;
//    tmp_buffer->Data_Length = sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_1) - 2;
//    tmp_buffer->Referee_Sequence = Referee.Sequence;
//    tmp_buffer->CRC_8 = Referee_Verify_CRC_8((uint8_t *) tmp_buffer, 4);
//    tmp_buffer->Referee_Command_ID = INTERACTION;
//
//    //交互帧头
//    Struct_Referee_Tx_Data_Interaction_Graphic_1 *tmp_data = (Struct_Referee_Tx_Data_Interaction_Graphic_1 *) tmp_buffer->Data;
//    tmp_data->Header = UI_GRAPHIC_1;
//    tmp_data->Sender = Referee.Robot_Status.Robot_ID;
//    tmp_data->Receiver = (Referee_Data_Robots_Client_ID_Struct)((int) (Referee.Robot_Status.Robot_ID) + 0x100);
//
//    // UI发一个图形帧内容
//    tmp_data->Graphic[0] = *Graphic_1;
//
//    tmp_data->CRC_16 = Referee_Verify_CRC_16((uint8_t *) tmp_buffer, 7 + tmp_buffer->Data_Length);
//
//    HAL_UART_Transmit(Referee.UART_Manage_Object->UART_Handler, (uint8_t *) tmp_buffer, 7 + sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_1), 100);
//
//    Referee.Sequence++;
//}
//
//
//
//
///*-------------------------------------------------------------------------------------------------------------------
//// 函数简介     UART通信发送UI函数
//// 参数说明     Graphic_1				图形地址
// 	 	 	  Graphic_2				图形地址
//// 返回参数     null
//// 使用示例     Referee_UART_Send_Interaction_UI_Graphic_2(&Graphic_1, &Graphic_2);
//// 备注信息
//-------------------------------------------------------------------------------------------------------------------*/
//void Referee_UART_Send_Interaction_UI_Graphic_2(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_1,
//												Struct_Referee_Data_Interaction_Graphic_Config *Graphic_2)
//{
//    Struct_Referee_UART_Data *tmp_buffer = (Struct_Referee_UART_Data *) Referee.UART_Manage_Object->Tx_Buffer;
//
//    // 裁判系统帧头
//    tmp_buffer->Referee_Frame_Header = 0xA5;
//    tmp_buffer->Data_Length = sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_2) - 2;
//    tmp_buffer->Referee_Sequence = Referee.Sequence;
//    tmp_buffer->CRC_8 = Referee_Verify_CRC_8((uint8_t *) tmp_buffer, 4);
//    tmp_buffer->Referee_Command_ID = INTERACTION;
//
//    // 交互帧头
//    Struct_Referee_Tx_Data_Interaction_Graphic_2 *tmp_data = (Struct_Referee_Tx_Data_Interaction_Graphic_2 *) tmp_buffer->Data;
//    tmp_data->Header = UI_GRAPHIC_2;
//    tmp_data->Sender = Referee.Robot_Status.Robot_ID;
//    tmp_data->Receiver = (Referee_Data_Robots_Client_ID_Struct)((int) (Referee.Robot_Status.Robot_ID) + 0x100);
//
//    // UI发两个图形帧内容
//    tmp_data->Graphic[0] = *Graphic_1;
//    tmp_data->Graphic[1] = *Graphic_2;
//
//    tmp_data->CRC_16 = Referee_Verify_CRC_16((uint8_t *) tmp_buffer, 7 + tmp_buffer->Data_Length);
//
//    HAL_UART_Transmit(Referee.UART_Manage_Object->UART_Handler, (uint8_t *) tmp_buffer, 7 + sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_2), 100);
//
//    Referee.Sequence++;
//}
//
//
//
//
///*-------------------------------------------------------------------------------------------------------------------
//// 函数简介     UART通信发送UI函数
//// 参数说明     Graphic_1				图形地址
// 	 	 	  Graphic_2				图形地址
// 	 	 	  Graphic_3				图形地址
// 	 	 	  Graphic_4				图形地址
// 	 	 	  Graphic_5				图形地址
//// 返回参数     null
//// 使用示例     Referee_UART_Send_Interaction_UI_Graphic_5(&Graphic_1, &Graphic_2, &Graphic_3, &Graphic_4, &Graphic_5);
//// 备注信息
//-------------------------------------------------------------------------------------------------------------------*/
//void Referee_UART_Send_Interaction_UI_Graphic_5(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_1,
//												Struct_Referee_Data_Interaction_Graphic_Config *Graphic_2,
//												Struct_Referee_Data_Interaction_Graphic_Config *Graphic_3,
//												Struct_Referee_Data_Interaction_Graphic_Config *Graphic_4,
//												Struct_Referee_Data_Interaction_Graphic_Config *Graphic_5)
//{
//    Struct_Referee_UART_Data *tmp_buffer = (Struct_Referee_UART_Data *) Referee.UART_Manage_Object->Tx_Buffer;
//
//    // 裁判系统帧头
//    tmp_buffer->Referee_Frame_Header = 0xA5;
//    tmp_buffer->Data_Length = sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_5) - 2;
//    tmp_buffer->Referee_Sequence = Referee.Sequence;
//    tmp_buffer->CRC_8 = Referee_Verify_CRC_8((uint8_t *) tmp_buffer, 4);
//    tmp_buffer->Referee_Command_ID = INTERACTION;
//
//    // 交互帧头
//    Struct_Referee_Tx_Data_Interaction_Graphic_5 *tmp_data = (Struct_Referee_Tx_Data_Interaction_Graphic_5 *) tmp_buffer->Data;
//    tmp_data->Header = UI_GRAPHIC_5;
//    tmp_data->Sender = Referee.Robot_Status.Robot_ID;
//    tmp_data->Receiver = (Referee_Data_Robots_Client_ID_Struct)((int) (Referee.Robot_Status.Robot_ID) + 0x100);
//
//    // UI发五个图形帧内容
//    tmp_data->Graphic[0] = *Graphic_1;
//    tmp_data->Graphic[1] = *Graphic_2;
//    tmp_data->Graphic[2] = *Graphic_3;
//    tmp_data->Graphic[3] = *Graphic_4;
//    tmp_data->Graphic[4] = *Graphic_5;
//
//    tmp_data->CRC_16 = Referee_Verify_CRC_16((uint8_t *) tmp_buffer, 7 + tmp_buffer->Data_Length);
//
//    HAL_UART_Transmit(Referee.UART_Manage_Object->UART_Handler, (uint8_t *) tmp_buffer, 7 + sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_5), 100);
//
//    Referee.Sequence++;
//}
//
//
//
//
///*-------------------------------------------------------------------------------------------------------------------
//// 函数简介     UART通信发送UI函数
//// 参数说明     Graphic_1				图形地址
// 	 	 	  Graphic_2				图形地址
// 	 	 	  Graphic_3				图形地址
// 	 	 	  Graphic_4				图形地址
// 	 	 	  Graphic_5				图形地址
// 	 	 	  Graphic_6				图形地址
// 	 	 	  Graphic_7				图形地址
//// 返回参数     null
//// 使用示例     Referee_UART_Send_Interaction_UI_Graphic_7(&Graphic_1, &Graphic_2, &Graphic_3, &Graphic_4, &Graphic_5, &Graphic_6, &Graphic_7);
//// 备注信息
//-------------------------------------------------------------------------------------------------------------------*/
//void Referee_UART_Send_Interaction_UI_Graphic_7(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_1,
//												Struct_Referee_Data_Interaction_Graphic_Config *Graphic_2,
//												Struct_Referee_Data_Interaction_Graphic_Config *Graphic_3,
//												Struct_Referee_Data_Interaction_Graphic_Config *Graphic_4,
//												Struct_Referee_Data_Interaction_Graphic_Config *Graphic_5,
//												Struct_Referee_Data_Interaction_Graphic_Config *Graphic_6,
//												Struct_Referee_Data_Interaction_Graphic_Config *Graphic_7)
//{
//    Struct_Referee_UART_Data *tmp_buffer = (Struct_Referee_UART_Data *) Referee.UART_Manage_Object->Tx_Buffer;
//
//    // 裁判系统帧头
//    tmp_buffer->Referee_Frame_Header = 0xA5;
//    tmp_buffer->Data_Length = sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_7) - 2;
//    tmp_buffer->Referee_Sequence = Referee.Sequence;
//    tmp_buffer->CRC_8 = Referee_Verify_CRC_8((uint8_t *) tmp_buffer, 4);
//    tmp_buffer->Referee_Command_ID = INTERACTION;
//
//    // 交互帧头
//    Struct_Referee_Tx_Data_Interaction_Graphic_7 *tmp_data = (Struct_Referee_Tx_Data_Interaction_Graphic_7 *) tmp_buffer->Data;
//    tmp_data->Header = UI_GRAPHIC_7;
//    tmp_data->Sender = Referee.Robot_Status.Robot_ID;
//    tmp_data->Receiver = (Referee_Data_Robots_Client_ID_Struct)((int) (Referee.Robot_Status.Robot_ID) + 0x100);
//
//    // UI发七个图形帧内容
//    tmp_data->Graphic[0] = *Graphic_1;
//    tmp_data->Graphic[1] = *Graphic_2;
//    tmp_data->Graphic[2] = *Graphic_3;
//    tmp_data->Graphic[3] = *Graphic_4;
//    tmp_data->Graphic[4] = *Graphic_5;
//    tmp_data->Graphic[5] = *Graphic_6;
//    tmp_data->Graphic[6] = *Graphic_7;
//
//    tmp_data->CRC_16 = Referee_Verify_CRC_16((uint8_t *) tmp_buffer, 7 + tmp_buffer->Data_Length);
//
//    HAL_UART_Transmit(Referee.UART_Manage_Object->UART_Handler, (uint8_t *) tmp_buffer, 7 + sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_7), 100);
//
//    Referee.Sequence++;
//}
//
//
//
//
///*-------------------------------------------------------------------------------------------------------------------
//// 函数简介     UART通信发送UI函数
//// 参数说明     Graphic_String		图形地址
// 	 	 	  String_Content		字符串地址
//// 返回参数     null
//// 使用示例     Referee_UART_Send_Interaction_UI_Graphic_String(&Graphic_String, &String_Content);
//// 备注信息
//-------------------------------------------------------------------------------------------------------------------*/
//void Referee_UART_Send_Interaction_UI_Graphic_String(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_String, const char *String_Content)
//{
//    Struct_Referee_UART_Data *tmp_buffer = (Struct_Referee_UART_Data *) Referee.UART_Manage_Object->Tx_Buffer;
//
//    // 裁判系统帧头
//    tmp_buffer->Referee_Frame_Header = 0xA5;
//    tmp_buffer->Data_Length = sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_String) - 2;
//    tmp_buffer->Referee_Sequence = Referee.Sequence;
//    tmp_buffer->CRC_8 = Referee_Verify_CRC_8((uint8_t *) tmp_buffer, 4);
//    tmp_buffer->Referee_Command_ID = INTERACTION;
//
//    // 交互帧头
//    Struct_Referee_Tx_Data_Interaction_Graphic_String *tmp_data = (Struct_Referee_Tx_Data_Interaction_Graphic_String *) tmp_buffer->Data;
//    tmp_data->Header = UI_GRAPHIC_STRING;
//    tmp_data->Sender = Referee.Robot_Status.Robot_ID;
//    tmp_data->Receiver = (Referee_Data_Robots_Client_ID_Struct)((int) (Referee.Robot_Status.Robot_ID) + 0x100);
//
//    // UI发字符串帧内容
//    tmp_data->Graphic_String = *Graphic_String;
//    bzero(tmp_data->String, 30);
//    strcpy((char *) tmp_data->String, String_Content);
//
//    tmp_data->CRC_16 = Referee_Verify_CRC_16((uint8_t *) tmp_buffer, 7 + tmp_buffer->Data_Length);
//
//    HAL_UART_Transmit(Referee.UART_Manage_Object->UART_Handler, (uint8_t *) tmp_buffer, 7 + sizeof(Struct_Referee_Tx_Data_Interaction_Graphic_String), 80);
//
//    Referee.Sequence++;
//}



/*原作写的，冗杂，直接在空闲中断回调里面实现*/
///*-------------------------------------------------------------------------------------------------------------------
//// 函数简介     UART通信接收回调函数
//// 参数说明     Rx_Data		接收的数据
// 	 	 	  Length		长度
//// 返回参数     null
//// 使用示例     Referee_UART_RxCpltCallback(&Rx_Data, Length);
//// 备注信息
//-------------------------------------------------------------------------------------------------------------------*/
//void Referee_UART_RxCpltCallback(uint8_t *Rx_Data, uint16_t Length)
//{
//    // 滑动窗口, 判断裁判系统是否在线
//	Referee.Flag += 1;
//
//	Referee_Data_Process(Length);
//}




///*-------------------------------------------------------------------------------------------------------------------
//// 函数简介     TIM定时器中断定期检测裁判系统是否存活
//// 参数说明     null
//// 返回参数     null
//// 使用示例     Referee_TIM_1000ms_Alive_PeriodElapsedCallback();
//// 备注信息
//-------------------------------------------------------------------------------------------------------------------*/
//void Referee_TIM_1000ms_Alive_PeriodElapsedCallback()
//{
//    // 判断该时间段内是否接收过裁判系统数据
//    if (Referee.Flag == Referee.Pre_Flag)
//    {
//        // 裁判系统断开连接
//    	Referee.Referee_Status = Referee_Status_DISABLE;
//
//        UART_Reinit(Referee.UART_Manage_Object->UART_Handler);
//    }
//    else
//    {
//        // 裁判系统保持连接
//    	Referee.Referee_Status = Referee_Status_ENABLE;
//    }
//    Referee.Pre_Flag = Referee.Flag;
//}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     裁判系统8bit循环冗余检验
// 参数说明     Message		消息
 	 	 	  Length		字节数
// 返回参数     uint8_t		校验码
// 使用示例     Referee_Verify_CRC_8(&Message, Length);
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
uint8_t Referee_Verify_CRC_8(uint8_t *Message, uint32_t Length)
{
    uint8_t index;
    uint8_t check = 0xff;

    if (Message == NULL)
    {
        return (check);
    }

    while (Length--)
    {
        index = *Message;
        Message++;
        check = crc_8_table[check ^ index];
    }
    return (check);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     裁判系统16bit循环冗余检验
// 参数说明     Message		消息
 	 	 	  Length		字节数
// 返回参数     uint8_t		校验码
// 使用示例     Referee_Verify_CRC_16(&Message, Length);
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
uint16_t Referee_Verify_CRC_16(uint8_t *Message, uint32_t Length)
{
    uint8_t index;
    uint16_t check = 0xffff;

    if (Message == NULL)
    {
        return (check);
    }

    while (Length--)
    {
        index = *Message;
        Message++;
        check = ((uint16_t) (check) >> 8) ^ crc_16_table[((uint16_t) (check) ^ (uint16_t) (index)) & 0xff];
    }
    return (check);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     数据处理过程
// 参数说明     Length		字节数
// 返回参数     null
// 使用示例     Referee_Data_Process(Length);
// 备注信息	  为节约性能不作校验但提供了接口, 如遇到大规模丢包或错乱现象, 可重新启用校验过程
-------------------------------------------------------------------------------------------------------------------*/
void Referee_Data_Process(uint16_t Length)
{
    // 数据处理过程
    Struct_Referee_UART_Data *tmp_buffer;

    for (int i = 0; i < Length;)
    {
//        tmp_buffer = (Struct_Referee_UART_Data *) &Referee.UART_Manage_Object->Rx_Buffer[i];//原作

        tmp_buffer = (Struct_Referee_UART_Data *) &Referee.p_Rx_Buffer[i];

        // 未通过头校验
        if (tmp_buffer->Referee_Frame_Header != Referee.Frame_Header)
        {
            i++;
            continue;
        }
        // 未通过CRC8校验, 顺一位继续判断
        if (Referee_Verify_CRC_8((uint8_t *) tmp_buffer, 4) != tmp_buffer->CRC_8)
        {
            i++;
            continue;
        }
        // 未通过CRC16校验, 跨过当前包继续判断
        if (Referee_Verify_CRC_16((uint8_t *) tmp_buffer, 7 + tmp_buffer->Data_Length) != *(uint16_t *) ((uint32_t) tmp_buffer + 7 + tmp_buffer->Data_Length))
        {
            i += 9 + tmp_buffer->Data_Length;
            continue;
        }
        // 通过校验但帧不够长
        if (i + 7 + tmp_buffer->Data_Length + 2 > Length)
        {
            break;
        }

        switch (tmp_buffer->Referee_Command_ID)
        {
			case (GAME_STATUS):
			{
				memcpy(&Referee.Game_Status, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Game_Status));

				break;
			}
			case (GAME_RESULT):
			{
				memcpy(&Referee.Game_Result, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Game_Result));

				break;
			}
			case (GAME_ROBOT_HP):
			{
				memcpy(&Referee.Game_Robot_HP, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Game_Robot_HP));

				break;
			}
			case (EVENT_SELF_DATA):
			{
				memcpy(&Referee.Event_Self_Data, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Event_Self_Data));

				break;
			}
			case (EVENT_SELF_SUPPLY):
			{
				memcpy(&Referee.Event_Self_Supply, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Event_Self_Supply));

				break;
			}
			case (EVENT_SELF_REFEREE_WARNING):
			{
				memcpy(&Referee.Event_Referee_Warning, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Event_Referee_Warning));

				break;
			}
			case (EVENT_SELF_DART_STATUS):
			{
				memcpy(&Referee.Event_Dart_Status, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Event_Dart_Status));

				break;
			}
			case (ROBOT_STATUS):
			{
				memcpy(&Referee.Robot_Status, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Status));

				break;
			}
			case (ROBOT_POWER_HEAT):
			{
				memcpy(&Referee.Robot_Power_Heat, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Power_Heat));

				break;
			}
			case (ROBOT_POSITION):
			{
				memcpy(&Referee.Robot_Position, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Position));

				break;
			}
			case (ROBOT_BUFF):
			{
				memcpy(&Referee.Robot_Buff, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Buff));

				break;
			}
			case (ROBOT_AERIAL_STATUS):
			{
				memcpy(&Referee.Robot_Aerial_Status, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Aerial_Status));

				break;
			}
			case (ROBOT_DAMAGE):
			{
				memcpy(&Referee.Robot_Damage, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Damage));

				break;
			}
			case (ROBOT_BOOSTER):
			{
				memcpy(&Referee.Robot_Booster, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Booster));

				break;
			}
			case (ROBOT_REMAINING_AMMO):
			{
				memcpy(&Referee.Robot_Remaining_Ammo, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Remaining_Ammo));

				break;
			}
			case (ROBOT_RFID):
			{
				memcpy(&Referee.Robot_RFID, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_RFID));

				break;
			}
			case (ROBOT_DART_COMMAND):
			{
				memcpy(&Referee.Robot_Dart_Command, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Dart_Command));

				break;
			}
			case (ROBOT_SENTRY_LOCATION):
			{
				memcpy(&Referee.Robot_Dart_Command, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Sentry_Location));

				break;
			}
			case (ROBOT_RADAR_MARK):
			{
				memcpy(&Referee.Robot_Dart_Command, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Radar_Mark));

				break;
			}
			case (ROBOT_SENTRY_DECISION):
			{
				memcpy(&Referee.Robot_Dart_Command, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Sentry_Decision));

				break;
			}
			case (ROBOT_RADAR_DECISION):
			{
				memcpy(&Referee.Robot_Dart_Command, tmp_buffer->Data, sizeof(Struct_Referee_Rx_Data_Robot_Radar_Decision));

				break;
			}
			default:
			{
				// 防非法值，可选添加日志
				break;
			}
        }

        // 缓冲区直接推移
        i += 7 + tmp_buffer->Data_Length + 2;
    }
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取裁判系统状态
// 参数说明     null
// 返回参数     Enum_Referee_Status 	裁判系统状态
// 使用示例     Referee_Get_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Status Referee_Get_Status()
{
    return (Referee.Referee_Status);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取裁判系统可信状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		裁判系统可信状态
// 使用示例     Referee_Get_Referee_Trust_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Referee_Trust_Status()
{
    return (Referee.Referee_Trust_Status);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取比赛类型
// 参数说明     null
// 返回参数     Enum_Referee_Game_Status_Type 	比赛类型
// 使用示例     Referee_Get_Game_Type();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Game_Status_Type Referee_Get_Game_Type()
{
    return ((Enum_Referee_Game_Status_Type)(Referee.Game_Status.Type_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取比赛阶段
// 参数说明     null
// 返回参数     Enum_Referee_Game_Status_Stage 	比赛阶段
// 使用示例     Referee_Get_Game_Stage();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Game_Status_Stage Referee_Get_Game_Stage()
{
    return ((Enum_Referee_Game_Status_Stage)(Referee.Game_Status.Stage_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取当前阶段剩余时间
// 参数说明     null
// 返回参数     uint16_t 		当前阶段剩余时间
// 使用示例     Referee_Get_Remaining_Time();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint16_t Referee_Get_Remaining_Time()
{
    return (Referee.Game_Status.Remaining_Time);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取系统时间戳
// 参数说明     null
// 返回参数     uint64_t 		系统时间戳
// 使用示例     Referee_Get_Timestamp();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint64_t Referee_Get_Timestamp()
{
    return (Referee.Game_Status.Timestamp);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取比赛结果
// 参数说明     null
// 返回参数     Enum_Referee_Game_Result 		比赛结果
// 使用示例     Referee_Get_Result();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Game_Result Referee_Get_Result()
{
    return (Referee.Game_Result.Result);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取机器人血量
// 参数说明     Robots_ID		通用双方机器人ID
// 返回参数     uint16_t 		机器人血量
// 使用示例     Referee_Get_HP();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint16_t Referee_Get_HP(Referee_Data_Robots_ID_Struct Robots_ID)
{
    switch (Robots_ID)
    {
		case (RED_HERO_1):
		{
			return (Referee.Game_Robot_HP.Red_Hero_1);
		}
		break;
		case (RED_ENGINEER_2):
		{
			return (Referee.Game_Robot_HP.Red_Engineer_2);
		}
		break;
		case (RED_INFANTRY_3):
		{
			return (Referee.Game_Robot_HP.Red_Infantry_3);
		}
		break;
		case (RED_INFANTRY_4):
		{
			return (Referee.Game_Robot_HP.Red_Infantry_4);
		}
		break;
		case (RED_INFANTRY_5):
		{
			return (Referee.Game_Robot_HP.Red_Infantry_5);
		}
		break;
		case (RED_SENTRY_7):
		{
		return (Referee.Game_Robot_HP.Red_Sentry_7);
		}
		break;
		case (RED_OUTPOST_11):
		{
			return (Referee.Game_Robot_HP.Red_Outpost_11);
		}
		break;
		case (RED_BASE_10):
		{
			return (Referee.Game_Robot_HP.Red_Base_10);
		}
		break;
		case (BLUE_HERO_1):
		{
			return (Referee.Game_Robot_HP.Blue_Hero_1);
		}
		break;
		case (BLUE_ENGINEER_2):
		{
			return (Referee.Game_Robot_HP.Blue_Engineer_2);
		}
		break;
		case (BLUE_INFANTRY_3):
		{
			return (Referee.Game_Robot_HP.Blue_Infantry_3);
		}
		break;
		case (BLUE_INFANTRY_4):
		{
			return (Referee.Game_Robot_HP.Blue_Infantry_4);
		}
		break;
		case (BLUE_INFANTRY_5):
		{
			return (Referee.Game_Robot_HP.Blue_Infantry_5);
		}
		break;
		case (BLUE_SENTRY_7):
		{
			return (Referee.Game_Robot_HP.Blue_Sentry_7);
		}
		break;
		case (BLUE_OUTPOST_11):
		{
			return (Referee.Game_Robot_HP.Blue_Outpost_11);
		}
		break;
		case (BLUE_BASE_10):
		{
			return (Referee.Game_Robot_HP.Blue_Base_10);
		}
		break;
		default:
		{
			return 0;
		}
		break;
    }
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取补给站前占领状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		补给站前占领状态
// 使用示例     Referee_Get_Supply_Front_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Supply_Front_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Event_Self_Data.Supply_Front_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取补给站内占领状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		补给站内占领状态
// 使用示例     Referee_Get_Supply_Inner_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Supply_Inner_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Event_Self_Data.Supply_Inner_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取补给站占领状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		补给站占领状态
// 使用示例     Referee_Get_Supply_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Supply_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Event_Self_Data.Supply_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取能量机关占领状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		能量机关占领状态
// 使用示例     Referee_Get_Power_Rune_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Power_Rune_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Event_Self_Data.Power_Rune_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取小能量机关激活状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		小能量机关激活状态
// 使用示例     Referee_Get_Power_Rune_Small_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Power_Rune_Small_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Event_Self_Data.Power_Rune_Small_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取大能量机关激活状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		大能量机关激活状态
// 使用示例     Referee_Get_Power_Rune_Big_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Power_Rune_Big_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Event_Self_Data.Power_Rune_Big_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取环形高地占领状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		环形高地占领状态
// 使用示例     Referee_Get_Highland_2_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Highland_2_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Event_Self_Data.Highland_2_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取梯形高地3占领状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		梯形高地3占领状态
// 使用示例     Referee_Get_Highland_3_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Highland_3_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Event_Self_Data.Highland_3_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取梯形高地4占领状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		梯形高地4占领状态
// 使用示例     Referee_Get_Highland_4_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Highland_4_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Event_Self_Data.Highland_4_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取基地护盾状态
// 参数说明     null
// 返回参数     uint8_t 		基地护盾状态
// 使用示例     Referee_Get_Base_Virtual_Shield_Percent();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_Base_Virtual_Shield_Percent()
{
    return (Referee.Event_Self_Data.Base_Virtual_Shield_Percent);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取敌方飞镖最后一次命中己方时间
// 参数说明     null
// 返回参数     uint16_t 		敌方飞镖最后一次命中己方时间
// 使用示例     Referee_Get_Enemy_Dart_Hit_Time();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint16_t Referee_Get_Enemy_Dart_Hit_Time()
{
    return (Referee.Event_Self_Data.Enemy_Dart_Hit_Time);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取敌方飞镖最后一次命中己方建筑物
// 参数说明     null
// 返回参数     Enum_Referee_Dart_Hit_Target 		敌方飞镖最后一次命中己方建筑物
// 使用示例     Referee_Get_Enemy_Dart_Hit_Target_Enum();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Dart_Hit_Target Referee_Get_Enemy_Dart_Hit_Target_Enum()
{
    return ((Enum_Referee_Dart_Hit_Target)(Referee.Event_Self_Data.Enemy_Dart_Hit_Target_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取中心增益点占领状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		中心增益点占领状态
// 使用示例     Referee_Get_Middle_Buff_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Middle_Buff_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Event_Self_Data.Middle_Buff_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取请求补给的机器人ID
// 参数说明     null
// 返回参数     Referee_Data_Robots_ID_Struct 		请求补给的机器人ID
// 使用示例     Referee_Get_Supply_Request_Robot();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Referee_Data_Robots_ID_Struct Referee_Get_Supply_Request_Robot()
{
    return (Referee.Event_Self_Supply.Robot);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取补给站的补给状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Event_Supply_Status 		补给站的补给状态
// 使用示例     Referee_Get_Supply_Request_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Event_Supply_Status Referee_Get_Supply_Request_Status()
{
    return (Referee.Event_Self_Supply.Status);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取补给子弹数量
// 参数说明     null
// 返回参数     Enum_Referee_Data_Event_Supply_Ammo_Number 		补给子弹数量
// 使用示例     Referee_Get_Middle_Buff_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Event_Supply_Ammo_Number Referee_Get_Supply_Ammo_Number()
{
    return (Referee.Event_Self_Supply.Ammo_Number);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取裁判判罚信息
// 参数说明     null
// 返回参数     Enum_Referee_Data_Event_Referee_Warning_Level 	裁判判罚信息
// 使用示例     Referee_Get_Referee_Warning();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Event_Referee_Warning_Level Referee_Get_Referee_Warning()
{
    return (Referee.Event_Referee_Warning.Level);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取裁判判罚机器人
// 参数说明     null
// 返回参数     Referee_Data_Robot_ID_Struct 		裁判判罚机器人
// 使用示例     Referee_Get_Referee_Warning_Robot();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Referee_Data_Robot_ID_Struct Referee_Get_Referee_Warning_Robot()
{
    return (Referee.Event_Referee_Warning.Robot_ID);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取飞镖剩余时间
// 参数说明     null
// 返回参数     uint8_t 		飞镖剩余时间
// 使用示例     Referee_Get_Dart_Remaining_Time();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_Dart_Remaining_Time()
{
    return (Referee.Event_Dart_Status.Dart_Remaining_Time);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取飞镖上一次击中的目标
// 参数说明     null
// 返回参数     Enum_Referee_Dart_Hit_Target 		飞镖上一次击中的目标
// 使用示例     Referee_Get_Last_Dart_Hit_Target();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Dart_Hit_Target Referee_Get_Last_Dart_Hit_Target()
{
    return ((Enum_Referee_Dart_Hit_Target)(Referee.Event_Dart_Status.Dart_Hit_Target_Enum_Last));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取飞镖上一次击中目标的次数
// 参数说明     null
// 返回参数     uint8_t 		飞镖上一次击中目标的次数
// 使用示例     Referee_Get_Dart_Hit_Target_Count();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_Dart_Hit_Target_Count()
{
    return (Referee.Event_Dart_Status.Dart_Hit_Target_Count);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取飞镖当前的目标
// 参数说明     null
// 返回参数     Enum_Referee_Dart_Hit_Target 		飞镖当前的目标
// 使用示例     Referee_Get_Now_Dart_Hit_Target();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Dart_Hit_Target Referee_Get_Now_Dart_Hit_Target()
{
    return ((Enum_Referee_Dart_Hit_Target)(Referee.Event_Dart_Status.Dart_Hit_Target_Enum_Now));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取自身ID
// 参数说明     null
// 返回参数     Referee_Data_Robots_ID_Struct 		自身ID
// 使用示例     Referee_Get_Self_ID();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Referee_Data_Robots_ID_Struct Referee_Get_Self_ID()
{
    return (Referee.Robot_Status.Robot_ID);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取自身等级
// 参数说明     null
// 返回参数     uint8_t 		自身等级
// 使用示例     Referee_Get_Self_Level();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_Self_Level()
{
    return (Referee.Robot_Status.Level);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取自身血量
// 参数说明     null
// 返回参数     uint16_t 		自身血量
// 使用示例     Referee_Get_Self_HP();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint16_t Referee_Get_Self_HP()
{
    return (Referee.Robot_Status.HP);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取自身血量上限
// 参数说明     null
// 返回参数     uint16_t 		自身血量上限
// 使用示例     Referee_Get_Self_HP_Max();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint16_t Referee_Get_Self_HP_Max()
{
    return (Referee.Robot_Status.HP_Max);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取发射机构冷却
// 参数说明     null
// 返回参数     uint16_t 		发射机构冷却
// 使用示例     Referee_Get_Self_Booster_Heat_CD();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint16_t Referee_Get_Self_Booster_Heat_CD()
{
    return (Referee.Robot_Status.Booster_Heat_CD);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取发射机构热量上限
// 参数说明     null
// 返回参数     uint16_t 		发射机构热量上限
// 使用示例     Referee_Get_Self_Booster_Heat_Max();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint16_t Referee_Get_Self_Booster_Heat_Max()
{
    return (Referee.Robot_Status.Booster_Heat_Max);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取底盘功率上限
// 参数说明     null
// 返回参数     uint16_t 		底盘功率上限
// 使用示例     Referee_Get_Self_Chassis_Power_Max();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint16_t Referee_Get_Self_Chassis_Power_Max()
{
    return (Referee.Robot_Status.Chassis_Power_Max);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取Gimbal供电状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		Gimbal供电状态
// 使用示例     Referee_Get_Self_HP();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Self_PM01_Gimbal_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_Status.PM01_Gimbal_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取Chassis供电状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		Chassis供电状态
// 使用示例     Referee_Get_Self_PM01_Chassis_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Self_PM01_Chassis_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_Status.PM01_Chassis_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取Booster供电状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		Booster供电状态
// 使用示例     Referee_Get_Self_PM01_Booster_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Self_PM01_Booster_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_Status.PM01_Booster_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取底盘电压
// 参数说明     null
// 返回参数     float 		底盘电压
// 使用示例     Referee_Get_Chassis_Voltage();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline float Referee_Get_Chassis_Voltage()
{
    return (Referee.Robot_Power_Heat.Chassis_Voltage / 1000.0f);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取底盘电流
// 参数说明     null
// 返回参数     float 		底盘电流
// 使用示例     Referee_Get_Chassis_Current();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline float Referee_Get_Chassis_Current()
{
    return (Referee.Robot_Power_Heat.Chassis_Current / 1000.0f);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取底盘功率
// 参数说明     null
// 返回参数     float 		底盘功率
// 使用示例     Referee_Get_Chassis_Power();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline float Referee_Get_Chassis_Power()
{
    return (Referee.Robot_Power_Heat.Chassis_Power);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取底盘能量缓冲
// 参数说明     null
// 返回参数     uint16_t 		底盘能量缓冲
// 使用示例     Referee_Get_Chassis_Energy_Buffer();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint16_t Referee_Get_Chassis_Energy_Buffer()
{
    return (Referee.Robot_Power_Heat.Chassis_Energy_Buffer);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取17mm1热量
// 参数说明     null
// 返回参数     uint16_t 		17mm1热量
// 使用示例     Referee_Get_Booster_17mm_1_Heat();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint16_t Referee_Get_Booster_17mm_1_Heat()
{
    return (Referee.Robot_Power_Heat.Booster_17mm_1_Heat);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取17mm2热量
// 参数说明     null
// 返回参数     uint16_t 		17mm2热量
// 使用示例     Referee_Get_Booster_17mm_2_Heat();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint16_t Referee_Get_Booster_17mm_2_Heat()
{
    return (Referee.Robot_Power_Heat.Booster_17mm_2_Heat);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取42mm热量
// 参数说明     null
// 返回参数     uint16_t 		42mm热量
// 使用示例     Referee_Get_Booster_42mm_Heat();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint16_t Referee_Get_Booster_42mm_Heat()
{
    return (Referee.Robot_Power_Heat.Booster_42mm_Heat);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取自身位置x
// 参数说明     null
// 返回参数     float 		自身位置x
// 使用示例     Referee_Get_Location_X();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline float Referee_Get_Location_X()
{
    return (Referee.Robot_Position.Location_X);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取自身位置y
// 参数说明     null
// 返回参数     float 		自身位置y
// 使用示例     Referee_Get_Location_Y();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline float Referee_Get_Location_Y()
{
    return (Referee.Robot_Position.Location_Y);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取自身方向yaw
// 参数说明     null
// 返回参数     float 		自身方向yaw
// 使用示例     Referee_Get_Location_Yaw();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline float Referee_Get_Location_Yaw()
{
    return (Referee.Robot_Position.Location_Yaw);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取补血buff百分比
// 参数说明     null
// 返回参数     uint8_t 		补血buff百分比
// 使用示例     Referee_Get_HP_Buff_Percent();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_HP_Buff_Percent()
{
    return (Referee.Robot_Buff.HP_Buff_Percent);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取冷却缩减buff倍数
// 参数说明     null
// 返回参数     uint8_t 		冷却缩减buff倍数
// 使用示例     Referee_Get_Booster_Heat_CD_Buff_Value();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_Booster_Heat_CD_Buff_Value()
{
    return (Referee.Robot_Buff.Booster_Heat_CD_Buff_Value);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取防御加成buff百分比
// 参数说明     null
// 返回参数     uint8_t 		防御加成buff百分比
// 使用示例     Referee_Get_Defend_Buff_Percent();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_Defend_Buff_Percent()
{
    return (Referee.Robot_Buff.Defend_Buff_Percent);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取负防御加成buff百分比
// 参数说明     null
// 返回参数     uint8_t 		负防御加成buff百分比
// 使用示例     Referee_Get_Defend_Debuff_Percent();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_Defend_Debuff_Percent()
{
    return (Referee.Robot_Buff.Defend_Buff_Percent);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取攻击加成buff状态
// 参数说明     null
// 返回参数     uint8_t 		攻击加成buff状态
// 使用示例     Referee_Get_Damage_Buff_Percent();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_Damage_Buff_Percent()
{
    return (Referee.Robot_Buff.Damage_Buff_Percent);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取无人机状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Event_Aerial_Status 		无人机状态
// 使用示例     Referee_Get_Aerial_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Event_Aerial_Status Referee_Get_Aerial_Status()
{
    return (Referee.Robot_Aerial_Status.Aerial_Status);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取无人机时间
// 参数说明     null
// 返回参数     uint8_t 		无人机时间
// 使用示例     Referee_Get_Aerial_Remaining_Time();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_Aerial_Remaining_Time()
{
    return (Referee.Robot_Aerial_Status.Remaining_Time);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取受击装甲板ID
// 参数说明     null
// 返回参数     uint8_t 		受击装甲板ID
// 使用示例     Referee_Get_Armor_Attacked_ID();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_Armor_Attacked_ID()
{
    return (Referee.Robot_Damage.Armor_ID);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取受击类型
// 参数说明     null
// 返回参数     Enum_Referee_Data_Event_Robot_Damage_Type 	受击类型
// 使用示例     Referee_Get_Attacked_Type();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Event_Robot_Damage_Type Referee_Get_Attacked_Type()
{
    return ((Enum_Referee_Data_Event_Robot_Damage_Type)(Referee.Robot_Damage.Type_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取射击子弹类型
// 参数说明     null
// 返回参数     Enum_Referee_Data_Robot_Ammo_Type 		射击子弹类型
// 使用示例     Referee_Get_Shoot_Ammo_Type();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Robot_Ammo_Type Referee_Get_Shoot_Ammo_Type()
{
    return (Referee.Robot_Booster.Ammo_Type);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取发射机构类型
// 参数说明     null
// 返回参数     Enum_Referee_Data_Robot_Booster_Type 		发射机构类型
// 使用示例     Referee_Get_Shoot_Booster_Type();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Robot_Booster_Type Referee_Get_Shoot_Booster_Type()
{
    return (Referee.Robot_Booster.Booster_Type);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取射频, Hz
// 参数说明     null
// 返回参数     uint8_t 		射频, Hz
// 使用示例     Referee_Get_Shoot_Frequency();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_Shoot_Frequency()
{
    return (Referee.Robot_Booster.Frequency);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取射速
// 参数说明     null
// 返回参数     float 		射速
// 使用示例     Referee_Get_Shoot_Speed();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline float Referee_Get_Shoot_Speed()
{
    return (Referee.Robot_Booster.Speed);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取17mm弹丸剩余数
// 参数说明     null
// 返回参数     uint16_t 		17mm弹丸剩余数
// 使用示例     Referee_Get_17mm_Remaining();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint16_t Referee_Get_17mm_Remaining()
{
    return (Referee.Robot_Remaining_Ammo.Booster_17mm);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取42mm弹丸剩余数
// 参数说明     null
// 返回参数     uint16_t 		42mm弹丸剩余数
// 使用示例     Referee_Get_42mm_Remaining();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint16_t Referee_Get_42mm_Remaining()
{
    return (Referee.Robot_Remaining_Ammo.Booster_42mm);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取金币剩余数
// 参数说明     null
// 返回参数     uint16_t 		金币剩余数
// 使用示例     Referee_Get_Money_Remaining();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint16_t Referee_Get_Money_Remaining()
{
    return (Referee.Robot_Remaining_Ammo.Money);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取基地增益RFID状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		基地增益RFID状态
// 使用示例     Referee_Get_Base_RFID_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Base_RFID_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_RFID.Base_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取己方环形高地RFID状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		己方环形高地RFID状态
// 使用示例     Referee_Get_Highland_2_Self_RFID_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Highland_2_Self_RFID_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_RFID.Highland_2_Self_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取敌方环形高地RFID状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		敌方环形高地RFID状态
// 使用示例     Referee_Get_Highland_2_Enemy_RFID_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Highland_2_Enemy_RFID_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_RFID.Highland_2_Enemy_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取己方高地3RFID状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		己方环形高地3RFID状态
// 使用示例     Referee_Get_Highland_3_Self_RFID_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Highland_3_Self_RFID_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_RFID.Highland_3_Self_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取敌方高地3RFID状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		敌方高地3RFID状态
// 使用示例     Referee_Get_Highland_3_Enemy_RFID_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Highland_3_Enemy_RFID_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_RFID.Highland_3_Enemy_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取己方高地4RFID状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		己方环形高地4RFID状态
// 使用示例     Referee_Get_Highland_4_Self_RFID_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Highland_4_Self_RFID_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_RFID.Highland_4_Self_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取敌方高地4RFID状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		敌方高地4RFID状态
// 使用示例     Referee_Get_Highland_4_Enemy_RFID_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Highland_4_Enemy_RFID_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_RFID.Highland_4_Enemy_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取能量机关增益RFID状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		能量机关增益RFID状态
// 使用示例     Referee_Get_Energy_RFID_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Energy_RFID_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_RFID.Energy_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取己方1阶段飞坡增益RFID状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		己方1阶段飞坡增益RFID状态
// 使用示例     Referee_Get_Flyover_1_Self_RFID_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Flyover_1_Self_RFID_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_RFID.Flyover_1_Self_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取己方2阶段飞坡增益RFID状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		己方2阶段飞坡增益RFID状态
// 使用示例     Referee_Get_Flyover_2_Self_RFID_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Flyover_2_Self_RFID_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_RFID.Flyover_2_Self_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取敌方1阶段飞坡增益RFID状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		敌方1阶段飞坡增益RFID状态
// 使用示例     Referee_Get_Flyover_1_Enemy_RFID_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Flyover_1_Enemy_RFID_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_RFID.Flyover_1_Enemy_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取敌方2阶段飞坡增益RFID状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		敌方2阶段飞坡增益RFID状态
// 使用示例     Referee_Get_Flyover_2_Enemy_RFID_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Flyover_2_Enemy_RFID_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_RFID.Flyover_2_Enemy_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取前哨站增益RFID状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		前哨站增益RFID状态
// 使用示例     Referee_Get_Outpost_RFID_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Outpost_RFID_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_RFID.Outpost_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取补血点增益RFID状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		补血点增益RFID状态
// 使用示例     Referee_Get_HP_RFID_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_HP_RFID_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_RFID.HP_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取己方哨兵巡逻区增益RFID状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		己方哨兵巡逻区增益RFID状态
// 使用示例     Referee_Get_Base_RFID_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Sentry_Self_RFID_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_RFID.Sentry_Self_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取敌方哨兵巡逻区增益RFID状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		敌方哨兵巡逻区增益RFID状态
// 使用示例     Referee_Get_Sentry_Enemy_RFID_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Sentry_Enemy_RFID_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_RFID.Sentry_Enemy_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取己方工程采矿区增益RFID状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		己方工程采矿区增益RFID状态
// 使用示例     Referee_Get_Base_RFID_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Engineer_Self_RFID_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_RFID.Engineer_Self_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取敌方工程采矿区增益RFID状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		敌方工程采矿区增益RFID状态
// 使用示例     Referee_Get_Engineer_Enemy_RFID_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Engineer_Enemy_RFID_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_RFID.Engineer_Enemy_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取工程兑换站增益RFID状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		工程兑换站增益RFID状态
// 使用示例     Referee_Get_Engineer_Exchange_RFID_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Engineer_Exchange_RFID_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_RFID.Engineer_Exchange_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取中心增益点RFID状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		中心增益点RFID状态
// 使用示例     Referee_Get_Middle_RFID_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Middle_RFID_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_RFID.Middle_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取飞镖发射口状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Robot_Dart_Command_Status 		飞镖发射口状态
// 使用示例     Referee_Get_Dart_Command_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Robot_Dart_Command_Status Referee_Get_Dart_Command_Status()
{
    return (Referee.Robot_Dart_Command.Status);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取飞镖切换击打目标时的比赛剩余时间
// 参数说明     null
// 返回参数     uint16_t 		飞镖切换击打目标时的比赛剩余时间
// 使用示例     Referee_Get_Dart_Switch_Remaining_Time();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint16_t Referee_Get_Dart_Switch_Remaining_Time()
{
    return (Referee.Robot_Dart_Command.Switch_Remaining_Time);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取最后一次操作手下达发射指令时的比赛剩余时间
// 参数说明     null
// 返回参数     uint16_t 		最后一次操作手下达发射指令时的比赛剩余时间
// 使用示例     Referee_Get_Dart_Launch_Remaining_Time();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint16_t Referee_Get_Dart_Launch_Remaining_Time()
{
    return (Referee.Robot_Dart_Command.Launch_Remaining_Time);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取己方英雄坐标x
// 参数说明     null
// 返回参数     float 		己方英雄坐标x
// 使用示例     Referee_Get_Sentry_Location_Hero_1_X();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline float Referee_Get_Sentry_Location_Hero_1_X()
{
    return (Referee.Robot_Sentry_Location.Hero_1_X);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取己方英雄坐标y
// 参数说明     null
// 返回参数     float 		己方英雄坐标y
// 使用示例     Referee_Get_Sentry_Location_Hero_1_Y();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline float Referee_Get_Sentry_Location_Hero_1_Y()
{
    return (Referee.Robot_Sentry_Location.Hero_1_Y);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取己方工程坐标x
// 参数说明     null
// 返回参数     float 		己方工程坐标x
// 使用示例     Referee_Get_Sentry_Location_Engineer_2_X();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline float Referee_Get_Sentry_Location_Engineer_2_X()
{
    return (Referee.Robot_Sentry_Location.Engineer_2_X);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取己方工程坐标y
// 参数说明     null
// 返回参数     float 		己方工程坐标y
// 使用示例     Referee_Get_Sentry_Location_Engineer_2_Y();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline float Referee_Get_Sentry_Location_Engineer_2_Y()
{
    return (Referee.Robot_Sentry_Location.Engineer_2_Y);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取己方步兵坐标x
// 参数说明     null
// 返回参数     float 		己方步兵坐标x
// 使用示例     Referee_Get_Sentry_Location_Infantry_3_X();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline float Referee_Get_Sentry_Location_Infantry_3_X()
{
    return (Referee.Robot_Sentry_Location.Infantry_3_X);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取己方步兵坐标y
// 参数说明     null
// 返回参数     float 		己方步兵坐标y
// 使用示例     Referee_Get_Sentry_Location_Infantry_3_Y();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline float Referee_Get_Sentry_Location_Infantry_3_Y()
{
    return (Referee.Robot_Sentry_Location.Infantry_3_Y);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取己方步兵坐标x
// 参数说明     null
// 返回参数     float 		己方步兵坐标x
// 使用示例     Referee_Get_Sentry_Location_Infantry_4_X();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline float Referee_Get_Sentry_Location_Infantry_4_X()
{
    return (Referee.Robot_Sentry_Location.Infantry_4_X);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取己方步兵坐标y
// 参数说明     null
// 返回参数     float 		己方步兵坐标y
// 使用示例     Referee_Get_Sentry_Location_Infantry_4_Y();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline float Referee_Get_Sentry_Location_Infantry_4_Y()
{
    return (Referee.Robot_Sentry_Location.Infantry_4_Y);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取己方步兵坐标x
// 参数说明     null
// 返回参数     float 		己方步兵坐标x
// 使用示例     Referee_Get_Sentry_Location_Infantry_5_X();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline float Referee_Get_Sentry_Location_Infantry_5_X()
{
    return (Referee.Robot_Sentry_Location.Infantry_5_X);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取己方步兵坐标y
// 参数说明     null
// 返回参数     float 		己方步兵坐标y
// 使用示例     Referee_Get_Sentry_Location_Infantry_5_Y();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline float Referee_Get_Sentry_Location_Infantry_5_Y()
{
    return (Referee.Robot_Sentry_Location.Infantry_5_Y);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取雷达对敌方英雄标记进度
// 参数说明     null
// 返回参数     uint8_t 		雷达对敌方英雄标记进度
// 使用示例     Referee_Get_Radar_Mark_Status_Hero_1();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_Radar_Mark_Status_Hero_1()
{
    return (Referee.Robot_Radar_Mark.Hero_1_Mark);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取雷达对敌方工程标记进度
// 参数说明     null
// 返回参数     uint8_t 		雷达对敌方工程标记进度
// 使用示例     Referee_Get_Radar_Mark_Status_Engineer_2();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_Radar_Mark_Status_Engineer_2()
{
    return (Referee.Robot_Radar_Mark.Engineer_2_Mark);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取雷达对敌方步兵标记进度
// 参数说明     null
// 返回参数     uint8_t 		雷达对敌方步兵标记进度
// 使用示例     Referee_Get_Radar_Mark_Status_Infantry_3();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_Radar_Mark_Status_Infantry_3()
{
    return (Referee.Robot_Radar_Mark.Infantry_3_Mark);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取雷达对敌方步兵标记进度
// 参数说明     null
// 返回参数     uint8_t 		雷达对敌方步兵标记进度
// 使用示例     Referee_Get_Radar_Mark_Status_Infantry_4();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_Radar_Mark_Status_Infantry_4()
{
    return (Referee.Robot_Radar_Mark.Infantry_4_Mark);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取雷达对敌方步兵标记进度
// 参数说明     null
// 返回参数     uint8_t 		雷达对敌方步兵标记进度
// 使用示例     Referee_Get_Radar_Mark_Status_Infantry_5();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_Radar_Mark_Status_Infantry_5()
{
    return (Referee.Robot_Radar_Mark.Infantry_5_Mark);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取雷达对敌方哨兵标记进度
// 参数说明     null
// 返回参数     uint8_t 		雷达对敌方哨兵标记进度
// 使用示例     Referee_Get_Radar_Mark_Status_Sentry_7();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_Radar_Mark_Status_Sentry_7()
{
    return (Referee.Robot_Radar_Mark.Sentry_7_Mark);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取哨兵决策购买子弹数量
// 参数说明     null
// 返回参数     uint16_t 		哨兵决策购买子弹数量
// 使用示例     Referee_Get_Sentry_Decision_Ammo_Exchange_Number();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint16_t Referee_Get_Sentry_Decision_Ammo_Exchange_Number()
{
    return (Referee.Robot_Sentry_Decision.Ammo_Exchange_Number);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取哨兵决策购买子弹次数
// 参数说明     null
// 返回参数     uint8_t 		哨兵决策购买子弹次数
// 使用示例     Referee_Get_Sentry_Decision_Ammo_Exchange_Time();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_Sentry_Decision_Ammo_Exchange_Time()
{
    return (Referee.Robot_Sentry_Decision.Ammo_Exchange_Time);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取哨兵决策购买血量次数
// 参数说明     null
// 返回参数     uint8_t 		哨兵决策购买血量次数
// 使用示例     Referee_Get_Sentry_Decision_HP_Exchange_Time();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_Sentry_Decision_HP_Exchange_Time()
{
    return (Referee.Robot_Sentry_Decision.HP_Exchange_Time);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取雷达决策重伤敌方机会
// 参数说明     null
// 返回参数     uint8_t 		雷达决策重伤敌方机会
// 使用示例     Referee_Get_Radar_Decision_Double_Damage_Chance();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline uint8_t Referee_Get_Radar_Decision_Double_Damage_Chance()
{
    return (Referee.Robot_Radar_Decision.Double_Damage_Chance);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取雷达决策重伤敌方状态
// 参数说明     null
// 返回参数     Enum_Referee_Data_Status 		雷达决策重伤敌方状态
// 使用示例     Referee_Get_Radar_Decision_Double_Damage_Enemy_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Enum_Referee_Data_Status Referee_Get_Radar_Decision_Double_Damage_Enemy_Status()
{
    return ((Enum_Referee_Data_Status)(Referee.Robot_Radar_Decision.Double_Damage_Enemy_Status_Enum));
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     设定裁判系统是否可信
// 参数说明     __Referee_Trust_Status		标签,场地,相关设施激活与存活状态
// 返回参数     Enum_Referee_Data_Status 		裁判系统是否可信
// 使用示例     Referee_Set_Referee_Trust_Status();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline void Referee_Set_Referee_Trust_Status(Enum_Referee_Data_Status __Referee_Trust_Status)
{
	Referee.Referee_Trust_Status = __Referee_Trust_Status;
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     设定裁判系统UI清除
// 参数说明     null
// 返回参数     null
// 使用示例     Referee_Set_Referee_UI_Change_Flag_Clear();
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline void Referee_Set_Referee_UI_Change_Flag_Clear()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
        	Referee.UI_Change_Flag[i][j] = 0;
        }
    }
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     设定裁判系统UI清除
// 参数说明     Layer_Num												图层编号, 0~9
 	 	 	  Graphic_Num											图形编号, 0~9
// 返回参数     Struct_Referee_Data_Interaction_Graphic_Config 		图形配置
// 使用示例     Referee_Set_Referee_UI_Clear(Layer_Num, Graphic_Num);
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Struct_Referee_Data_Interaction_Graphic_Config *Referee_Set_Referee_UI_Clear(uint8_t Layer_Num, uint8_t Graphic_Num)
{
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[0] = '0';
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[1] = '0' + Layer_Num;

	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[2] = '0' + Graphic_Num;

	Referee.Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_DELETE;

    Referee.UI_Change_Flag[Layer_Num][Graphic_Num] = 0;

    return (&Referee.Graphic_Config[Layer_Num][Graphic_Num]);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     设定裁判系统UI直线
// 参数说明     Layer_Num												图层编号, 0~9
 	 	 	  Graphic_Num											图形编号, 0~9
 	 	 	  Color													图形颜色
 	 	 	  Line_Width											线宽
 	 	 	  Start_X												起点x
 	 	 	  Start_Y												起点y
// 返回参数     Struct_Referee_Data_Interaction_Graphic_Config 		对应图层指针
// 使用示例     Referee_Set_Referee_UI_Line(Layer_Num, Graphic_Num, Color, Line_Width, Start_X, Start_Y);
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Struct_Referee_Data_Interaction_Graphic_Config *Referee_Set_Referee_UI_Line(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Start_X, uint32_t Start_Y, uint32_t End_X, uint32_t End_Y)
{
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[0] = '0';
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[1] = '0' + Layer_Num;
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[2] = '0' + Graphic_Num;

    if (Referee.UI_Change_Flag[Layer_Num][Graphic_Num] == 0)
    {
    	Referee.Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_ADD;
    	Referee.UI_Change_Flag[Layer_Num][Graphic_Num] = 1;
    }
    else
    {
    	Referee.Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_CHANGE;
    }

    Referee.Graphic_Config[Layer_Num][Graphic_Num].Type_Enum = Referee_Data_Interaction_Graphic_Type_LINE;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Layer_Num = Layer_Num;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Color_Enum = Color;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Line_Width = Line_Width;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Start_X = Start_X;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Start_Y = Start_Y;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Details_D = End_X;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Details_E = End_Y;

    return (&Referee.Graphic_Config[Layer_Num][Graphic_Num]);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     设定裁判系统UI矩形
// 参数说明     Layer_Num												图层编号, 0~9
 	 	 	  Graphic_Num											图形编号, 0~9
 	 	 	  Color													图形颜色
 	 	 	  Line_Width											线宽
 	 	 	  Start_X												起点x
 	 	 	  Start_Y												起点y
 	 	 	  End_X													终点x
 	 	 	  End_Y													终点y
// 返回参数     Struct_Referee_Data_Interaction_Graphic_Config 		对应图层指针
// 使用示例     Referee_Set_Referee_UI_Rectangle(Layer_Num, Graphic_Num, Color, Line_Width, Start_X, Start_Y, End_X, End_Y);
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Struct_Referee_Data_Interaction_Graphic_Config *Referee_Set_Referee_UI_Rectangle(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Start_X, uint32_t Start_Y, uint32_t End_X, uint32_t End_Y)
{
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[0] = '0';
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[1] = '0' + Layer_Num;
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[2] = '0' + Graphic_Num;

    if (Referee.UI_Change_Flag[Layer_Num][Graphic_Num] == 0)
    {
    	Referee.Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_ADD;
    	Referee.UI_Change_Flag[Layer_Num][Graphic_Num] = 1;
    }
    else
    {
    	Referee.Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_CHANGE;
    }

    Referee.Graphic_Config[Layer_Num][Graphic_Num].Type_Enum = Referee_Data_Interaction_Graphic_Type_RECTANGLE;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Layer_Num = Layer_Num;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Color_Enum = Color;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Line_Width = Line_Width;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Start_X = Start_X;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Start_Y = Start_Y;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Details_D = End_X;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Details_E = End_Y;

    return (&Referee.Graphic_Config[Layer_Num][Graphic_Num]);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     设定裁判系统UI圆形
// 参数说明     Layer_Num												图层编号, 0~9
 	 	 	  Graphic_Num											图形编号, 0~9
 	 	 	  Color													图形颜色
 	 	 	  Line_Width											线宽
 	 	 	  Center_X												圆心x
 	 	 	  Center_Y												圆心y
 	 	 	  Radius												半径
// 返回参数     Struct_Referee_Data_Interaction_Graphic_Config 		对应图层指针
// 使用示例     Referee_Set_Referee_UI_Circle(Layer_Num, Graphic_Num, Color, Line_Width, Center_X, Center_Y, Radius);
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Struct_Referee_Data_Interaction_Graphic_Config *Referee_Set_Referee_UI_Circle(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Center_X, uint32_t Center_Y, uint32_t Radius)
{
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[0] = '0';
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[1] = '0' + Layer_Num;
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[2] = '0' + Graphic_Num;

    if (Referee.UI_Change_Flag[Layer_Num][Graphic_Num] == 0)
    {
    	Referee.Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_ADD;
    	Referee.UI_Change_Flag[Layer_Num][Graphic_Num] = 1;
    }
    else
    {
    	Referee.Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_CHANGE;
    }

    Referee.Graphic_Config[Layer_Num][Graphic_Num].Type_Enum = Referee_Data_Interaction_Graphic_Type_CIRCLE;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Layer_Num = Layer_Num;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Color_Enum = Color;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Line_Width = Line_Width;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Start_X = Center_X;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Start_Y = Center_Y;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Details_C = Radius;

    return (&Referee.Graphic_Config[Layer_Num][Graphic_Num]);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     设定裁判系统UI椭圆形
// 参数说明     Layer_Num												图层编号, 0~9
			  Graphic_Num											图形编号, 0~9
			  Color													图形颜色
			  Line_Width											线宽
			  Center_X												圆心x
			  Center_Y												圆心y
			  Length_X												x半轴长度
			  Length_Y												y半轴长度
// 返回参数     Struct_Referee_Data_Interaction_Graphic_Config 		对应图层指针
// 使用示例     Referee_Set_Referee_UI_Oval(Layer_Num, Graphic_Num, Color, Line_Width, Center_X, Center_Y, Length_X, Length_Y);
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Struct_Referee_Data_Interaction_Graphic_Config *Referee_Set_Referee_UI_Oval(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Center_X, uint32_t Center_Y, uint32_t Length_X, uint32_t Length_Y)
{
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[0] = '0';
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[1] = '0' + Layer_Num;
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[2] = '0' + Graphic_Num;

    if (Referee.UI_Change_Flag[Layer_Num][Graphic_Num] == 0)
    {
    	Referee.Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_ADD;
    	Referee.UI_Change_Flag[Layer_Num][Graphic_Num] = 1;
    }
    else
    {
    	Referee.Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_CHANGE;
    }

    Referee.Graphic_Config[Layer_Num][Graphic_Num].Type_Enum = Referee_Data_Interaction_Graphic_Type_OVAL;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Layer_Num = Layer_Num;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Color_Enum = Color;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Line_Width = Line_Width;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Start_X = Center_X;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Start_Y = Center_Y;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Details_D = Length_X;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Details_E = Length_Y;

    return (&Referee.Graphic_Config[Layer_Num][Graphic_Num]);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     设定裁判系统UI圆弧形
// 参数说明     Layer_Num												图层编号, 0~9
 	 	 	  Graphic_Num											图形编号, 0~9
 	 	 	  Color													图形颜色
 	 	 	  Line_Width											线宽
 	 	 	  Center_X												圆心x
 	 	 	  Center_Y												圆心y
 	 	 	  Angle_Start											起始角度
 	 	 	  Angle_End												终止角度
 	 	 	  Length_X												x半轴长度
 	 	 	  Length_Y												y半轴长度
// 返回参数     Struct_Referee_Data_Interaction_Graphic_Config 		对应图层指针
// 使用示例     Referee_Set_Referee_UI_Arc(Layer_Num, Graphic_Num, Color, Line_Width, Center_X, Center_Y, Angle_Start, Angle_End, Length_X, Length_Y);
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Struct_Referee_Data_Interaction_Graphic_Config *Referee_Set_Referee_UI_Arc(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Center_X, uint32_t Center_Y, uint32_t Angle_Start, uint32_t Angle_End, uint32_t Length_X, uint32_t Length_Y)
{
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[0] = '0';
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[1] = '0' + Layer_Num;
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[2] = '0' + Graphic_Num;

    if (Referee.UI_Change_Flag[Layer_Num][Graphic_Num] == 0)
    {
    	Referee.Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_ADD;
        Referee.UI_Change_Flag[Layer_Num][Graphic_Num] = 1;
    }
    else
    {
    	Referee.Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_CHANGE;
    }

    Referee.Graphic_Config[Layer_Num][Graphic_Num].Type_Enum = Referee_Data_Interaction_Graphic_Type_ARC;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Layer_Num = Layer_Num;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Color_Enum = Color;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Line_Width = Line_Width;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Start_X = Center_X;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Start_Y = Center_Y;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Details_A = Angle_Start;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Details_B = Angle_End;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Details_D = Length_X;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Details_E = Length_Y;

    return (&Referee.Graphic_Config[Layer_Num][Graphic_Num]);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     设定裁判系统UI浮点数
// 参数说明     Layer_Num												图层编号, 0~9
 	 	 	  Graphic_Num											图形编号, 0~9
 	 	 	  Color													图形颜色
 	 	 	  Line_Width											线宽
 	 	 	  Start_X												起点x
 	 	 	  Start_Y												起点y
 	 	 	  Font_Width											字体大小
 	 	 	  Float													数值
// 返回参数     Struct_Referee_Data_Interaction_Graphic_Config 		对应图层指针
// 使用示例     Referee_Set_Referee_UI_Float(Layer_Num, Graphic_Num, Color, Line_Width, Start_X, Start_Y, Font_Width, Float);
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Struct_Referee_Data_Interaction_Graphic_Config *Referee_Set_Referee_UI_Float(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Start_X, uint32_t Start_Y, uint32_t Font_Width, float Float)
{
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[0] = '0';
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[1] = '0' + Layer_Num;
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[2] = '0' + Graphic_Num;

    if (Referee.UI_Change_Flag[Layer_Num][Graphic_Num] == 0)
    {
    	Referee.Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_ADD;
    	Referee.UI_Change_Flag[Layer_Num][Graphic_Num] = 1;
    }
    else
    {
    	Referee.Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_CHANGE;
    }

    Referee.Graphic_Config[Layer_Num][Graphic_Num].Type_Enum = Referee_Data_Interaction_Graphic_Type_FLOAT;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Layer_Num = Layer_Num;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Color_Enum = Color;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Line_Width = Line_Width;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Start_X = Start_X;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Start_Y = Start_Y;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Details_A = Font_Width;
    int32_t *tmp_pointer = (int32_t *) ((uint32_t) &Referee.Graphic_Config[Layer_Num][Graphic_Num] + 11);
    *tmp_pointer = (int32_t) (Float * 1000.0f);

    return (&Referee.Graphic_Config[Layer_Num][Graphic_Num]);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     设定裁判系统UI浮点数
// 参数说明     Layer_Num												图层编号, 0~9
 	 	 	  Graphic_Num											图形编号, 0~9
 	 	 	  Color													图形颜色
 	 	 	  Line_Width											线宽
 	 	 	  Start_X												起点x
 	 	 	  Start_Y												起点y
 	 	 	  Font_Width											字体大小
 	 	 	  Integer												数值
// 返回参数     Struct_Referee_Data_Interaction_Graphic_Config 		对应图层指针
// 使用示例     Referee_Set_Referee_UI_Integer(Layer_Num, Graphic_Num, Color, Line_Width, Start_X, Start_Y, Font_Width, Integer);
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Struct_Referee_Data_Interaction_Graphic_Config *Referee_Set_Referee_UI_Integer(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Start_X, uint32_t Start_Y, uint32_t Font_Width, int32_t Integer)
{
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[0] = '0';
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[1] = '0' + Layer_Num;
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[2] = '0' + Graphic_Num;

    if (Referee.UI_Change_Flag[Layer_Num][Graphic_Num] == 0)
    {
    	Referee.Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_ADD;
    	Referee.UI_Change_Flag[Layer_Num][Graphic_Num] = 1;
    }
    else
    {
    	Referee.Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_CHANGE;
    }

    Referee.Graphic_Config[Layer_Num][Graphic_Num].Type_Enum = Referee_Data_Interaction_Graphic_Type_INTEGER;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Layer_Num = Layer_Num;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Color_Enum = Color;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Line_Width = Line_Width;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Start_X = Start_X;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Start_Y = Start_Y;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Details_A = Font_Width;
    int32_t *tmp_pointer = (int32_t *) ((uint32_t) &Referee.Graphic_Config[Layer_Num][Graphic_Num] + 11);
    *tmp_pointer = (int32_t) (Integer);

    return (&Referee.Graphic_Config[Layer_Num][Graphic_Num]);
}




/*-------------------------------------------------------------------------------------------------------------------
// 函数简介     设定裁判系统UI字符串
// 参数说明     Layer_Num												图层编号, 0~9
 	 	 	  Graphic_Num											图形编号, 0~9
 	 	 	  Color													图形颜色
 	 	 	  Line_Width											线宽
 	 	 	  Start_X												起点x
 	 	 	  Start_Y												起点y
 	 	 	  Font_Width											字体大小
 	 	 	  String_Length											字符串长度
// 返回参数     Struct_Referee_Data_Interaction_Graphic_Config 		对应图层指针
// 使用示例     Referee_Set_Referee_UI_String(Layer_Num, Graphic_Num, Color, Line_Width, Start_X, Start_Y, Font_Width, String_Length);
// 备注信息
-------------------------------------------------------------------------------------------------------------------*/
inline Struct_Referee_Data_Interaction_Graphic_Config *Referee_Set_Referee_UI_String(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Start_X, uint32_t Start_Y, uint32_t Font_Width, uint32_t String_Length)
{
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[0] = '0';
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[1] = '0' + Layer_Num;
	Referee.Graphic_Config[Layer_Num][Graphic_Num].Index[2] = '0' + Graphic_Num;

    if (Referee.UI_Change_Flag[Layer_Num][Graphic_Num] == 0)
    {
    	Referee.Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_ADD;
    	Referee.UI_Change_Flag[Layer_Num][Graphic_Num] = 1;
    }
    else
    {
    	Referee.Graphic_Config[Layer_Num][Graphic_Num].Operation_Enum = Referee_Data_Interaction_Graphic_Operation_CHANGE;
    }

    Referee.Graphic_Config[Layer_Num][Graphic_Num].Type_Enum = Referee_Data_Interaction_Graphic_Type_STRING;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Layer_Num = Layer_Num;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Color_Enum = Color;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Line_Width = Line_Width;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Start_X = Start_X;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Start_Y = Start_Y;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Details_A = Font_Width;
    Referee.Graphic_Config[Layer_Num][Graphic_Num].Details_B = String_Length;

    return (&Referee.Graphic_Config[Layer_Num][Graphic_Num]);
}
