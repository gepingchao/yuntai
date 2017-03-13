/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "include.h"
#include "db_flash.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId web_serverHandle;
osThreadId sys_coreHandle;
osThreadId read_keyHandle;
osThreadId deal_keyHandle;
osThreadId serial_serverHandle;
osThreadId flash_serverHandle;
osThreadId motor_serverHandle;
osMessageQId key_valueHandle;
osMessageQId seriel_queueHandle;
osMessageQId flash_queueHandle;
osMessageQId motor_queueHandle;

/* USER CODE BEGIN Variables */
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void web_server_task(void const * argument);
void sys_core_task(void const * argument);
void read_key_task(void const * argument);
void deal_key_task(void const * argument);
void serial_server_task(void const * argument);
void flash_server_task(void const * argument);
void motor_server_task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */
void vApplicationIdleHook(void);
void vApplicationTickHook(void);

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/* USER CODE BEGIN 3 */

/* USER CODE END 3 */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of web_server */
  osThreadDef(web_server, web_server_task, osPriorityAboveNormal, 0, 300);
  web_serverHandle = osThreadCreate(osThread(web_server), NULL);

  /* definition and creation of sys_core */
  osThreadDef(sys_core, sys_core_task, osPriorityNormal, 0, 200);
  sys_coreHandle = osThreadCreate(osThread(sys_core), NULL);

  /* definition and creation of read_key */
  osThreadDef(read_key, read_key_task, osPriorityNormal, 0, 128);
  read_keyHandle = osThreadCreate(osThread(read_key), NULL);

  /* definition and creation of deal_key */
  osThreadDef(deal_key, deal_key_task, osPriorityHigh, 0, 200);
  deal_keyHandle = osThreadCreate(osThread(deal_key), NULL);

  /* definition and creation of serial_server */
  osThreadDef(serial_server, serial_server_task, osPriorityHigh, 0, 128);
  serial_serverHandle = osThreadCreate(osThread(serial_server), NULL);

  /* definition and creation of flash_server */
  osThreadDef(flash_server, flash_server_task, osPriorityAboveNormal, 0, 500);
  flash_serverHandle = osThreadCreate(osThread(flash_server), NULL);

  /* definition and creation of motor_server */
  osThreadDef(motor_server, motor_server_task, osPriorityRealtime, 0, 200);
  motor_serverHandle = osThreadCreate(osThread(motor_server), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of key_value */
  osMessageQDef(key_value, 16, uint16_t);
  key_valueHandle = osMessageCreate(osMessageQ(key_value), NULL);

  /* definition and creation of seriel_queue */
  osMessageQDef(seriel_queue, 6, uint32_t);
  seriel_queueHandle = osMessageCreate(osMessageQ(seriel_queue), NULL);

  /* definition and creation of flash_queue */
  osMessageQDef(flash_queue, 5, uint32_t);
  flash_queueHandle = osMessageCreate(osMessageQ(flash_queue), NULL);

  /* definition and creation of motor_queue */
  osMessageQDef(motor_queue, 8, uint32_t);
  motor_queueHandle = osMessageCreate(osMessageQ(motor_queue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* web_server_task function */
void web_server_task(void const * argument)
{

  /* USER CODE BEGIN web_server_task */
  save_task_info();
  osDelay(2000);
  /* Infinite loop */
  for(;;)
  {
	osDelay(20);
	do_http();
  }
  /* USER CODE END web_server_task */
}

/* sys_core_task function */
void sys_core_task(void const * argument)
{
  /* USER CODE BEGIN sys_core_task */
  save_task_info();
  InitW5500();
  tcp_server(1,3333);
  osDelay(200);
  /*load_flash_to_ram_page(1);
  S_Seek_Require req;
  S_Seek_Result res;
  seek_data(&req,&res);*/
  S_Client_Info client_info;
  client_info.target_server.server_ip[0] = 192;
  client_info.target_server.server_ip[1] = 168;
  client_info.target_server.server_ip[2] = 1;
  client_info.target_server.server_ip[3] = 210;
  client_info.target_server.server_port = 3333;
  client_info.this_client_socket_num = 2;
  tcp_client(&client_info);
  osDelay(200);
  init_db();
  //db_test();
  //adjust_motor(&motor_1_info);
  //operat_motor(1,200.0,360.0,&motor_1_info);
  //adjust_motor(&motor_2_info);
  //operat_motor(1,200.0,360.0,&motor_2_info);
  
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END sys_core_task */
}

/* read_key_task function */
void read_key_task(void const * argument)
{
  /* USER CODE BEGIN read_key_task */
  save_task_info();
  /* Infinite loop */
  for(;;)
  {
	osDelay(20);
	read_key();
	get_current_angle(&motor_1_info);
	get_current_angle(&motor_2_info);
  }
  /* USER CODE END read_key_task */
}

/* deal_key_task function */
void deal_key_task(void const * argument)
{
  /* USER CODE BEGIN deal_key_task */
  	save_task_info();
	unsigned char recv_key_value;	
  /* Infinite loop */
  for(;;)
  {
	xQueueReceive(key_valueHandle, &recv_key_value, portMAX_DELAY);
	deal_key_value(recv_key_value);
   }
  /* USER CODE END deal_key_task */
}

/* serial_server_task function */
void serial_server_task(void const * argument)
{
  /* USER CODE BEGIN serial_server_task */
  save_task_info();
  /* Infinite loop */
  for(;;)
  {
	osDelay(1000);
	//printf_by_web("\r\nhello %d\r\n",12345);
  }
  /* USER CODE END serial_server_task */
}

/* flash_server_task function */
void flash_server_task(void const * argument)
{
  /* USER CODE BEGIN flash_server_task */
  save_task_info();
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END flash_server_task */
}

/* motor_server_task function */
void motor_server_task(void const * argument)
{
  /* USER CODE BEGIN motor_server_task */
    	save_task_info();
	unsigned int recv_motor_command;
  /* Infinite loop */
  for(;;)
  {
  	xQueueReceive(motor_queueHandle, &recv_motor_command, portMAX_DELAY);
	task_motor_server(recv_motor_command);
	//deal_key_value(recv_key_value);
  }
  /* USER CODE END motor_server_task */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
