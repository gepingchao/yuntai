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

	TIM_HandleTypeDef* timer;

	unsigned int timer_value;
	
	unsigned int adjust;
	int speed;
	float current_position;
	float current_angle;
	unsigned int distance;
	unsigned int pulse_count;
	unsigned int finish_pulse;
	
}S_Motor_Info,*P_S_Motor_Info;
extern S_Motor_Info motor_1_info;
extern S_Motor_Info motor_2_info;











void user_pwm_setvalue(TIM_HandleTypeDef *htim,uint16_t value);
void set_timer_2(unsigned short frequency);
void set_timer_3(unsigned short frequency);
unsigned int set_timer(TIM_HandleTypeDef* timer,unsigned int frequency);
void task_motor_server(unsigned char command);

/////////////////////////////////////////////////////////////////////////////�����ǿ��Ե��õĺ���


void init_motor_info(void);//��ʼ���������״̬�ṹ��
void operat_motor(unsigned char direction,float speed,float angle,P_S_Motor_Info motor_info);//�����������ת��/ֹͣ
void adjust_motor(P_S_Motor_Info motor_info);//У׼�������
float get_current_angle(P_S_Motor_Info motor_info);//��ȡ��ǰ���������ת���Ƕ�
void remove_gap(P_S_Motor_Info motor_info,float gap);//������ת��϶  gap��ת��϶�� һ��ȡ2-3.5

#endif

