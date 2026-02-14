#ifndef DEVICE_REFEREE_INC_REFEREE_H_
#define DEVICE_REFEREE_INC_REFEREE_H_


/*********************************************************************参数*********************************************************************/

#include "main.h"


#define MY_REFEREE_BUF_SIZE 512

extern uint8_t My_Referee_Rx_Buf[MY_REFEREE_BUF_SIZE];



//裁判系统状态
typedef enum
{
    Referee_Status_DISABLE 	= 0,
    Referee_Status_ENABLE 	= 1,
} Enum_Referee_Status;


//各种标签,场地,相关设施激活与存活状态
typedef enum
{
    Referee_Data_Status_DISABLE 	= 0,
    Referee_Data_Status_ENABLE 		= 1,
} Enum_Referee_Data_Status;


//裁判系统命令码类型
typedef enum
{
    //游戏全局相关命令码
    GAME_STATUS = 0x0001,               								//0x0001 比赛状态数据
    GAME_RESULT = 0x0002,               								//0x0002 比赛结果数据
    GAME_ROBOT_HP = 0x0003,             								//0x0003 机器人血量数据

    //己方场地事件相关命令码
    EVENT_SELF_DATA = 0x0101,           								//0x0101 场地事件数据
    EVENT_SELF_SUPPLY = 0x0102,         								//0x0102 场地补给数据
    EVENT_SELF_REFEREE_WARNING = 0x0104,								//0x0104 裁判警告数据
    EVENT_SELF_DART_STATUS = 0x0105,    								//0x0105 飞镖发射相关数据

    //机器人自身状态相关命令码
    ROBOT_STATUS = 0x0201,              								//0x0201 机器人性能体系数据
    ROBOT_POWER_HEAT = 0x0202,          								//0x0202 底盘缓冲能量和射击热量数据
    ROBOT_POSITION = 0x0203,            								//0x0203 机器人位置数据
    ROBOT_BUFF = 0x0204,                								//0x0204 机器人增益和底盘能量数据
    ROBOT_AERIAL_STATUS = 0x0205,       								//0x0205 空中机器人专属状态数据
    ROBOT_DAMAGE = 0x0206,              								//0x0206 机器人伤害状态数据
    ROBOT_BOOSTER = 0x0207,             								//0x0207 实时射击数据
    ROBOT_REMAINING_AMMO = 0x0208,      								//0x0208 允许发弹量数据
    ROBOT_RFID = 0x0209,               									//0x0209 RFID模块状态数据
    ROBOT_DART_COMMAND = 0x020A,       									//0x020A 飞镖选手端指令数据
    ROBOT_SENTRY_LOCATION = 0x020B,     								//0x020B 地面机器人位置数据
    ROBOT_RADAR_MARK = 0x020C,          								//0x020C 雷达标记进度数据
    ROBOT_SENTRY_DECISION = 0x020D,     								//0x020D 哨兵自主决策信息同步
    ROBOT_RADAR_DECISION = 0x020E,      								//0x020E 雷达自主决策信息同步

    //交互相关命令码
    INTERACTION = 0x0301,               								//0x0301 机器人交互数据
    INTERACTION_ROBOT_RECEIVE_CUSTOM_CONTROLLER = 0x0302, 				//0x0302 自定义控制器与机器人交互数据
    INTERACTION_ROBOT_RECEIVE_CLIENT_MINIMAP = 0x0303,    				//0x0303 选手端小地图交互数据
    INTERACTION_ROBOT_RECEIVE_CLIENT_REMOTE_CONTROL = 0x0304, 			//0x0304 键鼠遥控数据
    INTERACTION_CLIENT_RECEIVE_RADAR = 0x0305,            				//0x0305 选手端小地图接收雷达数据
    INTERACTION_CLIENT_RECEIVE_CUSTOM_CONTROLLER = 0x0306, 				//0x0306 自定义控制器与选手端交互数据
    INTERACTION_CLIENT_RECEIVE_SENTRY_SEMIAUTOMATIC_MINIMAP = 0x0307, 	//0x0307 选手端小地图接收路径数据
    INTERACTION_CLIENT_RECEIVE_ROBOT_MINIMAP = 0x0308,    				//0x0308 选手端小地图接收机器人数据

    //图传链路专属命令码
	IMAGE_CHANNEL_SET = 0x0F01,          								//0x0F01 设置图传出图信道
	IMAGE_CHANNEL_QUERY = 0x0F02,        								//0x0F02 查询图传出图信道

	//雷达无线链路命令码
	RADAR_ENEMY_POS = 0x0A01,            								//0x0A01 敌方位置
	RADAR_ENEMY_HP = 0x0A02,             								//0x0A02 敌方血量
	RADAR_ENEMY_AMMO = 0x0A03,           								//0x0A03 敌方发弹量
	RADAR_ENEMY_STATE = 0x0A04,          								//0x0A04 敌方宏观状态
	RADAR_ENEMY_BUFF = 0x0A05,           								//0x0A05 敌方增益
	RADAR_ENEMY_KEY = 0x0A06,            								//0x0A06 敌方干扰波密钥
} Referee_Command_ID_Struct;

//typedef struct
//{
//    //游戏全局相关命令码
//    uint16_t GAME_STATUS;               								//0x0001 比赛状态数据
//    uint16_t GAME_RESULT;               								//0x0002 比赛结果数据
//    uint16_t GAME_ROBOT_HP;             								//0x0003 机器人血量数据
//
//    //己方场地事件相关命令码
//    uint16_t EVENT_SELF_DATA;           								//0x0101 场地事件数据
//    uint16_t EVENT_SELF_SUPPLY;         								//0x0102 场地补给数据
//    uint16_t EVENT_SELF_REFEREE_WARNING;								//0x0104 裁判警告数据
//    uint16_t EVENT_SELF_DART_STATUS;    								//0x0105 飞镖发射相关数据
//
//    //机器人自身状态相关命令码
//    uint16_t ROBOT_STATUS;              								//0x0201 机器人性能体系数据
//    uint16_t ROBOT_POWER_HEAT;          								//0x0202 底盘缓冲能量和射击热量数据
//    uint16_t ROBOT_POSITION;            								//0x0203 机器人位置数据
//    uint16_t ROBOT_BUFF;                								//0x0204 机器人增益和底盘能量数据
//    uint16_t ROBOT_AERIAL_STATUS;       								//0x0205 空中机器人专属状态数据
//    uint16_t ROBOT_DAMAGE;              								//0x0206 机器人伤害状态数据
//    uint16_t ROBOT_BOOSTER;             								//0x0207 实时射击数据
//    uint16_t ROBOT_REMAINING_AMMO;      								//0x0208 允许发弹量数据
//    uint16_t ROBOT_RFID;               									//0x0209 RFID模块状态数据
//    uint16_t ROBOT_DART_COMMAND;       									//0x020A 飞镖选手端指令数据
//    uint16_t ROBOT_SENTRY_LOCATION;     								//0x020B 地面机器人位置数据
//    uint16_t ROBOT_RADAR_MARK;          								//0x020C 雷达标记进度数据
//    uint16_t ROBOT_SENTRY_DECISION;     								//0x020D 哨兵自主决策信息同步
//    uint16_t ROBOT_RADAR_DECISION;      								//0x020E 雷达自主决策信息同步
//
//    //交互相关命令码
//    uint16_t INTERACTION;               								//0x0301 机器人交互数据
//    uint16_t INTERACTION_ROBOT_RECEIVE_CUSTOM_CONTROLLER; 				//0x0302 自定义控制器与机器人交互数据
//    uint16_t INTERACTION_ROBOT_RECEIVE_CLIENT_MINIMAP;    				//0x0303 选手端小地图交互数据
//    uint16_t INTERACTION_ROBOT_RECEIVE_CLIENT_REMOTE_CONTROL; 			//0x0304 键鼠遥控数据
//    uint16_t INTERACTION_CLIENT_RECEIVE_RADAR;            				//0x0305 选手端小地图接收雷达数据
//    uint16_t INTERACTION_CLIENT_RECEIVE_CUSTOM_CONTROLLER; 				//0x0306 自定义控制器与选手端交互数据
//    uint16_t INTERACTION_CLIENT_RECEIVE_SENTRY_SEMIAUTOMATIC_MINIMAP; 	//0x0307 选手端小地图接收路径数据
//    uint16_t INTERACTION_CLIENT_RECEIVE_ROBOT_MINIMAP;    				//0x0308 选手端小地图接收机器人数据
//
//    //图传链路专属命令码
//	uint16_t IMAGE_CHANNEL_SET;          								//0x0F01 设置图传出图信道
//	uint16_t IMAGE_CHANNEL_QUERY;        								//0x0F02 查询图传出图信道
//
//	//雷达无线链路命令码
//	uint16_t RADAR_ENEMY_POS;            								//0x0A01 敌方位置
//	uint16_t RADAR_ENEMY_HP;             								//0x0A02 敌方血量
//	uint16_t RADAR_ENEMY_AMMO;           								//0x0A03 敌方发弹量
//	uint16_t RADAR_ENEMY_STATE;          								//0x0A04 敌方宏观状态
//	uint16_t RADAR_ENEMY_BUFF;           								//0x0A05 敌方增益
//	uint16_t RADAR_ENEMY_KEY;            								//0x0A06 敌方干扰波密钥
//} Referee_Command_ID_Struct;


