/*
 * uCommon.h
 *
 *  Created on: 2024年5月7日
 *      Author: zhonggaowei
 */

#ifndef UCOMMON_H_
#define UCOMMON_H_

#include "Cpu.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "BSP_Config.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;


#define HexHI(n)  (((n)&0xffff0000)>>16)
#define HexLOW(n)  ((n)&0xffff)

#define INIT_THREAD_FREQUENCY 10

/***********************ADC*************************/
typedef struct{
	uint16_t gas;
	uint16_t temp;
	uint16_t batteryVol;
	uint16_t posRl;
	uint16_t posRr;
	uint16_t posFl;
	uint16_t posFr;
	uint16_t bandgap;
}ADC_USER;

#define ADC_BUFFER_MAX 50

struct UADC_T{
	uint16_t buffer[8];
	ADC_USER handleBuffer[ADC_BUFFER_MAX];
	ADC_USER result;
	uint8_t index;
	semaphore_t sema;
};

/***********************CAN*************************/
#define CAN_DEBUG_ID_DOWNLINK           0x1316
#define CAN_DEBUG_ID_UPLINK             0x1317

#define CAN_CTRL_ID_DOWNLINK            0x300
#define CAN_CTRL_ID_UPLINK              0x301

#define CAN_RX_Buffer_SIZE sizeof(can_message_t)
#define CAN_RX_Buffer_MAX 2


/***********************CAN0*************************/

struct UCAN_T{
	uint32_t rxBufferIndex;
	can_message_t rxMessageBuffer[CAN_RX_Buffer_MAX];

	queue_t queue;
};

/***********************EEE*************************/
#pragma pack(push)
#pragma pack(1)
typedef struct {
	uint8_t isFrontBridgeEnable;//使能前桥
	uint8_t isRearBridgeEnable;//使能后桥
	uint8_t flSensorDirection;//左前轮传感器方向：正向 ： 反向
	uint8_t frSensorDirection;//右前轮传感器方向：正向 ： 反向
	uint8_t rlSensorDirection;//左后轮传感器方向：正向 ： 反向
	uint8_t rrSensorDirection;//右后轮传感器方向：正向 ： 反向
	uint8_t tankGasPressureMax;//气罐压力最大值限值
	uint8_t tankGasPressureMin;//气罐压力最小值限值
	uint8_t isEnableGasPumpTemperature;//使能检测气泵温度

	uint8_t dynamicSpeedLiminitValue;//判定动态和静态的速度限值
	uint8_t switch2midPosSpeedLiminitValue;//车速达到此值后，自动调整到中位
	uint8_t frontBridgeCoefficient;//前桥高度传感器计算斜率值
	uint8_t rearBridgeCoefficient;//后桥高度传感器计算斜率值
	uint8_t staticModeAccuracy;//静态模式精度
	uint8_t dynamicModeAccuracy;//动态模式精度
	uint16_t positionHighOffset;//“高位”相对于中位的差值
	uint16_t positionLowOffset;//“低位”相对于中位的差值
	uint16_t limitHighOffset;//“高位”限值
	uint16_t limitLowOffset;//“低位”限值

	uint8_t startupMode;//启动时模式：中位 ： 回到熄火时位置
	uint8_t frontBridgeCtrlMode;//前桥控制模式： 轮控 ： 桥控
	uint8_t rearBridgeCtrlMode;//后桥控制模式： 轮控 ： 桥控
	uint8_t roadShoulderValue;//路肩模式限值
	uint8_t roadShoulderCtrlMode;//路肩模式的控制方式
	uint8_t gasPumpTemperatureAlertValue;//气泵温度传感器温度报警值
	uint8_t gasPumpContinuousWorkingPeriod;//气泵允许连续工作的时间周期
	uint8_t acceleratorPedalThreshold;//油门踏板限值
	uint8_t steeringWheelThreshold;//方向盘限值
	uint8_t brakePedalThreshold;//刹车踏板限值
	uint8_t doorCtrl;//门控
}USER_CONFIGURATION_T;
#pragma pack(pop)

