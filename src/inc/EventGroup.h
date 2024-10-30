/*
  Project   : G RTOS
  Author    : Ali Yasser
  Date      : October 24, 2024
  Version   : 1.0
  Contact   : k4.k4.3li@gmail.com

  Description:
  Header file for Event Group management in an embedded RTOS.
*/

#ifndef EVENT_GROUP_H
#define EVENT_GROUP_H

#include <stdint.h>
#include "Tasks.h"

#define OS_EVENT_GROUP_OK               0
#define OS_EVENT_GROUP_TIMEOUT          1
#define OS_EVENT_GROUP_ERROR            2

typedef uint32_t OS_EventGroupBits;  // 32-bit event group bits

/** Structure for Event Group */
typedef struct {
    OS_EventGroupBits bits;              // Holds the event flags
    OS_TCB* waitingTasks[10];            // Array of tasks waiting on this event group
    uint8_t numWaitingTasks;             // Number of tasks currently waiting
} OS_EventGroup;

/** Event Group function prototypes */
void OS_InitEventGroup(OS_EventGroup* eventGroup);
uint8_t OS_WaitForEventBits(OS_EventGroup* eventGroup, OS_EventGroupBits eventBits, uint8_t waitForAllBits, uint32_t timeout);
void OS_SetEventBits(OS_EventGroup* eventGroup, OS_EventGroupBits eventBits);
void OS_ClearEventBits(OS_EventGroup* eventGroup, OS_EventGroupBits eventBits);

#endif // EVENT_GROUP_H