//裁判系统 0x0301子命令码类型
typedef enum
{
    //选手端UI交互子命令码
    UI_LAYER_DELETE = 0x0100,        						//0x0100 选手端删除图层
    UI_GRAPHIC_1 = 0x0101,           						//0x0101 选手端绘制1个图形
    UI_GRAPHIC_2 = 0x0102,           						//0x0102 选手端绘制2个图形
    UI_GRAPHIC_5 = 0x0103,           						//0x0103 选手端绘制5个图形
    UI_GRAPHIC_7 = 0x0104,           						//0x0104 选手端绘制7个图形
    UI_GRAPHIC_STRING = 0x0110,      						//0x0110 选手端绘制字符图形

    //哨兵/雷达自主决策子命令码
    SENTRY = 0x0120,                 						//0x0120 哨兵自主决策指令,复活/兑换弹药/远程补血等
    RADAR = 0x0121,                  						//0x0121 雷达自主决策指令,触发双倍易伤/更新密钥/验证密钥
} Referee_Interaction_Command_ID_Struct;

//typedef struct
//{
//    //选手端UI交互子命令码
//    uint16_t UI_LAYER_DELETE;        						//0x0100 选手端删除图层
//    uint16_t UI_GRAPHIC_1;           						//0x0101 选手端绘制1个图形
//    uint16_t UI_GRAPHIC_2;           						//0x0102 选手端绘制2个图形
//    uint16_t UI_GRAPHIC_5;           						//0x0103 选手端绘制5个图形
//    uint16_t UI_GRAPHIC_7;           						//0x0104 选手端绘制7个图形
//    uint16_t UI_GRAPHIC_STRING;      						//0x0110 选手端绘制字符图形
//
//    //哨兵/雷达自主决策子命令码
//    uint16_t SENTRY;                 						//0x0120 哨兵自主决策指令,复活/兑换弹药/远程补血等
//    uint16_t RADAR;                  						//0x0121 雷达自主决策指令,触发双倍易伤/更新密钥/验证密钥
//} Referee_Interaction_Command_ID_Struct;


//通用单方机器人ID无阵营区分
typedef enum
{
	Robot_NULL_ID 	= 0,		//无效ID
	HERO_1 			= 1,		//1号英雄机器人
	ENGINEER_2 		= 2,		//2号工程机器人
	INFANTRY_3 		= 3,		//3号步兵机器人
	INFANTRY_4 		= 4,		//3号步兵机器人
	INFANTRY_5 		= 5,		//3号步兵机器人
	AERIAL_6 		= 6,		//空中机器人
	SENTRY_7 		= 7,		//哨兵机器人
	DART_8 			= 8,		//飞镖机器人
	RADAR_9 		= 9,		//雷达机器人
	BASE_10 		= 10,		//基地
	OUTPOST_11 		= 11,		//前哨站
} Referee_Data_Robot_ID_Struct;

//typedef struct
//{
//	uint8_t NULL_ID;		//无效ID
//	uint8_t HERO_1;			//1号英雄机器人
//	uint8_t ENGINEER_2;		//2号工程机器人
//	uint8_t INFANTRY_3;		//3号步兵机器人
//	uint8_t INFANTRY_4;		//3号步兵机器人
//	uint8_t INFANTRY_5;		//3号步兵机器人
//	uint8_t AERIAL_6;		//空中机器人
//	uint8_t SENTRY_7;		//哨兵机器人
//	uint8_t DART_8;			//飞镖机器人
//	uint8_t RADAR_9;		//雷达机器人
//	uint8_t BASE_10;		//基地
//	uint8_t OUTPOST_11;		//前哨站
//} Referee_Data_Robot_ID_Struct;

//通用双方机器人ID结构体区分红蓝阵营
typedef enum
{
	Robots_NULL_ID		= 0,		//无效ID

	//红方机器人ID
	RED_HERO_1 			= 1,		//1 红方1号英雄机器人
	RED_ENGINEER_2 		= 2,     	//2 红方2号工程机器人
	RED_INFANTRY_3 		= 3,     	//3 红方3号步兵机器人
	RED_INFANTRY_4 		= 4,     	//4 红方4号步兵机器人
	RED_INFANTRY_5 		= 5,     	//5 红方5号步兵机器人
	RED_AERIAL_6 		= 6,       	//6 红方6号空中机器人
	RED_SENTRY_7 		= 7,       	//7 红方7号哨兵机器人
	RED_DART_8 			= 8,        //8 红方8号飞镖机器人
	RED_RADAR_9 		= 9,        //9 红方9号雷达机器人
	RED_BASE_10 		= 10,       //10 红方基地
	RED_OUTPOST_11 		= 11,     	//11 红方前哨站

	//蓝方机器人ID
	BLUE_HERO_1 		= 101,      //101 蓝方1号英雄机器人
	BLUE_ENGINEER_2 	= 102,    	//102 蓝方2号工程机器人
	BLUE_INFANTRY_3 	= 103,    	//103 蓝方3号步兵机器人
	BLUE_INFANTRY_4 	= 104,    	//104 蓝方4号步兵机器人
	BLUE_INFANTRY_5 	= 105,    	//105 蓝方5号步兵机器人
	BLUE_AERIAL_6 		= 106,      //106 蓝方6号空中机器人
	BLUE_SENTRY_7 		= 107,      //107 蓝方7号哨兵机器人
	BLUE_DART_8 		= 108,      //108 蓝方8号飞镖机器人
	BLUE_RADAR_9 		= 109,      //109 蓝方9号雷达机器人
	BLUE_BASE_10 		= 110,      //110 蓝方基地
	BLUE_OUTPOST_11 	= 111,    	//111 蓝方前哨站
} Referee_Data_Robots_ID_Struct;

