#include "main.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "Tasks.h"
#include "Semaphore.h"

OS_Semaphore s1, s2;
OS_TCB t1,t2,t3,t4;
uint8_t Task1Led,Task2Led,Task3Led,Task4Led;
void task1 (){
	static uint8_t count = 0;
	while(1){
		Task1Led ^= 1;
		count++;
		if(count == 100){
			OS_ActivateTask(&t2);
		}
		if(count == 200){
			count = 0;
		}

	}
}
void task2 (){
	static uint8_t count = 0;
	while(1){
		Task2Led ^= 1;
		count++;
		if(count == 100){
			OS_ActivateTask(&t3);
		}
		if(count == 200){
			OS_TerminateTask(&t2);
			count = 0;
		}
	}
}
void task3 (){
	static uint8_t count = 0;
	while(1){
		Task3Led ^= 1;
		count++;
		if(count == 100){
			OS_ActivateTask(&t4);
		}
		if(count == 200){
			count = 0;
			OS_TerminateTask(&t3);
		}

	}
}
void task4 (){
	static uint8_t count = 0;
	while(1){
		Task4Led ^= 1;
		count++;
		if(count == 100){
			count = 0;
			OS_TerminateTask(&t4);
		}

	}
}
/* USER CODE END Includes */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  	// Variable to define error status
  OS_ErrorStatus ERROR = OS_OK;

  OS_Init();
  if(ERROR != OS_OK)
  	while(1);

  OS_InitSemaphore(&s1, 1);
  OS_InitSemaphore(&s2, 1);
  t1.func = task1;
  t1.Priority = 4 ;
  strcpy(t1.TaskName,"Task 1");
  t1.StackSize = 1024;

  ERROR = OS_CreateTask(&t1);
  if(ERROR != OS_OK)
		while(1);

  t2.func = task2;
  t2.Priority = 3 ;
  strcpy(t2.TaskName,"Task 2");
  t2.StackSize = 1024;

  ERROR = OS_CreateTask(&t2);
  if(ERROR != OS_OK)
	  while(1);

  t3.func = task3;
  t3.Priority = 2 ;
  strcpy(t3.TaskName,"Task 3");
  t3.StackSize = 1024;

  ERROR = OS_CreateTask(&t3);
  if(ERROR != OS_OK)
	  while(1);

  t4.func = task4;
  t4.Priority = 1 ;
  strcpy(t4.TaskName,"Task 4");
  t4.StackSize = 1024;

  ERROR = OS_CreateTask(&t4);
  if(ERROR != OS_OK)
	  while(1);

  ERROR= OS_ActivateTask(&t1);
  if(ERROR != OS_OK)
	  while(1);



  OS_StartOS();

  while (1)
  {

  }
}