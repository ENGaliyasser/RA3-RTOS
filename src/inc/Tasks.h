/*
  Project   : G RTOS
  Author    : Ali Yasser
  Date      : October 24, 2024
  Version   : 1.0
  Contact   : k4.k4.3li@gmail.com


*/
#ifndef INC_TASK_H_
#define INC_TASK_H_

#include <stdint.h>
#include <stddef.h>


// Enumeration for task auto-start options
typedef enum {
    noAutoStart,
    AutoStart
} OS_TaskAutoStart;

// Structure defining a task
typedef struct {
    uint8_t Priority;              // Task priority
    uint8_t TaskName[30];          // Name of the task
    uint16_t StackSize;            // Size of the task stack
    void (*func)(void);            // Pointer to the task function
    OS_TaskAutoStart AutoStart; // Auto-start option
    // Internal state management
    struct {
        enum {
            OS_TASK_BLOCKING_DISABLE,
            OS_TASK_BLOCKING_ENABLE
        } Blocking;                // Blocking state
        uint32_t TicksCount;      // Tick count for waiting
    } Waiting;
    uint32_t _S_PSP_Task;         // Start of task stack
    uint32_t _E_PSP_Task;         // End of task stack
    uint32_t* CurrentPSP;         // Current Process Stack Pointer
    enum {
        OS_TASK_SUSPEND,
        OS_TASK_WAITING,
        OS_TASK_READY,
        OS_TASK_RUNNING
    } TaskState;                 // Current state of the task
} OS_TCB;

// Enumeration for error statuses
typedef enum {
    OS_OK,
    OS_EXCEED_AVAILABLE_STACK,
    FIFO_INIT_ERROR,
    TASK_CREATION_ERROR,
	OS_PRIORITY_OUT_OF_RANGE
} OS_ErrorStatus;

// Stack padding definition
#define OS_STACK_PADDING 8

// Macro for service call request
#define OS_REQUEST_SERVICE(SVC_ID)  __asm volatile ("SVC %[SVCid]" : : [SVCid] "i" (SVC_ID));

// Structure defining the operating system attributes
typedef struct {
    uint8_t NoOfCreatedTasks;      // Number of created tasks
    uint32_t _S_MSP_Task;          // Start of main stack
    uint32_t _E_MSP_Task;          // End of main stack
    uint32_t PSP_LastEnd;          // End of last PSP allocated
    enum {
        OS_SUSPEND,
        OS_RUNNING
    } OS_Mode;                // Current OS mode
    OS_TCB* CurrentTask;    // Pointer to the current task
    OS_TCB* NextTask;       // Pointer to the next task
    OS_TCB* TaskTable[100]; // Table of all tasks in the system
} OS_Control;

// Extern declaration for OS_StructOS
extern OS_Control OS_ControlBlock;

// Enumeration for service call IDs
typedef enum {
    SVC_ACTIVATE,
    SVC_TERMINATE,
    SVC_WAITING,
    SVC_SUSPEND,
    SVC_ACQUIRE_MUTEX,
    SVC_RELEASE_MUTEX
} OS_SvcID; // Service Call IDs

typedef void (*OS_IdleHookCallback)(void);
typedef void (*OS_SysTickHook)(void);
extern OS_SysTickHook SysTickHook;
extern OS_IdleHookCallback IdleHookCallback;
// Function declarations
void OS_SortSchedulerTable();
void OS_UpdateReadyQueue();
void OS_DecideNext();
void OS_SvcServices(uint32_t* Stack_Pointer);
void OS_UpdateNoOfTicks();
void OS_RegisterIdleHook(OS_IdleHookCallback callback);
void OS_IdleTask();
OS_ErrorStatus OS_Init();
OS_ErrorStatus OS_CreateTask(OS_TCB* Task);
OS_ErrorStatus OS_ActivateTask(OS_TCB* Task);
OS_ErrorStatus OS_TerminateTask(OS_TCB* Task);
OS_ErrorStatus OS_DelayTask(OS_TCB* Task, uint32_t NoOfTicks);
OS_ErrorStatus OS_StartOS();

#endif /* INC_TASK_H_ */

