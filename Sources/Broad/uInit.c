/*
 * uInit.c
 *
 *  Created on: 2024Äê5ÔÂ8ÈÕ
 *      Author: zhonggaowei
 */

#include "uInit.h"
#include "uCan0.h"
#include "uAdc.h"
#include "uTimer.h"
#include "uio.h"
#include "uFlash.h"
#include "uTle8242.h"
#include "uMainThread.h"

void uBoardInit(void){
	MCU_SystemInit();
	uKeyConfig();
	uFlashInit();
	uTimerInit();

	PWM_Init(&pwm_pal1Instance,&pwm_pal1Configs);
//	PWM_UpdateDuty(&pwm_pal1Instance, 0, 1);
}

void uDataInit(void){
	uConfigurationRead();
	u.can.rxBufferIndex = 0;
	u.uMainStatus = uMainStatus_init;

	OSIF_QueueCreate(&u.can.queue, sizeof(uCanMessage_t), 10);
	OSIF_QueueCreate(&u.mt.queue, sizeof(umsg_t), 10);
}

void AppTasks_Create(void){
	xTaskCreate(Task_CAN_Receive, (const char *)"CAN",\
			Task_CAN_COM_STACK_SIZE, NULL, \
			Task_CAN_Com_TaskPriority, NULL );

	xTaskCreate(Task_ADC_Mesurement, (const char * const)"ADC", \
			Task_ADC_Mesurement__STACK_SIZE, NULL, \
			Task_ADC_Mesurement_TaskPriority, NULL );

	xTaskCreate(Task_IO_Control, (const char * const)"IO", \
			Task_IO_Ctrl_STACK_SIZE, NULL, \
			Task_IO_Ctrl_TaskPriority, NULL );

	xTaskCreate(Task_mThread, (const char * const)"m", \
			Task_MAIN_STACK_SIZE, NULL, \
			Task_Main_Thread_TaskPriority, NULL );

	xTaskCreate(Task_TLE8242_Ctrl, (const char * const)"8242", \
			Task_TLE8242_CTRL_STACK_SIZE, NULL, \
			Task_TLE8242_CTRL_TaskPriority, NULL );
}
