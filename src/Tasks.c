/*
  Project   : RA3 RTOS
  Author    : Ali Yasser
  Date      : October 24, 2024
  Version   : 1.0
  Contact   : k4.k4.3li@gmail.com

  Description:
  Implements task management and scheduling for the G RTOS, including a bubble sort
  mechanism for task prioritization, ready queue updates, and idle task processing.
*/

#include <Config.h>
#include <MemManag.h>
#include <Port.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include "Tasks.h"
#include "FIFO.h"

/* Ready Queue for the OS scheduler */
OS_tBuffer ReadyQueue;                 // FIFO buffer for ready tasks
OS_TCB* ReadyQueueFIFO[100];           // Array to hold tasks in ready queue
/* Idle Task Structure */
OS_TCB IdleTask;                       // Control block for the idle task
OS_Control OS_ControlBlock;            // OS Control Block structure to manage system states

/**
 * @brief Bubble sort function to sort tasks based on their priority.
 *
 * @param TaskTable The array of task control blocks (TCBs) to be sorted.
 * @param NoOfCreatedTasks The number of tasks created (size of TaskTable).
 */
void OS_BubbleSort(OS_TCB* TaskTable[], uint8_t NoOfCreatedTasks) {
    for (uint8_t i = 0; i < NoOfCreatedTasks - 1; i++) {
        for (uint8_t j = 0; j < NoOfCreatedTasks - i - 1; j++) {
            if (TaskTable[j]->Priority > TaskTable[j+1]->Priority) {
                // Swap the tasks based on priority
                OS_TCB* temp = TaskTable[j];
                TaskTable[j] = TaskTable[j+1];
                TaskTable[j+1] = temp;
            }
        }
    }
}

/**
 * @brief Sorts the scheduler's task table using the bubble sort algorithm.
 */
void OS_SortSchedulerTable() {
    // Use the bubble sort to sort the task table by priority
    OS_BubbleSort(OS_ControlBlock.TaskTable, OS_ControlBlock.NoOfCreatedTasks);
}

/**
 * @brief Updates the ready queue by enqueuing tasks that are not suspended.
 */
void OS_UpdateReadyQueue() {
    OS_TCB* temp = NULL;
    OS_TCB* CurrentTask = NULL;
    OS_TCB* NextTask = NULL;

    // 1- Clear the ready queue
    while(OS_FifoDequeue(&ReadyQueue, &temp) != FIFO_EMPTY);

    // 2- Update ready queue with tasks that are not suspended
    for(uint8_t i = 0; i < OS_ControlBlock.NoOfCreatedTasks; i++) {
        CurrentTask = OS_ControlBlock.TaskTable[i];
        NextTask = OS_ControlBlock.TaskTable[i+1];

        // Check if the task is not suspended
        if(CurrentTask->TaskState != OS_TASK_SUSPEND) {
            // Add task to ready queue based on priority
            if((NextTask->TaskState == OS_TASK_SUSPEND) || (CurrentTask->Priority < NextTask->Priority)) {
                OS_FifoEnqueue(&ReadyQueue, CurrentTask);
                CurrentTask->TaskState = OS_TASK_READY;
                break;
            }
            if(CurrentTask->Priority == NextTask->Priority) {
                OS_FifoEnqueue(&ReadyQueue, CurrentTask);
                CurrentTask->TaskState = OS_TASK_READY;
            }
        }
    }
}

/**
 * @brief Decides which task to run next based on the ready queue.
 * If no task is ready, the current task continues running, or the idle task is selected.
 */
void OS_DecideNext() {
    // Check if ready queue is empty and if the current task is not suspended
    if((ReadyQueue.counter == 0) && (OS_ControlBlock.CurrentTask->TaskState != OS_TASK_SUSPEND)) {
        // Continue running the current task
        OS_ControlBlock.CurrentTask->TaskState = OS_TASK_RUNNING;
        OS_FifoEnqueue(&ReadyQueue, OS_ControlBlock.CurrentTask);
        OS_ControlBlock.NextTask = OS_ControlBlock.CurrentTask;
    } else {
        // Get the next task from the ready queue
        OS_FifoDequeue(&ReadyQueue, &OS_ControlBlock.NextTask);

        // If no next task is available, set idle task as the next task
        if(!OS_ControlBlock.NextTask) {
            OS_ControlBlock.NextTask = &IdleTask;
        }

        // Set the next task to running state
        OS_ControlBlock.NextTask->TaskState = OS_TASK_RUNNING;

        // Maintain round robin scheduling if priority is equal
        if((OS_ControlBlock.NextTask->Priority == OS_ControlBlock.CurrentTask->Priority) &&
           (OS_ControlBlock.CurrentTask->TaskState != OS_TASK_SUSPEND)) {
            OS_FifoEnqueue(&ReadyQueue, OS_ControlBlock.CurrentTask);
            OS_ControlBlock.CurrentTask->TaskState = OS_TASK_READY;
        }
    }
}

