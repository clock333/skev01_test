/****************************************************************************************************************
 * osif_ext.c
 *
 *  Created on: Oct 16, 2018
 *      Author: nxf45772
 *      Function: these API are used to extend the S32K SDK OSIF component function to support Queue operations
 *      Reversion: modified by NXA07657 to support using queue from ISR, 2019.5.9
 ****************************************************************************************************************/

#include "osif_ext.h"
#include "string.h"

#define QUEUE_LENGTH  10
#define QUEUE_ITEMSIZE 50

#ifndef USING_OS_FREERTOS

#define QUEUE_BUF_LENGTH QUEUE_LENGTH*QUEUE_ITEMSIZE
static uint8_t queue_buf[QUEUE_BUF_LENGTH];

#else

/* Cortex M device - read ICSR[IPSR] value */
static inline bool osif_IsIsrContext(void)
{
    bool is_isr = false;
    uint32_t ipsr_code = (uint32_t)( (S32_SCB->ICSR & S32_SCB_ICSR_VECTACTIVE_MASK) >> S32_SCB_ICSR_VECTACTIVE_SHIFT );
    if (ipsr_code != 0u)
    {
        is_isr = true;
    }

    return is_isr;
}

#endif

#ifdef USING_OS_FREERTOS

status_t OSIF_QueueCreate(queue_t* pQ,uint32_t itemSize,uint32_t queueLen)
{
	status_t err;
	*pQ=xQueueCreate(queueLen,itemSize );
	if(*pQ!=NULL){
		err=STATUS_SUCCESS;
	}else{
		err=STATUS_ERROR;
	}
	return err;
}
#else
/*
 *
 * the bufPoolLen must >= length*itemSize+length*sizeof(queue_t)
 */
status_t OSIF_QueueCreate(queue_t* pQ,uint32_t itemSize,uint32_t queueLen)
{
	status_t err;
	
	if(queueLen*itemSize>QUEUE_BUF_LENGTH)
	{
		err=STATUS_ERROR;
		return err; /* not match misa*/
	}
	
	pQ->blockSize=itemSize;
	pQ->size=QUEUE_BUF_LENGTH;
	pQ->pWrite=pQ->pRead=pQ->pHeader=queue_buf;
	pQ->overwrite=0;

	err=OSIF_MutexCreate(&(pQ->taskMut));
	err|=OSIF_SemaCreate(&(pQ->canRead),0);

	if(err!=STATUS_SUCCESS)
	{
		OSIF_QueueDesory(pQ);
		return STATUS_ERROR;
	}
	//OSIF_SemaWait(&(pQ->canRead),OSIF_WAIT_FOREVER);
	return err;
}
#endif

#ifdef USING_OS_FREERTOS
status_t OSIF_QueueDesory(queue_t* pQ)
{
	vQueueDelete(*pQ);
	return STATUS_SUCCESS;
}
#else
status_t OSIF_QueueDesory(queue_t* pQ)
{
	OSIF_MutexDestroy(&(pQ->taskMut));
	OSIF_SemaDestroy(&(pQ->canRead));
	return STATUS_SUCCESS;
}
#endif

#ifdef USING_OS_FREERTOS
status_t OSIF_QueueRead(queue_t* pQ,void* item,uint32_t timeout)
{

	BaseType_t operation_status = pdFAIL;
	status_t osif_ret_code;

	/* Check if the post operation is executed from ISR context */
	bool is_isr = osif_IsIsrContext();
	if (is_isr)
	{
		/* Execution from exception handler (ISR) */
		BaseType_t taskWoken = pdFALSE;
		operation_status = xQueueReceiveFromISR(*pQ,item,&taskWoken);

		if (operation_status == pdPASS)
		{
			/* Perform a context switch if necessary */
			portYIELD_FROM_ISR(taskWoken);
		}
	}
	else
	{
		/* Execution from task */
		operation_status = xQueueReceive(*pQ,item,timeout);
	}

	/* pdFAIL in case that the semaphore is full */
	osif_ret_code = (operation_status == pdPASS) ? STATUS_SUCCESS : STATUS_ERROR;

	return osif_ret_code;
}
#else

