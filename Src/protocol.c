#include "include.h"

S_Protocol_Info g_protocol_info;

void motor_goto_angle(P_S_Motor_Info motor_info,float angle)
{	
	float operat_angle;
	operat_angle = angle - motor_info->current_angle;
	if(operat_angle > 0)
		{
			operat_motor(1, 8000.0,operat_angle, motor_info);
		}
	if(operat_angle < 0)
		{
			operat_motor(0, 8000.0,-operat_angle, motor_info);
		}
}

void motor_goto_angle_inspeed(P_S_Motor_Info motor_info,float angle,unsigned char speed)
{	
	float operat_angle,motor_speed;
	motor_speed = speed*200 + 200;
	operat_angle = angle - motor_info->current_angle;
	if(operat_angle > 0)
		{
			operat_motor(1, motor_speed,operat_angle, motor_info);
		}
	if(operat_angle < 0)
		{
			operat_motor(0, motor_speed,-operat_angle, motor_info);
		}
}

void motor_goto_angle_intime(P_S_Motor_Info motor_info,float angle,unsigned int time)
{	
	float operat_angle,motor_speed,runing_time;
	runing_time = ((float)time);
	
	//motor_speed = speed*200 + 200;
	operat_angle = angle - motor_info->current_angle;
	motor_speed = (operat_angle / runing_time)*44444.44;
	//motor_goto_angle_inspeed(motor_info,angle, motor_speed);
	if(operat_angle > 0)
		{
			operat_motor(1, motor_speed,operat_angle, motor_info);
		}
	if(operat_angle < 0)
		{
			operat_motor(0, -motor_speed,-operat_angle, motor_info);
		}
}

void motor_goto_point(P_S_DB_Posiation posiation_point)
{
	unsigned char direction;
	unsigned int h_posiation;
	unsigned int v_posiation;
	unsigned int runing_time;
	unsigned int time;
	float f_h_angle,f_v_angle;
	
	direction = posiation_point->direction;
	h_posiation = posiation_point->h_posiation;
	v_posiation = posiation_point->v_posiation;
	runing_time= posiation_point->runing_time;
	time= posiation_point->start_time;
  
  
	f_h_angle = ((float)h_posiation)/10;
	f_v_angle = ((float)v_posiation)/10;
	
	
	//motor_goto_angle_inspeed(&motor_1_info,f_h_angle,speed);
	//motor_goto_angle_inspeed(&motor_2_info,f_v_angle,speed);
	motor_goto_angle_intime(&motor_1_info,f_h_angle,runing_time);
	motor_goto_angle_intime(&motor_2_info,f_v_angle,runing_time);
}

void motor_jump_point(unsigned short posiation_point)
{
	S_Seek_Result res;
	set_mem((unsigned char*)&res,0,sizeof(res));
	if(seek_posiation_data(&res, posiation_point))
		{
			motor_goto_point((P_S_DB_Posiation)res.result[0]);
		}
	
}

unsigned char adjust_protocol(unsigned char* protocol)
{
	unsigned char data_length = 0;
	data_length = *(protocol + 3);
	if(VerifyCRC8Sub(protocol,data_length))
		{
			return 1;
		}
	else
		{
			return 0;
		}
	
}

void pretreatment_protocol(unsigned char* protocol,P_S_Protocol_Info protocol_info)//预处理通信帧
{
	protocol_info->address = *(protocol + 2);
	protocol_info->this_protocol_length= *(protocol + 3);
	protocol_info->command= (E_Motor_Ctrl_Command)*(protocol + 4);
	protocol_info->parameter= (protocol + 5);	
}

