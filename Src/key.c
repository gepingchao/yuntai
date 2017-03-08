#include "include.h"

key_data_def rtos_key;

extern osMessageQId key_valueHandle;


void read_key(void)
{
unsigned char key_trigger_value;
if(GPIO_PIN_RESET == Read_KEY_1) 
  		{
  			rtos_key.press_time[0] = (rtos_key.press_time[0] >0XEE) ? 0XEE : rtos_key.press_time[0] ;
  			rtos_key.press_time[0] ++;
  		}

	  if(GPIO_PIN_RESET == Read_KEY_2) 
  		{
  			rtos_key.press_time[1] = (rtos_key.press_time[1] >0XEE) ? 0XEE : rtos_key.press_time[1] ;
  			rtos_key.press_time[1] ++;
  		}

	 if(GPIO_PIN_RESET == Read_KEY_3) 
  		{
  			rtos_key.press_time[2] = (rtos_key.press_time[2] >0XEE) ? 0XEE : rtos_key.press_time[2] ;
  			rtos_key.press_time[2] ++;
  		}

	if(GPIO_PIN_RESET == Read_KEY_4) 
  		{
  			rtos_key.press_time[3] = (rtos_key.press_time[3] >0XEE) ? 0XEE : rtos_key.press_time[3] ;
  			rtos_key.press_time[3] ++;
  		}

	if(GPIO_PIN_RESET == Read_KEY_5) 
  		{
  			rtos_key.press_time[4] = (rtos_key.press_time[4] >0XEE) ? 0XEE : rtos_key.press_time[4] ;
  			rtos_key.press_time[4] ++;
  		}
	if(GPIO_PIN_RESET == Read_WARN_3) 
  		{
  			rtos_key.press_time[5] = (rtos_key.press_time[5] >0XEE) ? 0XEE : rtos_key.press_time[5] ;
  			rtos_key.press_time[5] ++;
  		}
	if(GPIO_PIN_RESET == Read_WARN_4) 
  		{
  			rtos_key.press_time[6] = (rtos_key.press_time[6] >0XEE) ? 0XEE : rtos_key.press_time[6] ;
  			rtos_key.press_time[6] ++;
  		}
	if(GPIO_PIN_RESET == Read_WARN_5) 
  		{
  			rtos_key.press_time[7] = (rtos_key.press_time[7] >0XEE) ? 0XEE : rtos_key.press_time[7] ;
  			rtos_key.press_time[7] ++;
  		}

	/*for(rtos_key.key_point = 0; rtos_key.key_point <8 ;rtos_key.key_point++)
		{
			if((rtos_key.press_time[rtos_key.key_point ]) > 2)
				{
					rtos_key.key_value[rtos_key.key_point ] = key_press(rtos_key.key_point);				
					if((rtos_key.press_time[rtos_key.key_point ]) > 0X30)
						{
							rtos_key.key_value[rtos_key.key_point ] = key_long_press(rtos_key.key_point);
						}
					if(rtos_key.key_ok[rtos_key.key_point ] == 0)
						{
							rtos_key.key_ok[rtos_key.key_point ] = 1;
							key_trigger_value = rtos_key.key_point;
							xQueueSend(key_valueHandle, &(key_trigger_value), 10);
						}
				}
		}*/

	
	for(rtos_key.key_point = 0; rtos_key.key_point <8 ;rtos_key.key_point++)
		{
			if((rtos_key.press_time[rtos_key.key_point ]) > 2)
				{
					rtos_key.key_value[rtos_key.key_point ] = key_press(rtos_key.key_point);	
						if((rtos_key.key_ok[rtos_key.key_point ]&0X01) == 0)
									{
										rtos_key.key_ok[rtos_key.key_point ] |= 0X01; 
										key_trigger_value =rtos_key.key_point;
										xQueueSend(key_valueHandle, &(key_trigger_value), 10);
									}
					if((rtos_key.press_time[rtos_key.key_point ]) > 0X40)
						{
							rtos_key.key_value[rtos_key.key_point ] = key_long_press(rtos_key.key_point);
							if((rtos_key.key_ok[rtos_key.key_point ]&0X02) == 0)
								{
									rtos_key.key_ok[rtos_key.key_point ] |= 0X02; 
									key_trigger_value =KEY_LONG_TRIGGER(rtos_key.key_point);
									xQueueSend(key_valueHandle, &(key_trigger_value), 10);
								}
						}
					if((rtos_key.press_time[rtos_key.key_point ]) > 0XAF)
						{
							rtos_key.key_value[rtos_key.key_point ] = key_long_long_press(rtos_key.key_point);
							if((rtos_key.key_ok[rtos_key.key_point ]&0X04) == 0)
								{
									rtos_key.key_ok[rtos_key.key_point ] |= 0X04; 
									key_trigger_value =KEY_LONG_LONG_TRIGGER(rtos_key.key_point);
									xQueueSend(key_valueHandle, &(key_trigger_value), 10);
								}
						}

				}
		}



	
	if(GPIO_PIN_SET == Read_KEY_1) 
		{
			if(rtos_key.key_ok[0])
				{
					rtos_key.key_ok[0] = 0;
					xQueueSend(key_valueHandle, &(rtos_key.key_value[0]), 10);
				}
			rtos_key.press_time[0] = 0;
		}
	
	if(GPIO_PIN_SET == Read_KEY_2) 
		{
			if(rtos_key.key_ok[1])
				{
					rtos_key.key_ok[1] = 0;
					xQueueSend(key_valueHandle, &(rtos_key.key_value[1]), 10);
				}
			rtos_key.press_time[1] = 0;
		}

	if(GPIO_PIN_SET == Read_KEY_3) 
		{
			if(rtos_key.key_ok[2])
				{
					rtos_key.key_ok[2] = 0;
					xQueueSend(key_valueHandle, &(rtos_key.key_value[2]), 10);
				}
			rtos_key.press_time[2] = 0;
		}

	if(GPIO_PIN_SET == Read_KEY_4) 
		{
			if(rtos_key.key_ok[3])
				{
					rtos_key.key_ok[3] = 0;
					xQueueSend(key_valueHandle, &(rtos_key.key_value[3]), 10);
				}
			rtos_key.press_time[3] = 0;
		}

	if(GPIO_PIN_SET == Read_KEY_5) 
		{
			if(rtos_key.key_ok[4])
				{
					rtos_key.key_ok[4] = 0;
					xQueueSend(key_valueHandle, &(rtos_key.key_value[4]), 10);
				}
			rtos_key.press_time[4] = 0;
		}

	if(GPIO_PIN_SET == Read_WARN_3) 
		{
			if(rtos_key.key_ok[5])
				{
					rtos_key.key_ok[5] = 0;
					xQueueSend(key_valueHandle, &(rtos_key.key_value[5]), 10);
				}
			rtos_key.press_time[5] = 0;
		}

	if(GPIO_PIN_SET == Read_WARN_4) 
		{
			if(rtos_key.key_ok[6])
				{
					rtos_key.key_ok[6] = 0;
					xQueueSend(key_valueHandle, &(rtos_key.key_value[6]), 10);
				}
			rtos_key.press_time[6] = 0;
		}

	if(GPIO_PIN_SET == Read_WARN_5) 
		{
			if(rtos_key.key_ok[7])
				{
					rtos_key.key_ok[7] = 0;
					xQueueSend(key_valueHandle, &(rtos_key.key_value[7]), 10);
				}
			rtos_key.press_time[7] = 0;
		}

  // LED_4_TOG;
  
  ////////////////////////////////////////////////////// led µ÷¹â²âÊÔ
	//user_LED_G_pwm_setvalue(g_led_G_pwm);
	//g_led_G_pwm += 1;
	//g_led_G_pwm = (g_led_G_pwm > 990? 0 : g_led_G_pwm);
	
	//user_LED_R_pwm_setvalue(300);
	//g_led_R_pwm -= 15;
	//g_led_R_pwm = (g_led_R_pwm < 30? 600 : g_led_R_pwm);
	//user_LED_B_pwm_setvalue(900);
	//g_led_B_pwm += 6;
	//g_led_B_pwm = (g_led_B_pwm > 400? 0 : g_led_B_pwm);

}

