/*
  Project   : RA3 RTOS
  Author    : Ali Yasser
  Date      : October 24, 2024
  Version   : 1.0
  Contact   : k4.k4.3li@gmail.com

  Description:
  Implementation of Mutex functions for task synchronization in an RTOS.
*/

#include "Mutex.h"
#include "FIFO.h"

/**
 * @brief Initializes a mutex.
 *
 * @param mutex Pointer to the mutex to be initialized.
 * @return OS_MutexState The result of the initialization (OS_MUTEX_INIT_OK).
 */
OS_MutexState OS_InitMutex(OS_Mutex* mutex) {
    mutex->isLocked = 0;           // Mutex is initially available
    mutex->waitingCount = 0;
    mutex->owner = NULL;

    OS_FifoInit(&(mutex->waitingQueue), mutex->waitingQueueBuffer, 100);

    return OS_MUTEX_INIT_OK;
}

/**
 * @brief Acquires the mutex for a task.
 *
 * @param mutex Pointer to the mutex.
 * @param task Pointer to the task attempting to acquire the mutex.
 * @return OS_MutexState The result of the acquire attempt (e.g., OS_MUTEX_AVAILABLE, OS_MUTEX_BUSY).
 */
OS_MutexState OS_AcquireMutex(OS_Mutex* mutex, OS_TCB* task) {
    if (mutex->isLocked && task == mutex->owner) {
        return OS_MUTEX_ALREADY_ACQUIRED;  // Task already owns the mutex
    }

    if (mutex->isLocked) {
        mutex->waitingCount++;
        OS_FifoEnqueue(&(mutex->waitingQueue), task);
        OS_TerminateTask(task);  // Block the task until mutex is available
        return OS_MUTEX_BUSY;
    } else {
        mutex->isLocked = 1;  // Lock the mutex
        mutex->owner = task;   // Set the owner of the mutex
        return OS_MUTEX_AVAILABLE;
    }
}

/**
 * @brief Releases the mutex from the current owner.
 *
 * @param mutex Pointer to the mutex.
 * @return OS_MutexState The result of the release operation (OS_MUTEX_AVAILABLE).
 */
OS_MutexState OS_ReleaseMutex(OS_Mutex* mutex) {
	OS_TCB* dequeuedTask;

    if (!mutex->isLocked) {
        return OS_MUTEX_AVAILABLE;  // Mutex is already available
    }

    mutex->isLocked = 0;  // Unlock the mutex
    if (mutex->waitingCount > 0) {
        mutex->waitingCount--;
        OS_FifoDequeue(&(mutex->waitingQueue), &dequeuedTask);
        mutex->owner = dequeuedTask;
        mutex->isLocked = 1;  // Lock the mutex for the new owner
        OS_ActivateTask(dequeuedTask);  // Wake up the next task in queue
    } else {
        mutex->owner = NULL;  // No tasks waiting, release ownership
    }

    return OS_MUTEX_AVAILABLE;
}
