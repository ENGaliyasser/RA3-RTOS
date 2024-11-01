/*
  Project   : RA3 RTOS
  Author    : Ali Yasser
  Date      : June 7, 2024
  Version   : 1.0
  Contact   : k4.k4.3li@gmail.com
*/

#include "Config.h"
#include <MemManag.h>
#include "Tasks.h"

/*
 * @brief: Externed from startup code; represents the top of the stack.
 */
extern uint32_t _estack;

/**
 * @brief  Initialize the stack for a new task (create a Process Stack Pointer - PSP).
 *
 * @param  Task: Pointer to the task's control block (OS_TCB structure).
 *
 * @retval OS_ErrorStatus: Returns OS_OK if the task stack was initialized successfully.
 *
 * @details
 * This function sets up the initial stack frame for a task in PSP mode.
 * It pushes dummy values onto the stack, which will be used for context switching later.
 * These dummy values include the task's XPSR, PC (function pointer), LR, and registers R0-R12.
 */
OS_ErrorStatus OS_CreateStack(OS_TCB* Task) {

    // Set PSP (Process Stack Pointer) to the task's starting PSP.
    Task->CurrentPSP = Task->_S_PSP_Task;

    // XPSR (Execution Program Status Register): Set the T-bit (bit 24) for Thumb state.
    Task->CurrentPSP--;
    *(Task->CurrentPSP) = 0x01000000;

    // PC (Program Counter): Set to the address of the task's function.
    Task->CurrentPSP--;
    *(Task->CurrentPSP) = (uint32_t)Task->func;

    // LR (Link Register): Set to return to Thread mode and use PSP (0xFFFFFFFD).
    Task->CurrentPSP--;
    *(Task->CurrentPSP) = 0xFFFFFFFD;

    // Initialize general-purpose registers R12, R3, R2, R1, and R0 with dummy values (0).
    for (uint32_t i = 0; i < 13; i++) {
        Task->CurrentPSP--;
        *(Task->CurrentPSP) = 0;
    }

    return OS_OK;
}

/**
 * @brief  Initialize the main stack (create a Main Stack Pointer - MSP).
 *
 * @retval OS_ErrorStatus: Returns OS_OK if the main stack was initialized successfully.
 *
 * @details
 * This function initializes the stack used by the main thread (MSP).
 * It allocates space for the main stack and aligns the stack pointer for the first task.
 */
OS_ErrorStatus OS_CreateMainStack() {
    OS_ErrorStatus error = OS_OK;

    // Main Stack Pointer (MSP) top set to the end of stack (estack).
    OS_ControlBlock._S_MSP_Task = &_estack;

    // Set the end of the main stack based on the main stack size.
    OS_ControlBlock._E_MSP_Task = OS_ControlBlock._S_MSP_Task - OS_MAIN_STACK_SIZE;

    // Align the Process Stack Pointer (PSP) for the first task with 8-byte padding.
    OS_ControlBlock.PSP_LastEnd = (OS_ControlBlock._E_MSP_Task - OS_STACK_PADDING);

    return error;
}
