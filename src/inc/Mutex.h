/*
  Project   : RA3 RTOS
  Author    : Ali Yasser
  Date      : October 24, 2024
  Version   : 1.0
  Contact   : k4.k4.3li@gmail.com

  Description:
  Mutex implementation for task synchronization in an RTOS.
*/

#ifndef MUTEX_H
#define MUTEX_H

#include "Tasks.h"
#include "FIFO.h"

/** Enum for mutex state */
typedef enum {
    OS_MUTEX_AVAILABLE,
    OS_MUTEX_BUSY,
    OS_MUTEX_ALREADY_ACQUIRED,
    OS_MUTEX_INIT_OK
} OS_MutexState;

/** Mutex structure */
typedef struct {
    int8_t isLocked;                          // Mutex lock flag (1 = locked, 0 = available)
    uint8_t waitingCount;                     // Number of tasks waiting for the mutex
    OS_TCB* owner;                            // Current owner of the mutex
    OS_TCB* waitingQueue[100];                // Waiting queue for tasks
    OS_TCB* waitingQueueBuffer[100];          // FIFO queue buffer
} OS_Mutex;

/* Function prototypes */
OS_MutexState OS_InitMutex(OS_Mutex* mutex);
OS_MutexState OS_AcquireMutex(OS_Mutex* mutex, OS_TCB* task);
OS_MutexState OS_ReleaseMutex(OS_Mutex* mutex);

#endif // MUTEX_H
