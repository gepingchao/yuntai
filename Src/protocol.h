#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#include "include.h"

typedef enum
{
	motor_reset = 0X0,
	motor_goto_start_position = 0X1,
	motor_set_mod = 0X2,
	motor_start_show = 0X10,
	motor_pause_show = 0X11,
	motor_stop_show = 0X12,
	motor1_goto_angle = 0X15,
	motor2_goto_angle = 0X16,
	motor_goto_posiation = 0X20,
	save_motor_posiation_info = 0X21,
	save_motor_end_posiation_info = 0X22,
	delete_all_motor_posiation_info = 0X23,
	polling_all_motor_posiation_info = 0X24,
	polling_one_motor_posiation_info = 0X25,
	commit_to_flash = 0X26,
	cancel_operat = 0X27,
	polling_motor_rs485_info = 0X30,

	test_motor_driver= 0X88
	
}E_Motor_Ctrl_Command;

typedef struct
{
	unsigned char owner;
	unsigned char address;
	unsigned char this_protocol_length;
	E_Motor_Ctrl_Command command;
	unsigned char* parameter;
	//unsigned char parameter[512];	
}S_Protocol_Info,*P_S_Protocol_Info;

void deal_protocol(unsigned char* protocol);


#endif