unsigned char deal_key_value(unsigned key_value)
{
	//static unsigned short freq_2 = 1200;
	//static unsigned short freq_3 = 100;
	switch(key_value)
  		{
  			case(KEY_1_TRIGGER):
				//LED_5_TOG;
				break;
			case(KEY_2_TRIGGER):
				//LED_5_TOG;
				break;
			case(KEY_3_TRIGGER):
				//LED_5_TOG;
				break;
			case(KEY_4_TRIGGER):
				//LED_5_TOG;
				break;
			case(KEY_5_TRIGGER):
				//LED_5_TOG;
				break;
			case(KEY_6_TRIGGER):

				break;
			case(KEY_7_TRIGGER):

				break;
			case(KEY_8_TRIGGER):

				break;
			case(KEY_9_TRIGGER):
				//LED_5_TOG;
				break;
			case(KEY_10_TRIGGER):
				//LED_5_TOG;
				break;

			case(KEY_1_LONG_TRIGGER):
				break;

			case(KEY_2_LONG_TRIGGER):
				break;

			case(KEY_3_LONG_TRIGGER):
				break;

			case(KEY_4_LONG_TRIGGER):
				break;

			case(KEY_5_LONG_TRIGGER):
				break;

			case(KEY_6_LONG_TRIGGER):
				break;

			case(KEY_7_LONG_TRIGGER):
				break;

			case(KEY_8_LONG_TRIGGER):
				break;

			case(KEY_9_LONG_TRIGGER):
				break;

			case(KEY_10_LONG_TRIGGER):
				break;

			case(KEY_1_LONG_LONG_TRIGGER):
				break;
			
			case(KEY_2_LONG_LONG_TRIGGER):
				break;
			
			case(KEY_3_LONG_LONG_TRIGGER):
				break;
			
			case(KEY_4_LONG_LONG_TRIGGER):
				break;
			
			case(KEY_5_LONG_LONG_TRIGGER):
				break;

			case(KEY_6_LONG_LONG_TRIGGER):
				break;

			case(KEY_7_LONG_LONG_TRIGGER):
				break;

			case(KEY_8_LONG_LONG_TRIGGER):
				break;

			case(KEY_9_LONG_LONG_TRIGGER):
				break;

			case(KEY_10_LONG_LONG_TRIGGER):
				break;
				


				
  			case(key1_press):
					LED_TOG;
				
				break;

			case(key1_long_press):
				
				break;

			case(key1_long_long_press):

				break;
				
			case(key2_press):
				LED_TOG;
				break;

			case(key2_long_press):
				break;
				
			case(key3_press):
				LED_TOG;
				//adjust_motor(&motor_1_info);
				//remove_gap(&motor_1_info,2.8);
				operat_motor(1,28000.0,1000.0,&motor_1_info);
				operat_motor(1,28000.0,1000.0,&motor_2_info);
				osDelay(5000);
				operat_motor(1,28500.0,8000.0,&motor_1_info);
				operat_motor(1,28500.0,8000.0,&motor_2_info);
				osDelay(5000);
				operat_motor(1,29000.0,9000.0,&motor_1_info);
				operat_motor(1,29000.0,9000.0,&motor_2_info);
				osDelay(5000);
				operat_motor(1,29500.0,9000.0,&motor_1_info);
				operat_motor(1,29500.0,9000.0,&motor_2_info);
				osDelay(5000);
				operat_motor(1,30000.0,7200.0,&motor_1_info);
				operat_motor(1,30000.0,7200.0,&motor_2_info);
				osDelay(5000);
				//adjust_motor(&motor_2_info);
				//operat_motor(1,250.0,360.0,&motor_2_info);
				//HAL_Delay(2000);
				break;

			case(key3_long_press):
				//insert_time_table(10,50,11,10,0XFF);
				SET_ENABLE_1_TOG;
				SET_ENABLE_2_TOG;
				break;
				
			case(key4_press):
				break;

			case(key4_long_press):
			
				break;
				
			case(key5_press):
				break;

			case(key5_long_long_press):
				break;
				
			case(warn3_press):
				break;

			case(warn3_long_press):
				break;

			case(warn3_long_long_press):
				break;

			case(warn4_press):
				break;

			case(warn4_long_press):
				break;

			case(warn4_long_long_press):
				break;

			case(warn5_press):
				break;

			case(warn5_long_press):
				break;

			case(warn5_long_long_press):
				break;
				
  		}
	return 1;
}


