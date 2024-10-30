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
		OS_DelayTask(&t1, 100);
	}


}
void task2 (){
	static uint8_t count = 0;
	while(1){
		Task2Led ^= 1;
		OS_DelayTask(&t2, 100);
	}
}
void task3 (){
	static uint8_t count = 0;
	while(1){
		Task3Led ^= 1;
		count++;
		if(count == 30){
			count=0;
			OS_ActivateTask(&t4);
		}
		OS_DelayTask(&t3, 500);

	}
}
void task4 (){
	static uint8_t count = 0;
	while(1){
		Task4Led ^= 1;
		OS_DelayTask(&t4, 1000);

	}
}

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void)
{


  HAL_Init();


  SystemClock_Config();


  MX_GPIO_Init();

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

  ERROR= OS_ActivateTask(&t2);
  if(ERROR != OS_OK)
	  while(1);

  ERROR= OS_ActivateTask(&t3);
  if(ERROR != OS_OK)
	  while(1);

  OS_StartOS();

  while (1)
  {

  }
  /* USER CODE END 3 */
}#include "main.h"
#include <stdint.h>
#include <stddef.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
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
		OS_DelayTask(&t1, 100);
	}


}
void task2 (){
	static uint8_t count = 0;
	while(1){
		Task2Led ^= 1;
		OS_DelayTask(&t2, 100);
	}
}
void task3 (){
	static uint8_t count = 0;
	while(1){
		Task3Led ^= 1;
		count++;
		if(count == 30){
			count=0;
			OS_ActivateTask(&t4);
		}
		OS_DelayTask(&t3, 500);

	}
}
void task4 (){
	static uint8_t count = 0;
	while(1){
		Task4Led ^= 1;
		OS_DelayTask(&t4, 1000);

	}
}


void SystemClock_Config(void);
static void MX_GPIO_Init(void);


int main(void)
{

  HAL_Init();


  SystemClock_Config();

  MX_GPIO_Init();
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

  ERROR= OS_ActivateTask(&t2);
  if(ERROR != OS_OK)
	  while(1);

  ERROR= OS_ActivateTask(&t3);
  if(ERROR != OS_OK)
	  while(1);

  OS_StartOS();

  while (1)
  {

 }
}