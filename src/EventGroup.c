/*
  Project   : RA3 RTOS
  Author    : Ali Yasser
  Date      : October 24, 2024
  Version   : 1.0
  Contact   : k4.k4.3li@gmail.com

  Description:
  Source file for Event Group management in an embedded RTOS.
*/

#include "EventGroup.h"
#include "Tasks.h"

/**
 * @brief Initializes an event group.
 *
 * This function sets the event bits to 0 and initializes the waiting tasks count to 0.
 *
 * @param eventGroup Pointer to the OS_EventGroup structure to be initialized.
 */
void OS_InitEventGroup(OS_EventGroup* eventGroup) {
    eventGroup->bits = 0;                 // Clear all event bits
    eventGroup->numWaitingTasks = 0;      // No tasks are waiting initially
}

/**
 * @brief Waits for specified event bits to be set.
 *
 * This function blocks the calling task until the specified event bits are set
 * or a timeout occurs. It checks if the required bits are already set before blocking.
 *
 * @param eventGroup Pointer to the OS_EventGroup structure being used.
 * @param eventBits The event bits to wait for.
 * @param waitForAllBits If set to 1, the function waits for all specified bits to be set;
 *                       if set to 0, it waits for any of the specified bits to be set.
 * @param timeout Time in milliseconds to wait before timing out.
 * @return uint8_t OS_EVENT_GROUP_OK if the event bits are set, or OS_EVENT_GROUP_TIMEOUT if the timeout occurs.
 */
uint8_t OS_WaitForEventBits(OS_EventGroup* eventGroup, OS_EventGroupBits eventBits, uint8_t waitForAllBits, uint32_t timeout) {
    OS_EventGroupBits currentBits = eventGroup->bits;

    // Check if the required bits are already set
    if ((waitForAllBits && ((currentBits & eventBits) == eventBits)) ||
        (!waitForAllBits && (currentBits & eventBits))) {
        return OS_EVENT_GROUP_OK;  // Event bits already set
    }

    // If not set, add the current task to the waiting list
    OS_TCB* currentTask = OS_ControlBlock.CurrentTask;  // Access current task directly

    // Block the task with timeout using the existing function
    OS_DelayTask(currentTask, timeout); // Block the task and wait for event bits

    // After waking up (when either the bits are set or timeout occurs), check the event bits again
    currentBits = eventGroup->bits;
    if ((waitForAllBits && ((currentBits & eventBits) == eventBits)) ||
        (!waitForAllBits && (currentBits & eventBits))) {
        return OS_EVENT_GROUP_OK;  // Event bits are now set
    }

    return OS_EVENT_GROUP_TIMEOUT;  // Timeout occurred, event bits not set
}

/**
 * @brief Sets specified event bits in the event group.
 *
 * This function updates the event bits and activates any tasks that are waiting for the bits to be set.
 *
 * @param eventGroup Pointer to the OS_EventGroup structure where the bits will be set.
 * @param eventBits The event bits to be set.
 */
void OS_SetEventBits(OS_EventGroup* eventGroup, OS_EventGroupBits eventBits) {
    eventGroup->bits |= eventBits;  // Set the specified event bits

    // Unblock any tasks waiting for the event bits
    for (uint8_t i = 0; i < eventGroup->numWaitingTasks; i++) {
        OS_TCB* task = eventGroup->waitingTasks[i];
        // Check if the task should be activated based on the current event bits
        if (task && ((eventGroup->bits & eventBits) == eventBits)) {
            OS_ActivateTask(task);  // Activate the task
            eventGroup->waitingTasks[i] = NULL;  // Remove task from the waiting list
        }
    }
}

/**
 * @brief Clears specified event bits in the event group.
 *
 * This function resets the specified bits in the event group.
 *
 * @param eventGroup Pointer to the OS_EventGroup structure where the bits will be cleared.
 * @param eventBits The event bits to be cleared.
 */
void OS_ClearEventBits(OS_EventGroup* eventGroup, OS_EventGroupBits eventBits) {
    eventGroup->bits &= ~eventBits;  // Clear the specified bits
}