status_t OSIF_QueueRead(queue_t* pQ,void* item,uint32_t timeout)
{
	status_t err;
	uint32_t readSize=pQ->blockSize;


	while(1)
	{
		err=OSIF_MutexLock(&(pQ->taskMut),timeout);
		if(err!=STATUS_SUCCESS)
		{
			return err;
		}
		if(pQ->fillSize==0)
		{
			OSIF_MutexUnlock(&(pQ->taskMut));
			err=OSIF_SemaWait(&(pQ->canRead),timeout);
			if(err!=STATUS_SUCCESS)
				return err;
			continue;
		}
		if((pQ->pRead+readSize)>(pQ->pHeader+pQ->size)){
			int rlen1=pQ->pHeader+pQ->size-pQ->pRead;
			int rlen2=readSize-rlen1;
			memcpy((char*)item,pQ->pRead,rlen1);
			memcpy((char*)item+rlen1,pQ->pHeader,rlen2);
			pQ->pRead=pQ->pHeader+rlen2;
		}else{
			memcpy((char*)item,pQ->pRead,readSize);
			pQ->pRead+=readSize;
		}
		pQ->fillSize-=readSize;
		OSIF_MutexUnlock(&(pQ->taskMut));
		break;
	}
	return STATUS_SUCCESS;
}
#endif


#ifdef USING_OS_FREERTOS

status_t OSIF_QueueWrite(queue_t* pQ,void* item,uint32_t timeout)
{
	BaseType_t operation_status = pdFAIL;
	status_t osif_ret_code;

	/* Check if the post operation is executed from ISR context */
	bool is_isr = osif_IsIsrContext();
	if (is_isr)
	{
		/* Execution from exception handler (ISR) */
		BaseType_t taskWoken = pdFALSE;
		operation_status = xQueueSendFromISR(*pQ,item,taskWoken);

		if (operation_status == pdPASS)
		{
			/* Perform a context switch if necessary */
			portYIELD_FROM_ISR(taskWoken);
		}
	}
	else
	{
		/* Execution from task */
		operation_status = xQueueSend(*pQ,item,timeout);
	}

	/* pdFAIL in case that the semaphore is full */
	osif_ret_code = (operation_status == pdPASS) ? STATUS_SUCCESS : STATUS_ERROR;

	return osif_ret_code;
                                             
}
#else
status_t OSIF_QueueWrite(queue_t* pQ,void* item,uint32_t timeout)
{
	status_t err;
	uint32_t bufLen=pQ->blockSize;
	uint32_t writeSize=pQ->size-pQ->fillSize;

	err=OSIF_MutexLock(&(pQ->taskMut),timeout);
	if(err!=STATUS_SUCCESS)
	{
		return err;
	}
	if((pQ->pWrite+bufLen)>(pQ->pHeader+pQ->size))
	{
		int wlen1=pQ->pHeader+pQ->size-pQ->pWrite;
		int wlen2=bufLen-wlen1;
		memcpy(pQ->pWrite,(char*)item,wlen1);
		memcpy(pQ->pHeader,(char*)item+wlen1,wlen2);
		pQ->pWrite=pQ->pHeader+wlen2;
	}else{
		memcpy(pQ->pWrite,(char*)item,bufLen);
		pQ->pWrite+=bufLen;
	}
	if(writeSize<bufLen)
	{
		pQ->overwrite=1;
		if((pQ->pRead+bufLen)>(pQ->pHeader+pQ->size)){
			int rlen1=pQ->pRead+bufLen-pQ->pHeader-pQ->size;
			pQ->pRead=pQ->pHeader+rlen1;
		}else{
			pQ->pRead+=bufLen;
		}
	}else
	{
		pQ->fillSize+=bufLen;
	}
	OSIF_MutexUnlock(&(pQ->taskMut));
	OSIF_SemaPost(&(pQ->canRead));

	return STATUS_SUCCESS;
}

#endif












