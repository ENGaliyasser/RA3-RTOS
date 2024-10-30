/*
  Project   : G RTOS
  Author    : Ali Yasser
  Date      : October 24, 2024
  Version   : 1.0
  Contact   : k4.k4.3li@gmail.com

  Description:
  Handling Semaphore operations like acquire and release.
*/

#include "Semaphore.h"

/**
 * @brief Initializes a semaphore.
 *
 * This function initializes the semaphore with an initial count,
 * sets the waiting count to zero, and clears the owner.
 *
 * @param semaphore Pointer to the OS_Semaphore structure to be initialized.
 * @param initialCount The initial count of the semaphore.
 * @return OS_SemaphoreState Status of the semaphore initialization.
 */
OS_SemaphoreState OS_InitSemaphore(OS_Semaphore* semaphore, uint8_t initialCount) {
    semaphore->count = initialCount;           // Set the initial count of the semaphore
    semaphore->waitingCount = 0;               // Initialize the waiting count to zero
    semaphore->owner = NULL;                    // Set the owner to NULL

    // Initialize the waiting queue for tasks
    OS_FifoInit(&(semaphore->waitingQueue), semaphore->waitingQueueBuffer, 100);

    return OS_SEMAPHORE_INIT_OK;               // Indicate successful initialization
}

/**
 * @brief Acquires a semaphore.
 *
 * This function attempts to acquire the semaphore. If the semaphore
 * is already acquired by the calling task, it returns an error.
 * If the semaphore is busy and the calling task is not the owner,
 * the task is added to the waiting queue and terminated.
 *
 * @param semaphore Pointer to the OS_Semaphore structure to acquire.
 * @param task Pointer to the OS_TCB structure of the task attempting to acquire the semaphore.
 * @return OS_SemaphoreState Status of the semaphore acquisition.
 */
OS_SemaphoreState OS_AcquireSemaphore(OS_Semaphore* semaphore, OS_TCB* task) {
    semaphore->count--;  // Decrement the semaphore count
    if (task == semaphore->owner) {
        return OS_SEMAPHORE_ALREADY_ACQUIRED;  // Return error if the task is already the owner
    }
    if (semaphore->count < 0 && semaphore->owner) {
        // If the semaphore is busy and owned, add the task to the waiting queue
        semaphore->waitingCount++;
        OS_FifoEnqueue(&(semaphore->waitingQueue), task); // Enqueue the task
        OS_TerminateTask(task);  // Terminate the task since it can't proceed
        return OS_SEMAPHORE_BUSY; // Indicate the semaphore is busy
    } else {
        semaphore->owner = task; // Set the current task as the owner
    }
    return OS_SEMAPHORE_AVAILABLE; // Indicate the semaphore is available
}

/**
 * @brief Releases a semaphore.
 *
 * This function releases the semaphore, increments its count,
 * and if there are tasks waiting for the semaphore, it dequeues
 * one and activates it.
 *
 * @param semaphore Pointer to the OS_Semaphore structure to release.
 * @return OS_SemaphoreState Status of the semaphore release.
 */
OS_SemaphoreState OS_ReleaseSemaphore(OS_Semaphore* semaphore) {
    OS_TCB* dequeuedTask; // Variable to hold the task that will be dequeued
    semaphore->count++;  // Increment the semaphore count
    if (semaphore->count <= 0) {
        // If there are tasks waiting for the semaphore
        semaphore->waitingCount--; // Decrement the waiting count
        OS_FifoDequeue(&(semaphore->waitingQueue), &dequeuedTask); // Dequeue the next waiting task
        semaphore->owner = dequeuedTask; // Set the dequeued task as the owner
        OS_ActivateTask(dequeuedTask); // Activate the dequeued task
        return OS_SEMAPHORE_AVAILABLE; // Indicate the semaphore is available
    }
    return OS_SEMAPHORE_BUSY; // Indicate the semaphore is still busy
}
