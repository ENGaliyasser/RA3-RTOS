/*
  Project   : G RTOS
  Author    : Ali Yasser
  Date      : June 7, 2024
  Version   : 1.0
  Contact   : k4.k4.3li@gmail.com

  Description:
  Implements port configurations and control for the STM32F1 ARM Cortex-M3,
  optimized for embedded systems and real-time applications.
*/
#include <Config.h>
#include <Port.h>
#include "Tasks.h"







uint8_t SystickLed;

/* FAULT HANDLERS : Useful for debugging */
void NMI_Handler(void) {
    while (1) {}
}

void HardFault_Handler(void) {
    while (1) {}
}

void MemManage_Handler(void) {
    while (1) {}
}

void BusFault_Handler(void) {
    while (1) {}
}

void UsageFault_Handler(void) {
    while (1) {}
}

/* SysTick Handler for OS tick update and context switching */
void SysTick_Handler(void) {
	SystickLed ^= 1;       // Toggle the SysTick LED to visually verify operation
	OS_UpdateNoOfTicks();           // Update the OS tick count
#if OS_TICK_HOOK_ENABLED
	if (SysTickHook != NULL) {
	    SysTickHook();           // Call the SysTick hook, if registered
	}
#endif
	OS_DecideNext();                // Determine the next task to run
#if OS_PREEMPTION_ENABLED
    OS_TRIGGER_PENDSV();   // Trigger PendSV only if preemption is enabled
#endif
}
            // Trigger PendSV for context switching
/* SVC Handler
 * This function identifies the current stack pointer (PSP or MSP),
 * retrieves the next instruction location, and calls the service handler.
 */
__attribute ((naked)) void SVC_Handler(void) {
    __asm(
        "TST LR, #4       \n\t"
        "ITE EQ           \n\t"
        "MRSEQ R0, MSP    \n\t"
        "MRSNE R0, PSP    \n\t"
        "B OS_SvcServices"
    );
}

/* OS hardware initialization function */
void OS_HwInit() {
    /* Set PendSV priority to match SysTick priority */
    __NVIC_SetPriority(PendSV_IRQn, 15);
}

/* Start OS timer for scheduling */
void OS_StartTimer() {
    /* SysTick setup: 72 MHz CPU clock, 1ms tick (72000 counts) */
	uint32_t Count = (OS_CPU_CLOCK_FREQ_IN_HZ / 1000) * OS_TICK_TIME_IN_MS;
    SysTick_Config(Count);
}

/* PendSV Handler for context switching between tasks
 * Handles saving and restoring task contexts (PSP, registers R4-R11)
 */
__attribute((naked)) void PendSV_Handler(void)
{
	/* Context Switching */
	/* Save the context of current */
	if (OS_ControlBlock.NextTask != NULL){
		/* 1- Get current PSP
		 * the returned PSP is after the CPU pushes XPSR, LR, PC, R0->R3,R12 */
		OS_GET_PSP(OS_ControlBlock.CurrentTask->CurrentPSP);
		/* 2- Push R4-> R11 manually */
		OS_ControlBlock.CurrentTask->CurrentPSP--;
		__asm volatile("MOV %0 , R4":"=r"(*(OS_ControlBlock.CurrentTask->CurrentPSP)));
		OS_ControlBlock.CurrentTask->CurrentPSP--;
		__asm volatile("MOV %0 , R5":"=r"(*(OS_ControlBlock.CurrentTask->CurrentPSP)));
		OS_ControlBlock.CurrentTask->CurrentPSP--;
		__asm volatile("MOV %0 , R6":"=r"(*(OS_ControlBlock.CurrentTask->CurrentPSP)));
		OS_ControlBlock.CurrentTask->CurrentPSP--;
		__asm volatile("MOV %0 , R7":"=r"(*(OS_ControlBlock.CurrentTask->CurrentPSP)));
		OS_ControlBlock.CurrentTask->CurrentPSP--;
		__asm volatile("MOV %0 , R8":"=r"(*(OS_ControlBlock.CurrentTask->CurrentPSP)));
		OS_ControlBlock.CurrentTask->CurrentPSP--;
		__asm volatile("MOV %0 , R9":"=r"(*(OS_ControlBlock.CurrentTask->CurrentPSP)));
		OS_ControlBlock.CurrentTask->CurrentPSP--;
		__asm volatile("MOV %0 , R10":"=r"(*(OS_ControlBlock.CurrentTask->CurrentPSP)));
		OS_ControlBlock.CurrentTask->CurrentPSP--;
		__asm volatile("MOV %0 , R11":"=r"(*(OS_ControlBlock.CurrentTask->CurrentPSP)));

		/* Restore the context of current */

		OS_ControlBlock.CurrentTask = OS_ControlBlock.NextTask;
		OS_ControlBlock.NextTask = 	NULL;
		/* The last value of the CurrentPSP of the current task is that is
		 * pointing to R11 */
		/* 1- Restore manually pushed registers */

		__asm volatile("MOV R11 , %0"::"r"(*(OS_ControlBlock.CurrentTask->CurrentPSP)));
		OS_ControlBlock.CurrentTask->CurrentPSP++;
		__asm volatile("MOV R10 , %0"::"r"(*(OS_ControlBlock.CurrentTask->CurrentPSP)));
		OS_ControlBlock.CurrentTask->CurrentPSP++;
		__asm volatile("MOV R9 , %0"::"r"(*(OS_ControlBlock.CurrentTask->CurrentPSP)));
		OS_ControlBlock.CurrentTask->CurrentPSP++;
		__asm volatile("MOV R8 , %0"::"r"(*(OS_ControlBlock.CurrentTask->CurrentPSP)));
		OS_ControlBlock.CurrentTask->CurrentPSP++;
		__asm volatile("MOV R7 , %0"::"r"(*(OS_ControlBlock.CurrentTask->CurrentPSP)));
		OS_ControlBlock.CurrentTask->CurrentPSP++;
		__asm volatile("MOV R6 , %0"::"r"(*(OS_ControlBlock.CurrentTask->CurrentPSP)));
		OS_ControlBlock.CurrentTask->CurrentPSP++;
		__asm volatile("MOV R5 , %0"::"r"(*(OS_ControlBlock.CurrentTask->CurrentPSP)));
		OS_ControlBlock.CurrentTask->CurrentPSP++;
		__asm volatile("MOV R4 , %0"::"r"(*(OS_ControlBlock.CurrentTask->CurrentPSP)));
		OS_ControlBlock.CurrentTask->CurrentPSP++;

		/* 2- Set PSP to the the task */
		OS_SET_PSP(OS_ControlBlock.CurrentTask->CurrentPSP);
		/* Now the CPU can restore automatically */
	}
	__asm volatile("BX LR");
}
