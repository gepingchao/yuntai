#include "include.h"

extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart1;
void UsartReceive_IDLE(UART_HandleTypeDef *huart)  
{  
	if(huart == &huart2)
	  	{
		    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
		    {   
		        __HAL_UART_CLEAR_IDLEFLAG(&huart2);  
			//HAL_TIM_Base_Start_IT(&htim2);
		        //UsartType3.rx_len =  RECEIVELEN - temp;   
		        //UsartType3.receive_flag=1;  
		        //HAL_UART_Receive_DMA(&huart3,UsartType3.usartDMA_rxBuf,RECEIVELEN);  
		    }  
	  	}
}