void execute_protocol(P_S_Protocol_Info protocol_info)
{
	float parameter[5]= {0.0};
	int int_parameter[10] = {0};
	unsigned char recv_parameter[40]={0};
	S_DB_Posiation posiation_data;
	static S_Seek_Result db_res;
	if((machine_info.this_machime_adress != protocol_info->address)&&(0XFF != protocol_info->address))
		{
			return;
		}
	switch(protocol_info->command)
		{
			case motor_reset:
				adjust_motor(&motor_1_info);
				adjust_motor(&motor_2_info);
				break;
				
			case motor_goto_start_position:
				break;
				
			case motor_set_mod:
				break;

			case motor_start_show:
				break;

			case motor_pause_show:
				break;

			case motor_stop_show:
				break;

			case motor1_goto_angle:
				memcopy((char *) (protocol_info->parameter), (char *) recv_parameter, 2);
				int_parameter[0] = (((int)recv_parameter[0])<<8) + recv_parameter[1];
				parameter[0] = (float)int_parameter[0];
				parameter[0] = parameter[0]/10;
				motor_goto_angle(&motor_1_info,parameter[0]);
				break;

			case motor2_goto_angle:
				memcopy((char *) (protocol_info->parameter), (char *) recv_parameter, 2);
				int_parameter[0] = (((int)recv_parameter[0])<<8) + recv_parameter[1];
				parameter[0] = (float)int_parameter[0];
				parameter[0] = parameter[0]/10;
				motor_goto_angle(&motor_2_info,parameter[0]);
				break;

			case motor_goto_posiation:
				memcopy((char *) (protocol_info->parameter), (char *) recv_parameter, 2);
				int_parameter[0] = (((int)recv_parameter[0])<<8) + recv_parameter[1];
				if(0XFF == int_parameter[0])
					{
						return;
					}
				motor_jump_point(int_parameter[0]);
				
				break;

			case save_motor_posiation_info:
				
				memcopy((char *) (protocol_info->parameter), (char *) recv_parameter, 21);
				int_parameter[0] = (((int)recv_parameter[0])<<24) +(((int)recv_parameter[1])<<16) +(((int)recv_parameter[2])<<8) + recv_parameter[3];
				int_parameter[1] = (((int)recv_parameter[4])<<24) +(((int)recv_parameter[5])<<16) +(((int)recv_parameter[6])<<8) + recv_parameter[7];
				int_parameter[2] = (((int)recv_parameter[8])<<24) +(((int)recv_parameter[9])<<16) +(((int)recv_parameter[10])<<8) + recv_parameter[11];
				int_parameter[3] = recv_parameter[12];
				int_parameter[4] = (((int)recv_parameter[13])<<24) +(((int)recv_parameter[14])<<16) +(((int)recv_parameter[15])<<8) + recv_parameter[16];
				int_parameter[5] = (((int)recv_parameter[17])<<24) +(((int)recv_parameter[18])<<16) +(((int)recv_parameter[19])<<8) + recv_parameter[20];
				posiation_data.data_type = db_type_posiation;
				posiation_data.direction = int_parameter[3];
				posiation_data.h_posiation = int_parameter[0];
				posiation_data.is_this_data_effect = EFFECT;
				posiation_data.poiation_number = int_parameter[5];
				posiation_data.runing_time= int_parameter[4];
				posiation_data.start_time= int_parameter[2];
				posiation_data.v_posiation = int_parameter[1];

				save_posiation_data(&db_res,&posiation_data);
				break;

			case save_motor_end_posiation_info:
				break;

			case delete_all_motor_posiation_info:
				reinit_db();
				break;
				
			case polling_all_motor_posiation_info:
				break;
				
			case polling_one_motor_posiation_info:
				memcopy((char *) (protocol_info->parameter), (char *) recv_parameter, 1);
				//int_parameter[0] = (((int)recv_parameter[0])<<24) +(((int)recv_parameter[1])<<16) +(((int)recv_parameter[2])<<8) + recv_parameter[3];
				
				set_mem((unsigned char * )&db_res, 0,sizeof(S_Seek_Result));
				seek_posiation_data(&db_res, recv_parameter[0] );
				if(db_res.equal_count > 0)
					{
						upload_posiation_data(&db_res,0);
					}
				break;
				
			case commit_to_flash:
				db_commit();
				break;

			case cancel_operat:
				break;
			
			case polling_motor_rs485_info:
				break;

			case test_motor_driver:
				recv_parameter[0] = *(protocol_info->parameter);
				recv_parameter[1] = *(protocol_info->parameter+1);
				recv_parameter[2] = *(protocol_info->parameter+2);
				recv_parameter[3] = *(protocol_info->parameter+3);
				recv_parameter[4] = *(protocol_info->parameter+4);
				int_parameter[0] = (((int)recv_parameter[0])<<16)+(((int)recv_parameter[1])<<8) + recv_parameter[2];
				int_parameter[1] = (((int)recv_parameter[3])<<8) + recv_parameter[4];
				parameter[0] = (float)int_parameter[0];
				parameter[1] = (float)int_parameter[1];
				parameter[1] = parameter[1]/10;
				operat_motor(1,parameter[0],parameter[1],&motor_1_info);
				operat_motor(1,parameter[0],parameter[1],&motor_2_info);

				//motor_goto_angle_intime(&motor_2_info,parameter[0]/10 ,int_parameter[1] );
				break;
				
			case test_motor_driver_time:
				memcopy((char *) (protocol_info->parameter), (char *) recv_parameter, 6);
				int_parameter[0] = (((int)recv_parameter[0])<<8) + recv_parameter[1];
				int_parameter[1] = (((int)recv_parameter[2])<<8) + recv_parameter[3];
				int_parameter[2] = (((int)recv_parameter[4])<<8) + recv_parameter[5];
				parameter[1] = ((float)int_parameter[1])/10;
				parameter[2] = ((float)int_parameter[2])/10;
				motor_goto_angle_intime(&motor_1_info,parameter[1],int_parameter[0]);
				motor_goto_angle_intime(&motor_2_info,parameter[2],int_parameter[0]);			
				


			default :
				break;
		}
}

void deal_protocol(unsigned char* protocol)
{
	if(0 == adjust_protocol(protocol))
		{
			return;
		}
	pretreatment_protocol(protocol,&g_protocol_info);
	execute_protocol(&g_protocol_info);
}


