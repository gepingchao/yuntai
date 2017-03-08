#include "include.h"



unsigned int g_motor_1_mesg;
unsigned int g_motor_2_mesg;

S_Motor_Info motor_1_info;
S_Motor_Info motor_2_info;


extern osMessageQId key_valueHandle;
extern osMessageQId seriel_queueHandle;
extern osMessageQId flash_queueHandle;
extern osMessageQId motor_queueHandle;





void user_pwm_setvalue(TIM_HandleTypeDef *htim,uint16_t value)
{
    TIM_OC_InitTypeDef sConfigOC;
 
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = value;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
    HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);   
}


unsigned int set_timer(TIM_HandleTypeDef* timer,unsigned int frequency)
{
  unsigned short loopx;
  unsigned short tmp_pres = 0;
  for(loopx = 2; loopx <1000 ;loopx ++)
  	{
  		if(frequency < (1100/(loopx +1)))
  			{
  				continue;
  			}
		else
			{
				tmp_pres = loopx;
        break;
			}
  	}
  unsigned int tmp_value;
  tmp_value = ((72000000/(tmp_pres +1))/frequency);
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  timer->Init.Prescaler = tmp_pres;
  timer->Init.Period = tmp_value;
  if (HAL_TIM_PWM_Init(timer) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(timer, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(timer, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }


  HAL_TIM_MspPostInit(timer);
  return tmp_value;
}	


void set_timer_2(unsigned short frequency)
{
  unsigned int tmp_value;
  tmp_value = (1000000/frequency) - 1;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 71;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = tmp_value;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }


  HAL_TIM_MspPostInit(&htim2);

}


void set_timer_3(unsigned short frequency)
{
  unsigned int tmp_value;
  tmp_value = (1000000/frequency) - 1;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 71;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = tmp_value;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_MspPostInit(&htim3);

}

unsigned int change_speed_2_freq(float speed)
{
//10000  ->  226.98 '/s
//44..065 -> 1'/s
	//unsigned short tmp_speed;
	//tmp_speed = (unsigned short)(speed * 44.056);
	//return tmp_speed;

return (unsigned int)speed;
}



/************
GEAR_REDUCTION_RATIO  齿轮组减速比
IC_REDUCTION_RATIO	驱动IC细分
PLUSE_PER_CRICLE 步进电机转一圈使用的脉冲数 (360/步进角)
根据使用的电机和减速器设置
*****************************/

#define GEAR_REDUCTION_RATIO 1
#define IC_REDUCTION_RATIO 16
#define PLUSE_PER_CRICLE 200


unsigned int change_angle_2_freq_default(float angle)
{
	float reduction_ratio = GEAR_REDUCTION_RATIO * IC_REDUCTION_RATIO*PLUSE_PER_CRICLE;
	unsigned int freq;
	//freq = (angle * reduction_ratio * 357) /(18*360);//如果需要软件校准
	freq = (angle/360) * reduction_ratio;
	return freq;
}

unsigned int change_angle_2_freq_user(P_S_Motor_Info motor_info,float angle)
{
	float tmp = 0.0;
	unsigned int freq = 0;
	tmp = angle / 360.0;
	freq = motor_info->adjust * tmp;
	return freq;
}

void motor_run(P_S_Motor_Info motor_info)
{
	motor_info->timer_value = set_timer(motor_info->timer,motor_info->speed);	
	osDelay(1);
	if(1 == motor_info->direction)
		{
			if(1 == motor_info ->motor_num)
				{
					SET_CCW1_1;
					SET_ENABLE_1_1;
				}
			if(2 == motor_info ->motor_num)
				{
					SET_CCW2_1;
					SET_ENABLE_2_1;
				}
		}
	if(0 == motor_info->direction)
		{
			if(1 == motor_info ->motor_num)
				{
					SET_CCW1_0;
					SET_ENABLE_1_1;
				}
			if(2 == motor_info ->motor_num)
				{
					SET_CCW2_0;
					SET_ENABLE_2_1;
				}
		}
	user_pwm_setvalue(motor_info->timer,(motor_info->timer_value)/2);
	//user_pwm_setvalue(motor_info->timer,200);
	HAL_TIM_Base_Start_IT(motor_info->timer);
}

void task_motor_server(unsigned char command)
{
	switch(command)
		{
			case motor1_run_out:
				motor_1_info.posiation_lock = INVALID;
				SET_ENABLE_1_0;				
				HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);  
				HAL_TIM_Base_Stop_IT(&htim3);
				
				break;

			case motor2_run_out:
				motor_2_info.posiation_lock = INVALID;
				SET_ENABLE_2_0;				
				HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);  
				HAL_TIM_Base_Stop_IT(&htim2);

				break;

			case motor1_start_run:
				motor_run(&motor_1_info);
				break;

			case motor2_start_run:
				motor_run(&motor_2_info);
				break;

			case motor1_stop:
				SET_ENABLE_1_0;				
				HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);  
				HAL_TIM_Base_Stop_IT(&htim3);
				motor_1_info.finish_pulse = motor_1_info.pulse_count;
				motor_1_info.pulse_count = 0;
				break;

			case motor2_stop:
				SET_ENABLE_2_0;
				HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1); 
				HAL_TIM_Base_Stop_IT(&htim2);
				motor_2_info.finish_pulse = motor_2_info.pulse_count;
				motor_2_info.pulse_count = 0;
				break;
				
			case motor1_limit_trigger:
				break;

			case motor2_limit_trigger:
				break;

			default :

				break;
		}
}


