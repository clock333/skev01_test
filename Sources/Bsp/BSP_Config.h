/*******************************************************************************
*
* Copyright 2006-2015 Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
*
****************************************************************************//*!
*
* @file     BSP_Config.h
*
* @date     March-28-2017
*
* @brief    MCU Peripherals Configuration
*
*******************************************************************************/
#ifndef BSP_CONFIG_H_
#define BSP_CONFIG_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "Cpu.h"

/* FreeRTOS Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/*include to use extend OSIF queue operation*/
#include "osif_ext.h"

#include "string.h"

/*******************************************************************************
* Constants and macros
*******************************************************************************/

/**********************************************************************
 * enable this macro to enable debug print
 * comment it to disable the debug print
 **********************************************************************/
#define DEBUG_PRINT_ENABLE 1

/********************************************************************************
 * the mutex for UART print peripheral lock
 * user use define it before use it
 * recommend to create the mutex in AppTasks_Create() after LPUART initialization
 ********************************************************************************/
 extern mutex_t UART_Print_Mutex;

/*********************************************************************
 * Definition of power modes indexes, as configured in Power Manager Component
 *  Refer to the Reference Manual for details about the power modes.
 *********************************************************************/
#define HSRUN (0u) /* High speed run      */
#define RUN   (1u) /* Run                 */
#define VLPR  (2u) /* Very low power run  */
#define STOP1 (3u) /* Stop option 1       */
#define STOP2 (4u) /* Stop option 2       */
#define VLPS  (5u) /* Very low power stop */

/*******************************************************************************
 * CPU_Dealy service API
 ******************************************************************************/
/*!
 * @addtogroup function_group
 * @{
 */
/*!
 * @brief Waits for specified amount of cycles which is given by 32bit
 *        value range. Assumption for this function is that target
 *        architecture is using 32bit general purpose registers.
 *
 * @param cycles - Number of cycles to wait.
 */
void CPU_DealyCycles(uint32_t cycles);

/*!
 * @brief Waits for specified amount of seconds.
 *
 * @param delay - Number of seconds to wait.
 */
void CPU_DealySec(uint16_t delay);

/*!
 * @brief Waits for specified amount of milliseconds.
 *
 * @param delay - Number of milliseconds to wait.
 */
void CPU_DealyMs(uint16_t delay);

/*!
 * @brief Waits for specified amount of microseconds.
 *
 * @param delay - Number of microseconds to wait.
 */
void CPU_DealyUs(uint16_t delay);

/*******************************************************************************
* Global function prototypes
*******************************************************************************/

void MCU_SystemInit(void);
void McuClockConfig(void);
void McuPowerConfig(void);
void McuIntConfig(void);
void McuPinsConfig(void);
void McuCacheConfig(void);
void McuDmaConfig(void);
void McuMpuInit(void);
void McuFlashPrefetchConfig(void);

/*the route used for debug print via OpenSDA virtual UART*/
status_t UART_Print(char *string);

#endif /* BSP_CONFIG_H_ */
