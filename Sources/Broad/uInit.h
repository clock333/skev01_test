/*
 * uInit.h
 *
 *  Created on: 2024年5月8日
 *      Author: zhonggaowei
 */

#ifndef UINIT_H_
#define UINIT_H_

#include "uCommon.h"

/***************************************************
 * define the application task priority
 * the task priority ranges from 0 to  configMAX_PRIORITIES �C 1
 * 0 is the lowest priority
 ***************************************************/
#define Task_ADC_Mesurement_TaskPriority  				5
#define Task_IO_Ctrl_TaskPriority    					5
#define Task_CAN_Com_TaskPriority         				7
#define Task_Main_Thread_TaskPriority         			6
#define Task_TLE8242_CTRL_TaskPriority         			8

/***************************************************
 * define the application task stack size
 * the actual task size i s the size*4B
 ***************************************************/
#define Task_ADC_Mesurement__STACK_SIZE   	256
#define Task_IO_Ctrl_STACK_SIZE      		configMINIMAL_STACK_SIZE
#define Task_CAN_COM_STACK_SIZE           	512
#define Task_MAIN_STACK_SIZE           		1024
#define Task_TLE8242_CTRL_STACK_SIZE        configMINIMAL_STACK_SIZE

void uBoardInit(void);
void uDataInit(void);
void AppTasks_Create(void);

#endif /* UINIT_H_ */
