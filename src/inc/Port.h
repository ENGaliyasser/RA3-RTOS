/*
  Project   : G RTOS
  Author    : Ali Yasser
  Date      : October 24, 2024
  Version   : 1.0
  Contact   : k4.k4.3li@gmail.com

*/
#ifndef INC_CORTEXM_OS_PORTING_H_
#define INC_CORTEXM_OS_PORTING_H_

#include <stdint.h>
#include <stddef.h>
#include "stm32f103xb.h"
#include "core_cm3.h"


/**
 * @brief Macro to set the PSP (Process Stack Pointer) to a specified address.
 * @param add Address to set as the PSP.
 */
#define OS_SET_PSP(add)                 __asm volatile("MOV R0, %[stackValue] \n\t MSR PSP, R0" : : [stackValue] "r" (add))
/**
 * @brief Macro to get the current PSP (Process Stack Pointer) value and store it in 'add'.
 * @param add Variable where the PSP value will be stored.
 */
#define OS_GET_PSP(add)                 __asm volatile("MRS R0, PSP \n\t MOV %[stackReturnedValue], R0" : [stackReturnedValue] "=r" (add))
/**
 * @brief Macro to switch the processor to use the PSP (Process Stack Pointer) for execution.
 */
#define OS_SWITCH_TO_PSP()             __asm("MRS R3, CONTROL \n\t ORR R3, R3, #0x2 \n\t MSR CONTROL, R3")
/**
 * @brief Macro to switch the processor to use the MSP (Main Stack Pointer) for execution.
 */
#define OS_SWITCH_TO_MSP()             __asm("MRS R3, CONTROL \n\t BIC R3, R3, #0x2 \n\t MSR CONTROL, R3")
/**
 * @brief Macro to switch the processor to privileged mode.
 */
#define OS_SWITCH_TO_PRIVELEGE()      __asm("MRS R3, CONTROL \n\t BIC R3, R3, #0x1 \n\t MSR CONTROL, R3")
/**
 * @brief Macro to switch the processor to non-privileged mode.
 */
#define OS_SWITCH_TO_NOT_PRIVELEGE()  __asm("MRS R3, CONTROL \n\t ORR R3, R3, #0x1 \n\t MSR CONTROL, R3")
/**
 * @brief Macro to trigger a PendSV exception.
 */
#define OS_TRIGGER_PENDSV()           SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;


void OS_HwInit();
void OS_StartTimer();
#endif /* INC_CORTEXM_OS_PORTING_H_ */
