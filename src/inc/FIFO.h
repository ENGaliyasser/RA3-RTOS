/*
  Project   : G RTOS
  Author    : Ali Yasser
  Date      : October 24, 2024
  Version   : 1.0
  Contact   : k4.k4.3li@gmail.com

  Description:
  Handling FIFOs operations like enqueue and dequeue
*/
#ifndef INC_FIFO_H_
#define INC_FIFO_H_

#include <stdio.h>
#include <stdint.h>
#include "Tasks.h"


#define ELEMENT_TYPE OS_TCB*
typedef struct{
	unsigned int counter;
	ELEMENT_TYPE* head;
	ELEMENT_TYPE* tail;
	ELEMENT_TYPE* base;
	unsigned int  length;
}OS_tBuffer;

typedef enum{
	FIFO_NO_ERROR,
	FIFO_FULL,
	FIFO_EMPTY,
	FIFO_NULL,

}OS_BufferStatus;

OS_BufferStatus OS_FifoInit (OS_tBuffer* fifo,ELEMENT_TYPE* buff , uint32_t length);
OS_BufferStatus OS_FifoEnqueue (OS_tBuffer* fifo,ELEMENT_TYPE item);
OS_BufferStatus OS_FifoDequeue (OS_tBuffer* fifo,ELEMENT_TYPE* item);
OS_BufferStatus OS_IsFifoFull (OS_tBuffer* fifo);


#endif /* INC_FIFO_H_ */