//typedef struct
//{
//	uint8_t NULL_ID;			//无效ID
//
//	//红方机器人ID
//	uint8_t RED_HERO_1;			//1 红方1号英雄机器人
//	uint8_t RED_ENGINEER_2;     //2 红方2号工程机器人
//	uint8_t RED_INFANTRY_3;     //3 红方3号步兵机器人
//	uint8_t RED_INFANTRY_4;     //4 红方4号步兵机器人
//	uint8_t RED_INFANTRY_5;     //5 红方5号步兵机器人
//	uint8_t RED_AERIAL_6;       //6 红方6号空中机器人
//	uint8_t RED_SENTRY_7;       //7 红方7号哨兵机器人
//	uint8_t RED_DART_8;         //8 红方8号飞镖机器人
//	uint8_t RED_RADAR_9;        //9 红方9号雷达机器人
//	uint8_t RED_BASE_10;        //10 红方基地
//	uint8_t RED_OUTPOST_11;     //11 红方前哨站
//
//	//蓝方机器人ID
//	uint8_t BLUE_HERO_1;        //101 蓝方1号英雄机器人
//	uint8_t BLUE_ENGINEER_2;    //102 蓝方2号工程机器人
//	uint8_t BLUE_INFANTRY_3;    //103 蓝方3号步兵机器人
//	uint8_t BLUE_INFANTRY_4;    //104 蓝方4号步兵机器人
//	uint8_t BLUE_INFANTRY_5;    //105 蓝方5号步兵机器人
//	uint8_t BLUE_AERIAL_6;      //106 蓝方6号空中机器人
//	uint8_t BLUE_SENTRY_7;      //107 蓝方7号哨兵机器人
//	uint8_t BLUE_DART_8;        //108 蓝方8号飞镖机器人
//	uint8_t BLUE_RADAR_9;       //109 蓝方9号雷达机器人
//	uint8_t BLUE_BASE_10;       //110 蓝方基地
//	uint8_t BLUE_OUTPOST_11;    //111 蓝方前哨站
//} Referee_Data_Robots_ID_Struct;

//通用双方机器人选手端ID结构体区分红蓝阵营
typedef enum
{
	Robots_Client_NULL_ID 		= 0x0000,		//无效ID

	//红方选手端ID
	Client_RED_HERO_1 			= 0x0101,      //0x0101 红方1号英雄机器人选手端
	Client_RED_ENGINEER_2 		= 0x0102,      //0x0102 红方2号工程机器人选手端
	Client_RED_INFANTRY_3 		= 0x0103,      //0x0103 红方3号步兵机器人选手端
	Client_RED_INFANTRY_4 		= 0x0104,      //0x0104 红方4号步兵机器人选手端
	Client_RED_INFANTRY_5 		= 0x0105,      //0x0105 红方5号步兵机器人选手端
	Client_RED_AERIAL_6 		= 0x0106,      //0x0106 红方6号空中机器人选手端

	//蓝方选手端ID
	Client_BLUE_HERO_1 			= 0x0165,      //0x0165 蓝方1号英雄机器人选手端
	Client_BLUE_ENGINEER_2 		= 0x0166,      //0x0166 蓝方2号工程机器人选手端
	Client_BLUE_INFANTRY_3 		= 0x0167,      //0x0167 蓝方3号步兵机器人选手端
	Client_BLUE_INFANTRY_4 		= 0x0168,      //0x0168 蓝方4号步兵机器人选手端
	Client_BLUE_INFANTRY_5 		= 0x0169,      //0x0169 蓝方5号步兵机器人选手端
	Client_BLUE_AERIAL_6 		= 0x016A,      //0x016A 蓝方6号空中机器人选手端

	//特殊ID
	SERVER 				= 0x8080,      //0x8080 裁判系统服务器ID,用于哨兵/雷达自主决策指令
} Referee_Data_Robots_Client_ID_Struct;

//typedef struct
//{
//	uint16_t NULL_ID;			//无效ID
//
//	//红方选手端ID
//	uint16_t RED_HERO_1;            //0x0101 红方1号英雄机器人选手端
//	uint16_t RED_ENGINEER_2;        //0x0102 红方2号工程机器人选手端
//	uint16_t RED_INFANTRY_3;        //0x0103 红方3号步兵机器人选手端
//	uint16_t RED_INFANTRY_4;        //0x0104 红方4号步兵机器人选手端
//	uint16_t RED_INFANTRY_5;        //0x0105 红方5号步兵机器人选手端
//	uint16_t RED_AERIAL_6;          //0x0106 红方6号空中机器人选手端
//
//	//蓝方选手端ID
//	uint16_t BLUE_HERO_1;           //0x0165 蓝方1号英雄机器人选手端
//	uint16_t BLUE_ENGINEER_2;       //0x0166 蓝方2号工程机器人选手端
//	uint16_t BLUE_INFANTRY_3;       //0x0167 蓝方3号步兵机器人选手端
//	uint16_t BLUE_INFANTRY_4;       //0x0168 蓝方4号步兵机器人选手端
//	uint16_t BLUE_INFANTRY_5;       //0x0169 蓝方5号步兵机器人选手端
//	uint16_t BLUE_AERIAL_6;         //0x016A 蓝方6号空中机器人选手端
//
//	//特殊ID
//	uint16_t SERVER;                //0x8080 裁判系统服务器ID,用于哨兵/雷达自主决策指令
//} Referee_Data_Robots_Client_ID_Struct;



//比赛类型
typedef enum
{
    Referee_Game_Status_Type_RMUC 		= 1,
    Referee_Game_Status_Type_SINGLE 	= 2,
    Referee_Game_Status_Type_ICRA		= 3,
    Referee_Game_Status_Type_RMUL_3V3	= 4,
    Referee_Game_Status_Type_RMUL_1V1 	= 5,
} Enum_Referee_Game_Status_Type;


//比赛阶段
typedef enum
{
    Referee_Game_Status_Stage_NOT_STARTED 		= 0,
    Referee_Game_Status_Stage_READY 			= 1,
    Referee_Game_Status_Stage_15s_SELF_TESTING 	= 2,
    Referee_Game_Status_Stage_5S_COUNTDOWN 		= 3,
    Referee_Game_Status_Stage_BATTLE 			= 4,
    Referee_Game_Status_Stage_SETTLEMENT 		= 5,
} Enum_Referee_Game_Status_Stage;


//比赛结果
typedef enum
{
    Referee_Game_Result_DRAW 		= 0,
    Referee_Game_Result_RED_WIN 	= 1,
    Referee_Game_Result_BLUE_WIN 	= 2,
} Enum_Referee_Game_Result;


//飞镖命中目标
typedef enum
{
    Referee_Dart_Hit_Target_NULL 			= 0,
    Referee_Dart_Hit_Target_OUTPOST 		= 1,
    Referee_Dart_Hit_Target_BASE_STATIC 	= 2,
    Referee_Dart_Hit_Target_BASE_RANDOM 	= 3,
} Enum_Referee_Dart_Hit_Target;


//补给站状态
typedef enum
{
    Referee_Data_Event_Supply_Status_CLOSED 	= 0,
    Referee_Data_Event_Supply_Status_READY 		= 1,
    Referee_Data_Event_Supply_Status_DROPPING 	= 2,
} Enum_Referee_Data_Event_Supply_Status;


//补给站提供子弹数量
typedef enum
{
    Referee_Data_Event_Supply_Ammo_Number_50 	= 50,
    Referee_Data_Event_Supply_Ammo_Number_100 	= 100,
    Referee_Data_Event_Supply_Ammo_Number_150 	= 150,
    Referee_Data_Event_Supply_Ammo_Number_200 	= 200,
} Enum_Referee_Data_Event_Supply_Ammo_Number;