void init_motor_info(void)
{
	motor_1_info.timer = &htim3;
	motor_1_info.motor_num = 1;
	motor_2_info.timer = &htim2;
	motor_2_info.motor_num = 2;
}


void operat_motor(unsigned char direction,float speed,float angle,P_S_Motor_Info motor_info)//direction 放向 speed速度单位度每秒angle角度单位度
{
	motor_info->direction = direction;
	motor_info->speed = change_speed_2_freq(speed);
	if(1 == motor_info->is_this_motor_adjusted)
		{
			motor_info->distance = change_angle_2_freq_user(motor_info,angle);
		}
	else
		{
			motor_info->distance = change_angle_2_freq_default(angle);
		}
	if(1 == motor_info->motor_num)
		{
			if(speed < 0.00001)
				{
					g_motor_1_mesg = motor1_stop;
					xQueueSend(motor_queueHandle,&g_motor_1_mesg, 0xFFFF);
				}
			else
				{
					g_motor_1_mesg = motor1_start_run;
					xQueueSend(motor_queueHandle,&g_motor_1_mesg, 0xFFFF);
				}
			return;
		}
	if(2 == motor_info->motor_num)
		{
			if(speed < 0.00001)
				{
					g_motor_2_mesg = motor2_stop;
					xQueueSend(motor_queueHandle,&g_motor_2_mesg, 0xFFFF);
				}
			else
				{
					g_motor_2_mesg = motor2_start_run;
					xQueueSend(motor_queueHandle,&g_motor_2_mesg, 0XFFFF);	
				}
			return;
		}

}


void adjust_motor(P_S_Motor_Info motor_info)
{
	
	unsigned int tick_start = 0;
	unsigned int adjust_time = 4000;
	tick_start = HAL_GetTick();
	motor_info->is_this_motor_adjusted = 0;
	motor_info->adjust = 0;
	operat_motor(0,8000.0,24.0,motor_info);
	osDelay(500);
	operat_motor(1,8000.0,720.0,motor_info);//电机开始正转至限位处
	
	while((HAL_GetTick() - tick_start) < adjust_time)//规定时间内无法完成电机校准则认为电机失效
		{
			if(1 == motor_info->motor_num)//校准电机1
				{
					if(Read_KEY_1 == GPIO_PIN_RESET)//电机转到限位处
						{
							//SET_ENABLE_1_0;//电机停止转动,定时器停止运转
							//HAL_TIM_Base_Stop_IT(&htim3);
							operat_motor(0,0,24.0,motor_info);
							osDelay(1);
							if(0 == motor_info->is_this_motor_adjusted)
								{
									operat_motor(0,1200.0,720.0,motor_info);//电机开始反转,到限位处停止
									motor_info->is_this_motor_adjusted = 1;
									motor_info->adjust = 0;
									motor_info->finish_pulse = 0; 
									osDelay(200);
									continue;
								}
							if(1 == motor_info->is_this_motor_adjusted)
								{
									motor_info->adjust = motor_info->finish_pulse;//记录转动整整一圈用的周期数
									motor_info->is_this_motor_operational = 1;
									motor_info->current_position = 0;
									break;
								}
						}
				}
			if(2 == motor_info->motor_num)//校准电机2
				{
					if(Read_KEY_2 == GPIO_PIN_RESET)//电机转到限位处
						{
							//SET_ENABLE_2_0;//电机停止转动,定时器停止运转
							//HAL_TIM_Base_Stop_IT(&htim2);
							operat_motor(0,0,24.0,motor_info);
							osDelay(1);
							if(0 == motor_info->is_this_motor_adjusted)
								{
									operat_motor(0,1200.0,720.0,motor_info);//电机开始反转,到限位处停止
									motor_info->is_this_motor_adjusted = 1;
									motor_info->adjust = 0;
									motor_info->finish_pulse = 0; 
									osDelay(200);
									continue;
								}
							if(1 == motor_info->is_this_motor_adjusted)
								{
									motor_info->adjust = motor_info->finish_pulse;//记录转动整整一圈用的周期数
									motor_info->is_this_motor_operational = 1;
									motor_info->current_position = 0;
									break;
								}
						}
				}
		}
	motor_info->is_this_motor_adjusted = 1;
	if((motor_info->adjust < 0x3D00)||(motor_info->adjust > 0x3F00))
		{
			motor_info->is_this_motor_adjusted = 0;//校准不准确
		}
	if((HAL_GetTick() - tick_start) > adjust_time)
		{
			motor_info->is_this_motor_operational = 0;//电机运转不正常
		}	
}



float get_current_angle(P_S_Motor_Info motor_info)
{
	float tmp_angle;
	unsigned int completely_pulse;
	
	if(1 == motor_info->is_this_motor_adjusted)
		{
			completely_pulse = motor_info->adjust;
		}
	else
		{
			completely_pulse = 0X3DF8;
		}
	tmp_angle = 360 * ((float)(motor_info->current_position) - (float)(completely_pulse))/(float)(completely_pulse);
	motor_info->current_angle = tmp_angle;
	return tmp_angle;
}


void remove_gap(P_S_Motor_Info motor_info,float gap)
{
	if(1 == motor_info->direction)
		{
			motor_info->posiation_lock = EFFECT;
			operat_motor(0,220.0,gap,motor_info);
			osDelay(15);
		}
	if(0 == motor_info->direction)
		{
			motor_info->posiation_lock = EFFECT;
			operat_motor(1,220.0,gap,motor_info);
			osDelay(15);
		}
		
}

