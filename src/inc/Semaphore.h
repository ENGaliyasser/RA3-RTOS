/*
  Project   : RA3 RTOS
  Author    : Ali Yasser
  Date      : October 24, 2024
  Version   : 1.0
  Contact   : k4.k4.3li@gmail.com

  Description:
  Semaphore definitions and APIs for G RTOS.
*/

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "FIFO.h"
#include "Tasks.h"

/** Enum for semaphore states */
typedef enum {
    OS_SEMAPHORE_AVAILABLE,        // Semaphore is available
    OS_SEMAPHORE_BUSY,             // Semaphore is currently busy
    OS_SEMAPHORE_ALREADY_ACQUIRED, // Task already owns the semaphore
    OS_SEMAPHORE_INIT_OK           // Semaphore initialized successfully
} OS_SemaphoreState;

/** Semaphore structure */
typedef struct {
    int32_t count;                     // Semaphore count (number of available resources)
    uint8_t waitingCount;              // Number of tasks waiting for the semaphore
    OS_TCB* owner;                     // Current owner of the semaphore
    OS_tBuffer waitingQueue;           // FIFO buffer for waiting tasks
    OS_TCB* waitingQueueBuffer[100];   // Buffer for task control blocks waiting for the semaphore
} OS_Semaphore;

/** Semaphore function prototypes */
OS_SemaphoreState OS_InitSemaphore(OS_Semaphore* semaphore, uint8_t initialCount);
OS_SemaphoreState OS_AcquireSemaphore(OS_Semaphore* semaphore, OS_TCB* task);
OS_SemaphoreState OS_ReleaseSemaphore(OS_Semaphore* semaphore);

#endif // SEMAPHORE_H
