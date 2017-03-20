#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "include.h"


  typedef enum
  {
  	motor1_run_out = 1,
	motor2_run_out,
	motor1_start_run,
	motor2_start_run,
	motor1_stop,
	motor2_stop,
	motor1_start_up,
	motor1_halt,
	motor2_start_up,
	motor2_halt,
	motor1_limit_trigger,
	motor2_limit_trigger
  } E_Motor_Command;



extern unsigned int g_motor_1_mesg;
extern unsigned int g_motor_2_mesg;

typedef struct
{
	unsigned char motor_num;
	unsigned char direction;
	unsigned char is_this_motor_adjusted;
	unsigned char is_this_motor_operational;//当前电机运行是否正常
	unsigned char posiation_lock;//位置锁   当转向更改时对位置加锁,消除间隙的脉冲不改变当前位置
	unsigned char start_up_flag;
	TIM_HandleTypeDef* timer;

	unsigned int timer_value;
	
	unsigned int adjust;
	int inc_pulse;
	int dynamic_speed;
	int speed;
	float current_position;
	float current_angle;
	float unsigned_posiation;//0-360-0
	float unsigned_current_angle;
	unsigned int distance;
	unsigned int pulse_count;
	unsigned int finish_pulse;
	
}S_Motor_Info,*P_S_Motor_Info;
extern S_Motor_Info motor_1_info;
extern S_Motor_Info motor_2_info;











void user_pwm_setvalue(TIM_HandleTypeDef *htim,uint16_t value);
unsigned int set_timer(TIM_HandleTypeDef* timer,unsigned int frequency);
void dynamic_timer(P_S_Motor_Info motor);

void task_motor_server(unsigned char command);

/////////////////////////////////////////////////////////////////////////////下面是可以调用的函数


void init_motor_info(void);//初始化步进电机状态结构体
void operat_motor(unsigned char direction,float speed,float angle,P_S_Motor_Info motor_info);//操作步进电机转动/停止
void adjust_motor(P_S_Motor_Info motor_info);//校准步进电机
float get_current_angle(P_S_Motor_Info motor_info);//获取当前步进电机的转动角度
void remove_gap(P_S_Motor_Info motor_info,float gap);//消除反转间隙  gap反转间隙角 一般取2-3.5

#endif

