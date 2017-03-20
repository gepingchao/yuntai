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
	unsigned char is_this_motor_operational;//��ǰ��������Ƿ�����
	unsigned char posiation_lock;//λ����   ��ת�����ʱ��λ�ü���,������϶�����岻�ı䵱ǰλ��
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

/////////////////////////////////////////////////////////////////////////////�����ǿ��Ե��õĺ���


void init_motor_info(void);//��ʼ���������״̬�ṹ��
void operat_motor(unsigned char direction,float speed,float angle,P_S_Motor_Info motor_info);//�����������ת��/ֹͣ
void adjust_motor(P_S_Motor_Info motor_info);//У׼�������
float get_current_angle(P_S_Motor_Info motor_info);//��ȡ��ǰ���������ת���Ƕ�
void remove_gap(P_S_Motor_Info motor_info,float gap);//������ת��϶  gap��ת��϶�� һ��ȡ2-3.5

#endif

