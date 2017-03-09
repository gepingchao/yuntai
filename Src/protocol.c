#include "include.h"

S_Protocol_Info g_protocol_info;

void motor_goto_angle(P_S_Motor_Info motor_info,float angle)
{	
	float operat_angle;
	operat_angle = angle - motor_info->current_angle;
	if(operat_angle > 0)
		{
			operat_motor(1, 220.0,operat_angle, motor_info);
		}
	if(operat_angle < 0)
		{
			operat_motor(0, 220.0,operat_angle, motor_info);
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

void pretreatment_protocol(unsigned char* protocol,P_S_Protocol_Info protocol_info)//Ԥ����ͨ��֡
{
	protocol_info->address = *(protocol + 2);
	protocol_info->this_protocol_length= *(protocol + 3);
	protocol_info->command= (E_Motor_Ctrl_Command)*(protocol + 4);
	protocol_info->parameter= (protocol + 5);	
}

void execute_protocol(P_S_Protocol_Info protocol_info)
{
	float parameter[5]= {0.0};
	int int_parameter[5] = {0};
	unsigned char recv_parameter[20]={0};
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
				recv_parameter[0] = *(protocol_info->parameter);
				recv_parameter[1] = *(protocol_info->parameter+1);
				int_parameter[0] = (((int)recv_parameter[0])<<8) + recv_parameter[1];
				parameter[0] = (float)int_parameter[0];
				parameter[0] = parameter[0]/10;
				motor_goto_angle(&motor_1_info,parameter[0]);
				break;

			case motor2_goto_angle:
				recv_parameter[0] = *(protocol_info->parameter);
				recv_parameter[1] = *(protocol_info->parameter+1);
				int_parameter[0] = (((int)recv_parameter[0])<<8) + recv_parameter[1];
				parameter[0] = (float)int_parameter[0];
				parameter[0] = parameter[0]/10;
				motor_goto_angle(&motor_2_info,parameter[0]);
				break;

			case motor_goto_posiation:
				
				break;

			case save_motor_posiation_info:
				
				memcopy((char *) (protocol_info->parameter), (char *) recv_parameter, 17);
				int_parameter[0] = (((int)recv_parameter[0])<<24) +(((int)recv_parameter[1])<<16) +(((int)recv_parameter[2])<<8) + recv_parameter[3];
				int_parameter[1] = (((int)recv_parameter[4])<<24) +(((int)recv_parameter[5])<<16) +(((int)recv_parameter[6])<<8) + recv_parameter[7];
				int_parameter[2] = (((int)recv_parameter[8])<<24) +(((int)recv_parameter[9])<<16) +(((int)recv_parameter[10])<<8) + recv_parameter[11];
				int_parameter[3] = recv_parameter[12];
				int_parameter[4] = (((int)recv_parameter[13])<<24) +(((int)recv_parameter[14])<<16) +(((int)recv_parameter[5])<<8) + recv_parameter[16];

				posiation_data.data_type = db_type_posiation;
				posiation_data.direction = int_parameter[3];
				posiation_data.h_posiation = int_parameter[0];
				posiation_data.is_this_data_effect = EFFECT;
				posiation_data.poiation_number = int_parameter[4];
				posiation_data.speed = int_parameter[2];
				posiation_data.time = int_parameter[2];
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
				upload_posiation_data(&db_res,0);
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
				break;
				



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