/**
 * @brief Handles system calls (SVC) for task services such as activation, termination, and suspension.
 * @param Stack_Pointer Pointer to the task's stack, which holds the SVC number and parameters.
 */
void OS_SvcServices(uint32_t* Stack_Pointer) {
    // Extract the SVC number from the stack
    uint8_t SVC_ID = *((uint8_t*)(((uint8_t*)Stack_Pointer[6]) - 2));

    switch(SVC_ID) {
        case SVC_ACTIVATE:
        case SVC_TERMINATE:
            // Sort the scheduler table and update the ready queue
            OS_SortSchedulerTable();
            OS_UpdateReadyQueue();

            if(OS_ControlBlock.OS_Mode == OS_RUNNING) {
                if(strcmp(OS_ControlBlock.CurrentTask->TaskName, "IDLE") != 0) {
                    OS_DecideNext();
                    OS_TRIGGER_PENDSV();
                }
            }
        break;

        case SVC_WAITING:
            OS_SortSchedulerTable();
            OS_UpdateReadyQueue();
        break;

        case SVC_SUSPEND:
        break;
    }
}

/**
 * @brief Updates the number of ticks for each task.
 * Decrements the tick count for blocking tasks and requests a wait service if the count reaches zero.
 */
void OS_UpdateNoOfTicks() {
    for(uint8_t i = 0; i < OS_ControlBlock.NoOfCreatedTasks; i++) {
        if(OS_ControlBlock.TaskTable[i]->Waiting.Blocking == OS_TASK_BLOCKING_ENABLE) {
            OS_ControlBlock.TaskTable[i]->Waiting.TicksCount--;
            if(OS_ControlBlock.TaskTable[i]->Waiting.TicksCount == 1) {
                OS_ControlBlock.TaskTable[i]->TaskState = OS_TASK_WAITING;
                OS_ControlBlock.TaskTable[i]->Waiting.Blocking = OS_TASK_BLOCKING_DISABLE;
                OS_REQUEST_SERVICE(SVC_WAITING);  // Request SVC for task state update
            }
        }
    }
}
/**
 * @brief Registers a callback function to be called during the SysTick task's execution.
 * @param callback The function pointer for the SysTick hook callback.
 */
OS_SysTickHook SysTickHook = NULL;
void OS_RegisterSysTickHook(OS_SysTickHook callback) {
    SysTickHook = callback;
}
/**
 * @brief Registers a callback function to be called during the idle task's execution.
 * @param callback The function pointer for the idle hook callback.
 */
uint8_t IdleTaskTest;
OS_IdleHookCallback IdleHookCallback;
void OS_RegisterIdleHook(OS_IdleHookCallback callback) {
    IdleHookCallback = callback;
}

/**
 * @brief The idle task runs when no other task is ready. It can also trigger the idle hook.
 */
void OS_IdleTask() {
    while (1) {
#if OS_IDLE_TASK_HOOK_ENABLED
        if (IdleHookCallback != NULL) {
            IdleHookCallback();  // Call the registered idle hook callback
        }
#endif
        IdleTaskTest ^= 1;  // For testing using logic analyzer
        __asm("WFE");       // Wait for event to enter sleep mode (power efficiency)
    }
}
/**
 * @brief Creates a task and adds it to the scheduler table.
 *
 * @param Task Pointer to the task control block (TCB) that defines the task.
 * @return OS_ErrorStatus Returns the status of the task creation process (OS_OK if successful).
 */
OS_ErrorStatus OS_CreateTask(OS_TCB* Task) {
    // If stackSize is 0, use the default size
    if (Task->StackSize == 0) {
        Task->StackSize = OS_DEFAULT_TASK_STACK_SIZE;
    }

    // Validate the task's priority to ensure it's within the allowed range
    if (Task->Priority < OS_HIGHEST_PRIORITY || Task->Priority > OS_LOWEST_PRIORITY) {
        return OS_PRIORITY_OUT_OF_RANGE;  // Return an error if priority is out of range
    }

    // Allocate stack memory and ensure it's not exceeding the PSP stack region
    Task->_S_PSP_Task = OS_ControlBlock.PSP_LastEnd;
    Task->_E_PSP_Task = Task->_S_PSP_Task - Task->StackSize;

    // Align 8 bytes padding between the task and others to maintain memory alignment
    OS_ControlBlock.PSP_LastEnd = Task->_E_PSP_Task - OS_STACK_PADDING;

    // Create the stack for the task
    OS_CreateStack(Task);

    // Add task to Scheduler table (Waiting Queue)
    OS_ControlBlock.TaskTable[OS_ControlBlock.NoOfCreatedTasks++] = Task;

    // Update task state to Suspended
    Task->TaskState = OS_TASK_SUSPEND;

    return OS_OK;  // Task creation was successful
}