#define EEE_BASE                    0x14000000u
#define uconf	((USER_CONFIGURATION_T*)EEE_BASE)

/***********************VALVE*************************/

#define TLE8242_CHANNAL_VENT_VALVE 		0UL
#define TLE8242_CHANNAL_TANK_VALVE   	1UL
#define TLE8242_CHANNAL_RL_VALVE     	2UL
#define TLE8242_CHANNAL_RR_VALVE     	3UL
#define TLE8242_CHANNAL_GAS_PUMP        4UL
#define TLE8242_CHANNAL_FL_VALVE        6UL
#define TLE8242_CHANNAL_FR_VALVE        7UL

/***********************USER*************************/
struct TheCar_T{
	uint16_t speedOfCar;
	uint8_t door;
	uint16_t steeringWheelAngle;
	uint8_t brakePedal;
	uint16_t acceleratorPedal;
	uint16_t rotate;
};
/***********************USER*************************/

enum{
	uMessageType_btn,
	uMessageType_timer,

	uMessageType_handShake,

	uMessageType_adc_bridgeRear,
	uMessageType_adc_batteryVoltage,
	uMessageType_adc_gasPressure,
	uMessageType_adc_valvesTemperature,

	uMessageType_car_speed,
	uMessageType_car_door,
	uMessageType_car_angle,
	uMessageType_car_brakePedal,
	uMessageType_car_rotate,
	uMessageType_car_acceleratorPedal,


	uMessageType_canCtrl,
	uMessageType_canConf,
	uMessageType_canDebug,
};

enum{
	uMessageValue_canCtrl_ventValve_on,
	uMessageValue_canCtrl_ventValve_off,
	uMessageValue_canCtrl_tankValve_on,
	uMessageValue_canCtrl_tankValve_off,
	uMessageValue_canCtrl_rearleftValve_on,
	uMessageValue_canCtrl_rearleftValve_off,
	uMessageValue_canCtrl_rearrightValve_on,
	uMessageValue_canCtrl_rearrightValve_off,
	uMessageValue_canCtrl_gasPumpValve_on,
	uMessageValue_canCtrl_gaspumpValve_off,
	uMessageValue_canCtrl_frontleftValve_on,
	uMessageValue_canCtrl_frontleftValve_off,
	uMessageValue_canCtrl_frontrightValve_on,
	uMessageValue_canCtrl_frontrightValve_off,

	uMessageValue_canCtrl_up,
	uMessageValue_canCtrl_mid,
	uMessageValue_canCtrl_down,
};

typedef struct{
	uint16_t type;
	uint32_t value;
}umsg_t;

enum{
	uMainStatus_init = 2,
	uMainStatus_debug,
	uMainStatus_normal,
	uMainStatus_fireOff,
};

struct MainThread_T{
	queue_t queue;
};

typedef struct{
	uint8_t cmd;
	uint8_t ecuType;
	uint8_t client;
	uint8_t carType;
	uint8_t hardware_major;
	uint8_t hardware_minor;
	uint8_t software_major;
	uint8_t software_minor;
}BoardInfo_T;

struct USER_T{
	struct UCAN_T can;
	USER_CONFIGURATION_T conf;
	struct MainThread_T mt;
//	struct TLE8242_T tle;
	struct TheCar_T car;
	struct UADC_T adc;
	uint16_t uMainStatus;
	uint32_t count_lpit;
	uint8_t testCount;
	can_message_t bufferTest;
	u8 flag;
};

extern struct USER_T u;
extern const BoardInfo_T boardInfo;

void uCanPrintf(const char *format, ...);
unsigned int fourCharToInt(unsigned char a, unsigned char b, unsigned char c, unsigned char d);

#endif /* UCOMMON_H_ */