//裁判警告等级
typedef enum
{
    Referee_Data_Referee_Warning_Level_BOTH_YELLOW 	= 1,
    Referee_Data_Referee_Warning_Level_YELLOW 		= 2,
    Referee_Data_Referee_Warning_Level_RED 			= 3,
    Referee_Data_Referee_Warning_Level_FAIL 		= 4,
} Enum_Referee_Data_Event_Referee_Warning_Level;


//裁判警告等级
typedef enum
{
    Referee_Data_Event_Aerial_Status_COOLING 	= 0,
    Referee_Data_Event_Aerial_Status_READY		= 1,
    Referee_Data_Event_Aerial_Status_EXECUTING	= 2,
} Enum_Referee_Data_Event_Aerial_Status;


//伤害类型
typedef enum
{
    Referee_Data_Robot_Damage_Type_ARMOR_ATTACKED 	= 0,
    Referee_Data_Robot_Damage_Type_MODULE_OFFLINE	= 1,
    Referee_Data_Robot_Damage_Type_BOOSTER_SPEED	= 2,
    Referee_Data_Robot_Damage_Type_BOOSTER_HEAT		= 3,
    Referee_Data_Robot_Damage_Type_CHASSIS_POWER	= 4,
    Referee_Data_Robot_Damage_Type_ARMOR_COLLISION	= 5,
} Enum_Referee_Data_Event_Robot_Damage_Type;


//子弹类型
typedef enum
{
    Referee_Data_Robot_Ammo_Type_BOOSTER_17MM = 1,
    Referee_Data_Robot_Ammo_Type_BOOSTER_42mm = 2,
} Enum_Referee_Data_Robot_Ammo_Type;


//发射机构类型
typedef enum
{
    Referee_Data_Robot_Booster_Type_BOOSTER_17MM_1 = 1,
    Referee_Data_Robot_Booster_Type_BOOSTER_17MM_2,
    Referee_Data_Robot_Booster_Type_BOOSTER_42mm,
} Enum_Referee_Data_Robot_Booster_Type;


//飞镖发射口状态
typedef enum
{
    Referee_Data_Robot_Dart_Command_Status_OPEN 		= 0,
    Referee_Data_Robot_Dart_Command_Status_CLOSED		= 1,
    Referee_Data_Robot_Dart_Command_Status_EXECUTING	= 2,
} Enum_Referee_Data_Robot_Dart_Command_Status;


//飞镖击打目标
typedef enum
{
    Referee_Data_Robot_Dart_Command_Target_OUTPOST 	= 0,
    Referee_Data_Robot_Dart_Command_Target_BASE		= 1,
} Enum_Referee_Data_Robot_Dart_Command_Target;


//图形操作交互信息
typedef enum
{
    Referee_Data_Interaction_Layer_Delete_Operation_NULL 		= 0,
    Referee_Data_Interaction_Layer_Delete_Operation_CLEAR_ONE	= 1,
    Referee_Data_Interaction_Layer_Delete_Operation_CLEAR_ALL	= 2,
} Enum_Referee_Data_Interaction_Layer_Delete_Operation;


//图形操作
typedef enum
{
    Referee_Data_Interaction_Graphic_Operation_NULL 	= 0,
    Referee_Data_Interaction_Graphic_Operation_ADD		= 1,
    Referee_Data_Interaction_Graphic_Operation_CHANGE	= 2,
    Referee_Data_Interaction_Graphic_Operation_DELETE	= 3,
} Enum_Referee_Data_Interaction_Graphic_Operation;


//图形类型
typedef enum
{
    Referee_Data_Interaction_Graphic_Type_LINE 		= 0,
    Referee_Data_Interaction_Graphic_Type_RECTANGLE	= 1,
    Referee_Data_Interaction_Graphic_Type_CIRCLE	= 2,
    Referee_Data_Interaction_Graphic_Type_OVAL		= 3,
    Referee_Data_Interaction_Graphic_Type_ARC		= 4,
    Referee_Data_Interaction_Graphic_Type_FLOAT		= 5,
    Referee_Data_Interaction_Graphic_Type_INTEGER	= 6,
    Referee_Data_Interaction_Graphic_Type_STRING	= 7,
} Enum_Referee_Data_Interaction_Graphic_Type;


//图形颜色
typedef enum
{
    Referee_Data_Interaction_Graphic_Color_MAIN 	= 0,
    Referee_Data_Interaction_Graphic_Color_YELLOW	= 1,
    Referee_Data_Interaction_Graphic_Color_GREEN	= 2,
    Referee_Data_Interaction_Graphic_Color_ORANGE	= 3,
    Referee_Data_Interaction_Graphic_Color_PURPLE	= 4,
    Referee_Data_Interaction_Graphic_Color_PINK		= 5,
    Referee_Data_Interaction_Graphic_Color_CYAN		= 6,
    Referee_Data_Interaction_Graphic_Color_BLACK	= 7,
    Referee_Data_Interaction_Graphic_Color_WHITE	= 8,
} Enum_Referee_Data_Interaction_Graphic_Color;


//图形操作交互信息
typedef enum
{
    Referee_Data_Interaction_Semiautomatic_Command_ATTACK 	= 1,
    Referee_Data_Interaction_Semiautomatic_Command_DEFENCE	= 2,
    Referee_Data_Interaction_Semiautomatic_Command_MOVE		= 3,
} Enum_Referee_Data_Interaction_Semiautomatic_Command;


//图形配置结构体
typedef struct
{
    uint8_t Index[3];
    uint32_t Operation_Enum : 3;
    uint32_t Type_Enum : 3;
    uint32_t Layer_Num : 4;
    uint32_t Color_Enum : 4;
    uint32_t Details_A : 9;
    uint32_t Details_B : 9;
    uint32_t Line_Width : 10;
    uint32_t Start_X : 11;
    uint32_t Start_Y : 11;
    uint32_t Details_C : 10;
    uint32_t Details_D : 11;
    uint32_t Details_E : 11;
} __attribute__((packed)) Struct_Referee_Data_Interaction_Graphic_Config;


//裁判系统源数据
typedef struct
{
    uint8_t Referee_Frame_Header;
    uint16_t Data_Length;
    uint8_t Referee_Sequence;
    uint8_t CRC_8;
    Referee_Command_ID_Struct Referee_Command_ID;
    uint8_t Data[121];
} __attribute__((packed)) Struct_Referee_UART_Data;