/**
 * @brief Activates a task, changing its state to "Waiting."
 *
 * @param Task Pointer to the task control block (TCB) to be activated.
 * @return OS_ErrorStatus Returns the status of the activation process (OS_OK if successful).
 */
OS_ErrorStatus OS_ActivateTask(OS_TCB* Task) {
    // Change task state from Suspended to Waiting
    Task->TaskState = OS_TASK_WAITING;

    // Request task activation via Supervisor Call (SVC)
    OS_REQUEST_SERVICE(SVC_ACTIVATE);

    return OS_OK;
}

/**
 * @brief Terminates a task, changing its state to "Suspended."
 *
 * @param Task Pointer to the task control block (TCB) to be terminated.
 * @return OS_ErrorStatus Returns the status of the termination process (OS_OK if successful).
 */
OS_ErrorStatus OS_TerminateTask(OS_TCB* Task) {
    // Change task state from Waiting to Suspended
    Task->TaskState = OS_TASK_SUSPEND;

    // Request task termination via Supervisor Call (SVC)
    OS_REQUEST_SERVICE(SVC_TERMINATE);

    return OS_OK;
}

/**
 * @brief Puts a task in a delay state for a specified number of ticks.
 *
 * @param Task Pointer to the task control block (TCB) to be delayed.
 * @param Copy_u32NoOfTicks Number of ticks to delay the task.
 * @return OS_ErrorStatus Returns the status of the delay process (OS_OK if successful).
 */
OS_ErrorStatus OS_DelayTask(OS_TCB* Task, uint32_t Copy_u32NoOfTicks) {
    // Enable task blocking and set the tick count for the delay
    Task->Waiting.Blocking = OS_TASK_BLOCKING_ENABLE;
    Task->Waiting.TicksCount = Copy_u32NoOfTicks;

    // Terminate the task until the delay period is over
    OS_TerminateTask(Task);

    return OS_OK;
}

/**
 * @brief Initializes the operating system.
 *
 * @return OS_ErrorStatus Returns the status of the initialization process (OS_OK if successful).
 */
OS_ErrorStatus OS_Init() {
    OS_ErrorStatus Error = OS_OK;

    // Initialize the hardware (e.g., clocks, peripherals)
    OS_HwInit();

    // Set OS mode to Suspended
    OS_ControlBlock.OS_Mode = OS_SUSPEND;

    // Assign the main stack for the OS
    Error += OS_CreateMainStack();

    // Create the ready queue to store tasks ready for execution
    if (OS_FifoInit(&ReadyQueue, ReadyQueueFIFO, 100) != FIFO_NO_ERROR) {
        Error += FIFO_INIT_ERROR;
    }

    // Initialize the Idle task
    strcpy(IdleTask.TaskName, "IDLE");
    IdleTask.Priority = OS_LOWEST_PRIORITY;    // Lowest priority for idle task
    IdleTask.func = OS_IdleTask;               // Idle task function
    IdleTask.StackSize = 300;                  // Set stack size for idle task
    Error += OS_CreateTask(&IdleTask);         // Create the idle task

    return Error;  // Return any errors encountered during initialization
}

/**
 * @brief Starts the operating system, setting the Idle task as the first task.
 *
 * @return OS_ErrorStatus Returns the status of the startup process (OS_OK if successful).
 */
OS_ErrorStatus OS_StartOS() {
    // 1- Set the OS mode to Running
    OS_ControlBlock.OS_Mode = OS_RUNNING;

    // 2- By default, start the Idle task as the first task
    OS_ControlBlock.CurrentTask = &IdleTask;

    // 3- Activate the Idle task (won't be immediately activated based on its conditions)
    OS_ActivateTask(&IdleTask);

    // 4- Start the system timer
    OS_StartTimer();

    // 5- Set PSP (Process Stack Pointer) to the Idle task's stack
    OS_SET_PSP(OS_ControlBlock.CurrentTask->CurrentPSP);

    // Switch to Process Stack Pointer mode and non-privileged mode
    OS_SWITCH_TO_PSP();
    OS_SWITCH_TO_NOT_PRIVELEGE();

    // Execute the Idle task function (system enters its main loop)
    OS_ControlBlock.CurrentTask->func();

    return OS_OK;  // OS successfully started
}
