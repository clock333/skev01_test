/*
 * CallBack.c
 *
 *  Created on: Apr 22, 2019
 *      Author: nxa07657
 */


/*******************************************************************************
 * User Include
 ******************************************************************************/
#include "CallBack.h"

#include "uCan0.h"
#include "uAdc.h"
#include "ucommon.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

void CAN_TX_RX_Callback(uint32_t instance, can_event_t eventType, uint32_t objIdx, void *driverState)
{
	(void)instance;
	(void)driverState;
	uCanMessage_t msg;

	if(CAN_EVENT_RX_COMPLETE==eventType)
	{
		msg.type = canMessageType_rx;
		msg.buffer = u.can.rxMessageBuffer[u.can.rxBufferIndex++%CAN_RX_Buffer_MAX];
		OSIF_QueueWrite(&u.can.queue,&msg,10);
		CAN_Receive(&can_pal1_instance,objIdx,&u.can.rxMessageBuffer[u.can.rxBufferIndex%CAN_RX_Buffer_MAX]);
	}
}

void ADC0_DMA1_Callback(void *parameter, edma_chn_status_t status)
{
	(void)status;
	(void)parameter;

	memcpy(&u.adc.handleBuffer[u.adc.index], u.adc.buffer, 16);
	u.adc.index = (u.adc.index + 1)%ADC_BUFFER_MAX;
	if(!u.adc.index)	OSIF_SemaPost(&u.adc.sema);
}

/******************************************************************************
 * EOF
 *****************************************************************************/
