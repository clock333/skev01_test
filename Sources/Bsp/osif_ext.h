/*
 * osif_ext.h
 *
 *  Created on: Oct 16, 2018
 *      Author: nxf45772
 */

#ifndef OSIF_EXT_H_
#define OSIF_EXT_H_

#include "cpu.h"
#include "osif.h"

#ifdef USING_OS_FREERTOS
#include "queue.h"
typedef QueueHandle_t queue_t;

#else
typedef struct{

	char* pHeader;
	char* pRead;
	char* pWrite;
	uint32_t size;
	uint32_t blockSize;
	uint32_t fillSize;
	uint32_t overwrite;
	mutex_t taskMut;
	semaphore_t canRead;
}queue_t;

#endif

status_t OSIF_QueueCreate(queue_t* pQ,uint32_t itemSize,uint32_t queueLen);
status_t OSIF_QueueDesory(queue_t* pQ);
status_t OSIF_QueueRead(queue_t* pQ,void* item,uint32_t timeout);
status_t OSIF_QueueWrite(queue_t* pQ,void* item,uint32_t timeout);

#endif /* OSIF_EXT_H_ */
