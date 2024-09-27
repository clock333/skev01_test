/*
 * CallBack.h
 *
 *  Created on: Apr 22, 2019
 *      Author: nxa07657
 */

#ifndef CALLBACK_H_
#define CALLBACK_H_

#if defined (__cplusplus)
extern "C" {
#endif

#include "uCommon.h"

/*******************************************************************************
 * Global Variables
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

extern void CAN_TX_RX_Callback(uint32_t instance, can_event_t eventType, uint32_t objIdx, void *driverState);
extern void ADC0_DMA1_Callback(void *parameter, edma_chn_status_t status);

#if defined (__cplusplus)
}
#endif


#endif /* CALLBACK_H_ */
