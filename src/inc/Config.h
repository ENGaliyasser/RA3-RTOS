/*
  Project   : G RTOS
  Author    : Ali Yasser
  Date      : October 24, 2024
  Version   : 1.0
  Contact   : k4.k4.3li@gmail.com

*/


#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

// Define macro for OS preemption control
#define OS_PREEMPTION_ENABLED 1  // Set to 1 to enable, 0 to disable preemption

// Size of the main stack in bytes
#define OS_MAIN_STACK_SIZE           3072

// Default stack size for tasks in bytes
#define OS_DEFAULT_TASK_STACK_SIZE    1024

// Time duration of each tick in milliseconds
#define OS_TICK_TIME_IN_MS           1

// CPU clock frequency in hertz
#define OS_CPU_CLOCK_FREQ_IN_HZ      72000000  // 72 MHz

// Lowest priority level for tasks
#define OS_LOWEST_PRIORITY            255

// Highest priority level for tasks
#define OS_HIGHEST_PRIORITY           0

// Enable/disable the tick task hook
#define OS_TICK_HOOK_ENABLED     1

// Enable/disable the idle task hook
#define OS_IDLE_TASK_HOOK_ENABLED     1

#endif /* INC_CONFIG_H_ */
