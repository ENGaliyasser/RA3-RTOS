/*
  Project   : RA3 RTOS
  Author    : Ali Yasser
  Date      : October 24, 2024
  Version   : 1.0
  Contact   : k4.k4.3li@gmail.com

*/

#ifndef INC_MEMMANAG_H_
#define INC_MEMMANAG_H_




#include <Tasks.h>

OS_ErrorStatus OS_CreateMainStack();
OS_ErrorStatus OS_CreateStack(OS_TCB* Task);
#endif /* INC_MEMMANAG_H_ */