//裁判系统经过处理的数据, 0x0001比赛状态
typedef struct
{
    uint8_t Type_Enum : 4;
    uint8_t Stage_Enum : 4;
    uint16_t Remaining_Time;
    uint64_t Timestamp;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Game_Status;


//裁判系统经过处理的数据, 0x0002比赛结果, 比赛结束后发送
typedef struct
{
    Enum_Referee_Game_Result Result;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Game_Result;


//裁判系统经过处理的数据, 0x0003机器人血量
typedef struct
{
    uint16_t Red_Hero_1;
    uint16_t Red_Engineer_2;
    uint16_t Red_Infantry_3;
    uint16_t Red_Infantry_4;
    uint16_t Red_Infantry_5;
    uint16_t Red_Sentry_7;
    uint16_t Red_Outpost_11;
    uint16_t Red_Base_10;
    uint16_t Blue_Hero_1;
    uint16_t Blue_Engineer_2;
    uint16_t Blue_Infantry_3;
    uint16_t Blue_Infantry_4;
    uint16_t Blue_Infantry_5;
    uint16_t Blue_Sentry_7;
    uint16_t Blue_Outpost_11;
    uint16_t Blue_Base_10;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Game_Robot_HP;


//裁判系统经过处理的数据, 0x0101场地事件
typedef struct
{
    uint32_t Supply_Front_Status_Enum : 1;
    uint32_t Supply_Inner_Status_Enum : 1;
    uint32_t Supply_Status_Enum : 1;
    uint32_t Power_Rune_Status_Enum : 1;
    uint32_t Power_Rune_Small_Status_Enum : 1;
    uint32_t Power_Rune_Big_Status_Enum : 1;
    uint32_t Highland_2_Status_Enum : 2;
    uint32_t Highland_3_Status_Enum : 2;
    uint32_t Highland_4_Status_Enum : 2;
    uint32_t Base_Virtual_Shield_Percent : 7;
    uint32_t Enemy_Dart_Hit_Time : 9;
    uint32_t Enemy_Dart_Hit_Target_Enum : 2;
    uint32_t Middle_Buff_Status_Enum : 2;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Event_Self_Data;


//裁判系统经过处理的数据, 0x0102补给站状态, 补给请求后对应发送
typedef struct
{
    uint8_t Reserved;
    Referee_Command_ID_Struct Robot;
    Enum_Referee_Data_Event_Supply_Status Status;
    Enum_Referee_Data_Event_Supply_Ammo_Number Ammo_Number;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Event_Self_Supply;


//裁判系统经过处理的数据, 0x0104裁判警告信息, 判罚发生后发送
typedef struct
{
    Enum_Referee_Data_Event_Referee_Warning_Level Level;
    Referee_Data_Robot_ID_Struct Robot_ID;
    uint8_t Count;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Event_Referee_Warning;


//裁判系统经过处理的数据, 0x0105飞镖15s倒计时
typedef struct
{
    uint8_t Dart_Remaining_Time;
    uint16_t Dart_Hit_Target_Enum_Last : 2;
    uint16_t Dart_Hit_Target_Count : 3;
    uint16_t Dart_Hit_Target_Enum_Now : 2;
    uint16_t Reserved : 9;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Event_Dart_Status;


//裁判系统经过处理的数据, 0x0201机器人状态
typedef struct
{
	Referee_Data_Robots_ID_Struct Robot_ID;
    uint8_t Level;
    uint16_t HP;
    uint16_t HP_Max;
    uint16_t Booster_Heat_CD;
    uint16_t Booster_Heat_Max;
    uint16_t Chassis_Power_Max;
    uint8_t PM01_Gimbal_Status_Enum : 1;
    uint8_t PM01_Chassis_Status_Enum : 1;
    uint8_t PM01_Booster_Status_Enum : 1;
    uint8_t Reserved : 5;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Robot_Status;


//裁判系统经过处理的数据, 0x0202当前机器人实时功率热量, 电压mV, 电流mA
typedef struct
{
    uint16_t Chassis_Voltage;
    uint16_t Chassis_Current;
    float Chassis_Power;
    uint16_t Chassis_Energy_Buffer;
    uint16_t Booster_17mm_1_Heat;
    uint16_t Booster_17mm_2_Heat;
    uint16_t Booster_42mm_Heat;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Robot_Power_Heat;


//裁判系统经过处理的数据, 0x0203当前机器人实时位置
typedef struct
{
    float Location_X;
    float Location_Y;
    float Location_Yaw;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Robot_Position;


//裁判系统经过处理的数据, 0x0204当前机器人增益
typedef struct
{
    uint8_t HP_Buff_Percent;
    uint8_t Booster_Heat_CD_Buff_Value;
    uint8_t Defend_Buff_Percent;
    uint8_t Defend_Debuff_Percent;
    uint16_t Damage_Buff_Percent;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Robot_Buff;


//裁判系统经过处理的数据, 0x0205无人机可攻击时间
typedef struct
{
    Enum_Referee_Data_Event_Aerial_Status Aerial_Status;
    uint8_t Remaining_Time;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Robot_Aerial_Status;


//裁判系统经过处理的数据, 0x0206伤害情况, 伤害发生后发送
typedef struct
{
    uint8_t Armor_ID : 4;
    uint8_t Type_Enum : 4;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Robot_Damage;


//裁判系统经过处理的数据, 0x0207子弹信息, 射击发生后发送
typedef struct
{
    Enum_Referee_Data_Robot_Ammo_Type Ammo_Type;
    Enum_Referee_Data_Robot_Booster_Type Booster_Type;
    uint8_t Frequency;
    float Speed;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Robot_Booster;


//裁判系统经过处理的数据, 0x0208子弹剩余信息
typedef struct
{
    uint16_t Booster_17mm;
    uint16_t Booster_42mm;
    uint16_t Money;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Robot_Remaining_Ammo;


//裁判系统经过处理的数据, 0x0209RFID状态信息
typedef struct
{
    uint32_t Base_Status_Enum : 1;
    uint32_t Highland_2_Self_Status_Enum : 1;
    uint32_t Highland_2_Enemy_Status_Enum : 1;
    uint32_t Highland_3_Self_Status_Enum : 1;
    uint32_t Highland_3_Enemy_Status_Enum : 1;
    uint32_t Highland_4_Self_Status_Enum : 1;
    uint32_t Highland_4_Enemy_Status_Enum : 1;
    uint32_t Energy_Status_Enum : 1;
    uint32_t Flyover_1_Self_Status_Enum : 1;
    uint32_t Flyover_2_Self_Status_Enum : 1;
    uint32_t Flyover_1_Enemy_Status_Enum : 1;
    uint32_t Flyover_2_Enemy_Status_Enum : 1;
    uint32_t Outpost_Status_Enum : 1;
    uint32_t HP_Status_Enum : 1;
    uint32_t Sentry_Self_Status_Enum : 1;
    uint32_t Sentry_Enemy_Status_Enum : 1;
    uint32_t Engineer_Self_Status_Enum : 1;
    uint32_t Engineer_Enemy_Status_Enum : 1;
    uint32_t Engineer_Exchange_Status_Enum : 1;
    uint32_t Middle_Status_Enum : 1;
    uint32_t Reserved : 12;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Robot_RFID;


//裁判系统经过处理的数据, 0x020a飞镖状态
typedef struct
{
    Enum_Referee_Data_Robot_Dart_Command_Status Status;
    uint8_t Reserved;
    uint16_t Switch_Remaining_Time;
    uint16_t Launch_Remaining_Time;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Robot_Dart_Command;


//裁判系统经过处理的数据, 0x020b哨兵获取己方位置信息
typedef struct
{
    float Hero_1_X;
    float Hero_1_Y;
    float Engineer_2_X;
    float Engineer_2_Y;
    float Infantry_3_X;
    float Infantry_3_Y;
    float Infantry_4_X;
    float Infantry_4_Y;
    float Infantry_5_X;
    float Infantry_5_Y;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Robot_Sentry_Location;


//裁判系统经过处理的数据, 0x020c雷达标记进度
typedef struct
{
    uint8_t Hero_1_Mark;
    uint8_t Engineer_2_Mark;
    uint8_t Infantry_3_Mark;
    uint8_t Infantry_4_Mark;
    uint8_t Infantry_5_Mark;
    uint8_t Sentry_7_Mark;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Robot_Radar_Mark;


//裁判系统经过处理的数据, 0x020d哨兵决策信息
typedef struct
{
    uint32_t Ammo_Exchange_Number : 11;
    uint32_t Ammo_Exchange_Time : 4;
    uint32_t HP_Exchange_Time : 4;
    uint32_t Reserved : 13;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Robot_Sentry_Decision;


//裁判系统经过处理的数据, 0x020e雷达决策信息
typedef struct
{
    uint8_t Double_Damage_Chance : 2;
    uint8_t Double_Damage_Enemy_Status_Enum : 1;
    uint8_t Reserved : 5;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Robot_Radar_Decision;


//裁判系统接收的数据, 0x0301机器人间交互信息, 用户自主发送
typedef struct
{
	uint16_t Header;
	Referee_Data_Robots_ID_Struct Sender;
	Referee_Data_Robots_ID_Struct Receiver;
	uint8_t Data[113];
	uint16_t CRC_16;
}__attribute__((packed)) Struct_Referee_Data_Interaction;


//裁判系统接收的数据, 0x0301图形删除交互信息, 用户自主发送
typedef struct
{
    uint16_t Header;// = UI_LAYER_DELETE;
    Referee_Data_Robots_ID_Struct Sender;
    uint8_t Reserved;
    Referee_Data_Robots_Client_ID_Struct Receiver;
    Enum_Referee_Data_Interaction_Layer_Delete_Operation Operation;
    uint8_t Delete_Serial;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Tx_Data_Interaction_Layer_Delete;


//裁判系统接收的数据, 0x0301画一个图形交互信息, 用户自主发送
typedef struct
{
    uint16_t Header;
    Referee_Data_Robots_ID_Struct Sender;
    uint8_t Reserved;
    Referee_Data_Robots_Client_ID_Struct Receiver;
    Struct_Referee_Data_Interaction_Graphic_Config Graphic[1];
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Tx_Data_Interaction_Graphic_1;


//裁判系统接收的数据, 0x0301画两个图形交互信息, 用户自主发送
typedef struct
{
    uint16_t Header;// = UI_GRAPHIC_2;
    Referee_Data_Robots_ID_Struct Sender;
    uint8_t Reserved;
    Referee_Data_Robots_Client_ID_Struct Receiver;
    Struct_Referee_Data_Interaction_Graphic_Config Graphic[2];
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Tx_Data_Interaction_Graphic_2;


//裁判系统接收的数据, 0x0301画五个图形交互信息, 用户自主发送
typedef struct
{
    uint16_t Header;// = UI_GRAPHIC_5;
    Referee_Data_Robots_ID_Struct Sender;
    uint8_t Reserved;
    Referee_Data_Robots_Client_ID_Struct Receiver;
    Struct_Referee_Data_Interaction_Graphic_Config Graphic[5];
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Tx_Data_Interaction_Graphic_5;


//裁判系统接收的数据, 0x0301画七个图形交互信息, 用户自主发送
typedef struct
{
    uint16_t Header;// = UI_GRAPHIC_7;
    Referee_Data_Robots_ID_Struct Sender;
    uint8_t Reserved;
    Referee_Data_Robots_Client_ID_Struct Receiver;
    Struct_Referee_Data_Interaction_Graphic_Config Graphic[7];
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Tx_Data_Interaction_Graphic_7;


//裁判系统接收的数据, 0x0301画字符图形交互信息, 用户自主发送
typedef struct
{
    uint16_t Header;// = UI_GRAPHIC_STRING;
    Referee_Data_Robots_ID_Struct Sender;
    uint8_t Reserved;
    Referee_Data_Robots_Client_ID_Struct Receiver;
    Struct_Referee_Data_Interaction_Graphic_Config Graphic_String;
    uint8_t String[30];
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Tx_Data_Interaction_Graphic_String;


//裁判系统接收的数据, 0x0301哨兵自主决策交互信息, 哨兵自主发送
typedef struct
{
    uint16_t Header;// = SENTRY;
    uint32_t Confirm_Respawn_Status_Enum : 1;
    uint32_t Confirm_Exchange_Respawn_Status_Enum : 1;
    uint32_t Request_Exchange_Ammo_Number : 11;
    uint32_t Request_Exchange_Ammo_Time : 4;
    uint32_t Request_Exchange_HP_Time : 4;
    uint32_t Reserved : 11;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Tx_Data_Interaction_Sentry;


//裁判系统接收的数据, 0x0301雷达自主决策交互信息, 雷达自主发送
typedef struct
{
    uint16_t Header;// = RADAR;
    Enum_Referee_Data_Status Request_Double_Damage;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Tx_Data_Interaction_Radar;


//裁判系统接收的数据, 0x0302自定义控制器交互信息, 用户自主发送
typedef struct
{
    uint8_t Data[30];
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Interaction_Custom_Controller;


//裁判系统接收的数据, 0x0303客户端发送小地图交互信息, 用户自主发送
typedef struct
{
    float Coordinate_X;
    float Coordinate_Y;
    Enum_Referee_Data_Status Keyboard;
    Referee_Data_Robots_ID_Struct Enemy_ID;
    Referee_Data_Robots_ID_Struct Source_ID;
    uint8_t Reserved;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Interaction_Robot_Receive_Client_Minimap;


//裁判系统发送的数据, 0x0304图传键鼠遥控交互信息
typedef struct
{
    uint16_t Mouse_X;
    uint16_t Mouse_Y;
    uint16_t Mouse_Z;
    Enum_Referee_Data_Status Mouse_Left_Key_Status;
    Enum_Referee_Data_Status Mouse_Right_Key_Status;
    uint16_t Keyboard_Key;
    uint16_t Reserved;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Interaction_Robot_Receive_Client_Remote_Control;


//裁判系统接收的数据, 0x0305客户端接收小地图交互信息, 用户自主发送
typedef struct
{
	Referee_Data_Robots_ID_Struct Robot_ID;
    float Coordinate_X;
    float Coordinate_Y;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Rx_Data_Interaction_Client_Receive_Radar;


//裁判系统接收的数据, 0x0306客户端接收模拟键鼠遥控交互信息, 用户自主发送
typedef struct
{
    uint16_t Keyboard_Key;
    uint16_t Mouse_X : 12;
    Enum_Referee_Data_Status Mouse_Left_Key_Status : 4;
    uint16_t Mouse_Y : 12;
    Enum_Referee_Data_Status Mouse_Right_Key_Status : 4;
    uint16_t Reserved;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Tx_Data_Interaction_Client_Receive_Custom_Controller;


//裁判系统接收的数据, 0x0307客户端接收模拟键鼠遥控交互信息, 用户自主发送
typedef struct
{
    Enum_Referee_Data_Interaction_Semiautomatic_Command Command;
    uint16_t Start_X;
    uint16_t Start_Y;
    int8_t Delta_X_List[49];
    int8_t Delta_Y_List[49];
    Referee_Data_Robots_Client_ID_Struct Sender_ID;
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Tx_Data_Interaction_Client_Receive_Sentry_Semiautomatic_Minimap;


//裁判系统接收的数据, 0x0308客户端接收模拟键鼠遥控交互信息, 用户自主发送
typedef struct
{
	Referee_Data_Robots_Client_ID_Struct Sender_ID;
	Referee_Data_Robots_Client_ID_Struct Receiver_ID;
    uint8_t Data[30];
    uint16_t CRC_16;
} __attribute__((packed)) Struct_Referee_Tx_Data_Interaction_Client_Receive_Robot_Minimap;




//裁判系统结构体
typedef struct
{
    UART_HandleTypeDef *huart;          // 直接保存 HAL 库句柄指针，如 &huart7
    uint8_t *p_Rx_Buffer;               // 指向你自己在外面建的存储缓冲区
    uint16_t Rx_Buffer_Length;          // 缓冲区长度

//	UART_Manage_Object *UART_Manage_Object;									//绑定的UART(原作的uart封装成宝宝巴士了，看不懂一点)

    uint8_t Frame_Header; 													//数据包头标

    uint32_t Flag;															//当前时刻的裁判系统接收flag
    uint32_t Pre_Flag; 														//前一时刻的裁判系统接收flag

    uint8_t Sequence;														//发送序列号

    uint8_t UI_Change_Flag[10][10];											//UI是否是初次绘制, 没绘制过是0

    Enum_Referee_Status Referee_Status;										//裁判系统状态
    Struct_Referee_Rx_Data_Game_Status Game_Status;							//比赛状态
    Struct_Referee_Rx_Data_Game_Result Game_Result;							//比赛结果
    Struct_Referee_Rx_Data_Game_Robot_HP Game_Robot_HP;						//机器人血量
    Struct_Referee_Rx_Data_Event_Self_Data Event_Self_Data;					//场地事件
    Struct_Referee_Rx_Data_Event_Self_Supply Event_Self_Supply;				//补给站状态
    Struct_Referee_Rx_Data_Event_Referee_Warning Event_Referee_Warning;		//裁判警告信息
    Struct_Referee_Rx_Data_Event_Dart_Status Event_Dart_Status;				//飞镖15s倒计时


    Struct_Referee_Rx_Data_Robot_Status Robot_Status;						//机器人状态
    Struct_Referee_Rx_Data_Robot_Power_Heat Robot_Power_Heat;				//当前机器人实时功率热量
    Struct_Referee_Rx_Data_Robot_Position Robot_Position;					//当前机器人实时位置
    Struct_Referee_Rx_Data_Robot_Buff Robot_Buff;							//当前机器人增益
    Struct_Referee_Rx_Data_Robot_Aerial_Status Robot_Aerial_Status;			//无人机可攻击时间
    Struct_Referee_Rx_Data_Robot_Damage Robot_Damage;						//伤害情况
    Struct_Referee_Rx_Data_Robot_Booster Robot_Booster; 					//子弹信息
    Struct_Referee_Rx_Data_Robot_Remaining_Ammo Robot_Remaining_Ammo;		//子弹剩余信息
    Struct_Referee_Rx_Data_Robot_RFID Robot_RFID;							//RFID状态信息
    Struct_Referee_Rx_Data_Robot_Dart_Command Robot_Dart_Command;			//飞镖状态
    Struct_Referee_Rx_Data_Robot_Sentry_Location Robot_Sentry_Location;		//哨兵获取己方位置信息
    Struct_Referee_Rx_Data_Robot_Radar_Mark Robot_Radar_Mark;				//雷达标记进度
    Struct_Referee_Rx_Data_Robot_Sentry_Decision Robot_Sentry_Decision; 	//哨兵决策信息
    Struct_Referee_Rx_Data_Robot_Radar_Decision Robot_Radar_Decision;		//雷达决策信息

    Struct_Referee_Data_Interaction_Graphic_Config Graphic_Config[10][10];	//图形配置

    Enum_Referee_Data_Status Referee_Trust_Status;  						//裁判系统是否可信
} Class_Referee;
extern Class_Referee Referee;
/*********************************************************************参数*********************************************************************/


/*********************************************************************函数*********************************************************************/
void Referee_Init(UART_HandleTypeDef *huart, uint8_t __Frame_Header);
void Referee_UART_Send_Interaction_UI_Graphic_1(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_1);
void Referee_UART_Send_Interaction_UI_Graphic_2(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_1, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_2);
void Referee_UART_Send_Interaction_UI_Graphic_5(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_1, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_2, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_3, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_4, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_5);
void Referee_UART_Send_Interaction_UI_Graphic_7(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_1, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_2, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_3, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_4, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_5, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_6, Struct_Referee_Data_Interaction_Graphic_Config *Graphic_7);
void Referee_UART_Send_Interaction_UI_Graphic_String(Struct_Referee_Data_Interaction_Graphic_Config *Graphic_String, const char *String_Content);
void Referee_UART_RxCpltCallback(uint8_t *Rx_Data, uint16_t Length);
void Referee_TIM_1000ms_Alive_PeriodElapsedCallback();
uint8_t Referee_Verify_CRC_8(uint8_t *Message, uint32_t Length);
uint16_t Referee_Verify_CRC_16(uint8_t *Message, uint32_t Length);
void Referee_Data_Process(uint16_t Length);

Enum_Referee_Status Referee_Get_Status();
Enum_Referee_Data_Status Referee_Get_Referee_Trust_Status();
Enum_Referee_Game_Status_Type Referee_Get_Game_Type();
Enum_Referee_Game_Status_Stage Referee_Get_Game_Stage();
uint16_t Referee_Get_Remaining_Time();
uint64_t Referee_Get_Timestamp();
Enum_Referee_Game_Result Referee_Get_Result();
uint16_t Referee_Get_HP(Referee_Data_Robots_ID_Struct Robots_ID);
Enum_Referee_Data_Status Referee_Get_Supply_Front_Status();
Enum_Referee_Data_Status Referee_Get_Supply_Inner_Status();
Enum_Referee_Data_Status Referee_Get_Supply_Status();
Enum_Referee_Data_Status Referee_Get_Power_Rune_Status();
Enum_Referee_Data_Status Referee_Get_Power_Rune_Small_Status();
Enum_Referee_Data_Status Referee_Get_Power_Rune_Big_Status();
Enum_Referee_Data_Status Referee_Get_Highland_2_Status();
Enum_Referee_Data_Status Referee_Get_Highland_3_Status();
Enum_Referee_Data_Status Referee_Get_Highland_4_Status();
uint8_t Referee_Get_Base_Virtual_Shield_Percent();
uint16_t Referee_Get_Enemy_Dart_Hit_Time();
Enum_Referee_Dart_Hit_Target Referee_Get_Enemy_Dart_Hit_Target_Enum();
Enum_Referee_Data_Status Referee_Get_Middle_Buff_Status();
Referee_Data_Robots_ID_Struct Referee_Get_Supply_Request_Robot();
Enum_Referee_Data_Event_Supply_Status Referee_Get_Supply_Request_Status();
Enum_Referee_Data_Event_Supply_Ammo_Number Referee_Get_Supply_Ammo_Number();
Enum_Referee_Data_Event_Referee_Warning_Level Referee_Get_Referee_Warning();
Referee_Data_Robot_ID_Struct Referee_Get_Referee_Warning_Robot();
uint8_t Referee_Get_Dart_Remaining_Time();
Enum_Referee_Dart_Hit_Target Referee_Get_Last_Dart_Hit_Target();
uint8_t Referee_Get_Dart_Hit_Target_Count();
Enum_Referee_Dart_Hit_Target Referee_Get_Now_Dart_Hit_Target();
Referee_Data_Robots_ID_Struct Referee_Get_Self_ID();
uint8_t Referee_Get_Self_Level();
uint16_t Referee_Get_Self_HP();
uint16_t Referee_Get_Self_HP_Max();
uint16_t Referee_Get_Self_Booster_Heat_CD();
uint16_t Referee_Get_Self_Booster_Heat_Max();
uint16_t Referee_Get_Self_Chassis_Power_Max();
Enum_Referee_Data_Status Referee_Get_Self_PM01_Gimbal_Status();
Enum_Referee_Data_Status Referee_Get_Self_PM01_Chassis_Status();
Enum_Referee_Data_Status Referee_Get_Self_PM01_Booster_Status();
float Referee_Get_Chassis_Voltage();
float Referee_Get_Chassis_Current();
float Referee_Get_Chassis_Power();
uint16_t Referee_Get_Chassis_Energy_Buffer();
uint16_t Referee_Get_Booster_17mm_1_Heat();
uint16_t Referee_Get_Booster_17mm_2_Heat();
uint16_t Referee_Get_Booster_42mm_Heat();
float Referee_Get_Location_X();
float Referee_Get_Location_Y();
float Referee_Get_Location_Yaw();
uint8_t Referee_Get_HP_Buff_Percent();
uint8_t Referee_Get_Booster_Heat_CD_Buff_Value();
uint8_t Referee_Get_Defend_Buff_Percent();
uint8_t Referee_Get_Defend_Debuff_Percent();
uint8_t Referee_Get_Damage_Buff_Percent();
Enum_Referee_Data_Event_Aerial_Status Referee_Get_Aerial_Status();
uint8_t Referee_Get_Aerial_Remaining_Time();
uint8_t Referee_Get_Armor_Attacked_ID();
Enum_Referee_Data_Event_Robot_Damage_Type Referee_Get_Attacked_Type();
Enum_Referee_Data_Robot_Ammo_Type Referee_Get_Shoot_Ammo_Type();
Enum_Referee_Data_Robot_Booster_Type Referee_Get_Shoot_Booster_Type();
uint8_t Referee_Get_Shoot_Frequency();
float Referee_Get_Shoot_Speed();
uint16_t Referee_Get_17mm_Remaining();
uint16_t Referee_Get_42mm_Remaining();
uint16_t Referee_Get_Money_Remaining();
Enum_Referee_Data_Status Referee_Get_Base_RFID_Status();
Enum_Referee_Data_Status Referee_Get_Highland_2_Self_RFID_Status();
Enum_Referee_Data_Status Referee_Get_Highland_2_Enemy_RFID_Status();
Enum_Referee_Data_Status Referee_Get_Highland_3_Self_RFID_Status();
Enum_Referee_Data_Status Referee_Get_Highland_3_Enemy_RFID_Status();
Enum_Referee_Data_Status Referee_Get_Highland_4_Self_RFID_Status();
Enum_Referee_Data_Status Referee_Get_Highland_4_Enemy_RFID_Status();
Enum_Referee_Data_Status Referee_Get_Energy_RFID_Status();
Enum_Referee_Data_Status Referee_Get_Flyover_1_Self_RFID_Status();
Enum_Referee_Data_Status Referee_Get_Flyover_2_Self_RFID_Status();
Enum_Referee_Data_Status Referee_Get_Flyover_1_Enemy_RFID_Status();
Enum_Referee_Data_Status Referee_Get_Flyover_2_Enemy_RFID_Status();
Enum_Referee_Data_Status Referee_Get_Outpost_RFID_Status();
Enum_Referee_Data_Status Referee_Get_HP_RFID_Status();
Enum_Referee_Data_Status Referee_Get_Sentry_Self_RFID_Status();
Enum_Referee_Data_Status Referee_Get_Sentry_Enemy_RFID_Status();
Enum_Referee_Data_Status Referee_Get_Engineer_Self_RFID_Status();
Enum_Referee_Data_Status Referee_Get_Engineer_Enemy_RFID_Status();
Enum_Referee_Data_Status Referee_Get_Engineer_Exchange_RFID_Status();
Enum_Referee_Data_Status Referee_Get_Middle_RFID_Status();
Enum_Referee_Data_Robot_Dart_Command_Status Referee_Get_Dart_Command_Status();
uint16_t Referee_Get_Dart_Switch_Remaining_Time();
uint16_t Referee_Get_Dart_Launch_Remaining_Time();
float Referee_Get_Sentry_Location_Hero_1_X();
float Referee_Get_Sentry_Location_Hero_1_Y();
float Referee_Get_Sentry_Location_Engineer_2_X();
float Referee_Get_Sentry_Location_Engineer_2_Y();
float Referee_Get_Sentry_Location_Infantry_3_X();
float Referee_Get_Sentry_Location_Infantry_3_Y();
float Referee_Get_Sentry_Location_Infantry_4_X();
float Referee_Get_Sentry_Location_Infantry_4_Y();
float Referee_Get_Sentry_Location_Infantry_5_X();
float Referee_Get_Sentry_Location_Infantry_5_Y();
uint8_t Referee_Get_Radar_Mark_Status_Hero_1();
uint8_t Referee_Get_Radar_Mark_Status_Engineer_2();
uint8_t Referee_Get_Radar_Mark_Status_Infantry_3();
uint8_t Referee_Get_Radar_Mark_Status_Infantry_4();
uint8_t Referee_Get_Radar_Mark_Status_Infantry_5();
uint8_t Referee_Get_Radar_Mark_Status_Sentry_7();
uint16_t Referee_Get_Sentry_Decision_Ammo_Exchange_Number();
uint8_t Referee_Get_Sentry_Decision_Ammo_Exchange_Time();
uint8_t Referee_Get_Sentry_Decision_HP_Exchange_Time();
uint8_t Referee_Get_Radar_Decision_Double_Damage_Chance();
Enum_Referee_Data_Status Referee_Get_Radar_Decision_Double_Damage_Enemy_Status();
void Referee_Set_Referee_Trust_Status(Enum_Referee_Data_Status __Referee_Trust_Status);
void Referee_Set_Referee_UI_Change_Flag_Clear();
Struct_Referee_Data_Interaction_Graphic_Config *Referee_Set_Referee_UI_Clear(uint8_t Layer_Num, uint8_t Graphic_Num);
Struct_Referee_Data_Interaction_Graphic_Config *Referee_Set_Referee_UI_Line(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Start_X, uint32_t Start_Y, uint32_t End_X, uint32_t End_Y);
Struct_Referee_Data_Interaction_Graphic_Config *Referee_Set_Referee_UI_Rectangle(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Start_X, uint32_t Start_Y, uint32_t End_X, uint32_t End_Y);
Struct_Referee_Data_Interaction_Graphic_Config *Referee_Set_Referee_UI_Circle(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Center_X, uint32_t Center_Y, uint32_t Radius);
Struct_Referee_Data_Interaction_Graphic_Config *Referee_Set_Referee_UI_Oval(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Center_X, uint32_t Center_Y, uint32_t Length_X, uint32_t Length_Y);
Struct_Referee_Data_Interaction_Graphic_Config *Referee_Set_Referee_UI_Arc(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Center_X, uint32_t Center_Y, uint32_t Angle_Start, uint32_t Angle_End, uint32_t Length_X, uint32_t Length_Y);
Struct_Referee_Data_Interaction_Graphic_Config *Referee_Set_Referee_UI_Float(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Start_X, uint32_t Start_Y, uint32_t Font_Width, float Float);
Struct_Referee_Data_Interaction_Graphic_Config *Referee_Set_Referee_UI_Integer(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Start_X, uint32_t Start_Y, uint32_t Font_Width, int32_t Integer);
Struct_Referee_Data_Interaction_Graphic_Config *Referee_Set_Referee_UI_String(uint8_t Layer_Num, uint8_t Graphic_Num, Enum_Referee_Data_Interaction_Graphic_Color Color, uint32_t Line_Width, uint32_t Start_X, uint32_t Start_Y, uint32_t Font_Width, uint32_t String_Length);
/*********************************************************************函数*********************************************************************/


#endif /* DEVICE_REFEREE_INC_REFEREE_H_ */
