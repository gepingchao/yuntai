#include "include.h"
#include "db_flash.h"

void reinit_db(void)
{
	unsigned char loopx;
	for(loopx = 0 ; loopx < DB_DATA_PAGE_NUMBER ; loopx ++)
		{
			erase_flash(DB_DATA_PAGE_ADRESS(loopx));
		}
	reinit_data_map();
	creat_database(db_type_posiation,sizeof(S_DB_Posiation));
}


void init_db(void)
{
	init_data_map();
	creat_database(db_type_posiation,sizeof(S_DB_Posiation));
	//creat_database(db_type_user_define_2,sizeof(demo));
	//creat_database(db_type_user_define_3,sizeof(demo));
	//creat_database(db_type_user_define_4,sizeof(demo));//根据实际情况建表
}

unsigned char save_posiation_data(P_S_Seek_Result res,P_S_DB_Posiation data)
{
	S_Seek_Require req;
	req.compare_member_length = sizeof(data->poiation_number);
	req.compare_member_offset = offsetof(S_DB_Posiation,poiation_number);
	req.compare_value = data->poiation_number;
	req.compare_type = compare_equel;

	req.data_length = sizeof(S_DB_Posiation);
	req.data_type = data->data_type;
	
	req.effect_flags_length = 1;
	req.effect_flags_offset = offsetof(S_DB_Posiation,is_this_data_effect);
	req.effect_flags_value = data->is_this_data_effect;
	req.expect_seek_num = 1;
	req.start_compare_offset = 0;

	return save_data(&req,res, (void *) data);
}

unsigned char seek_posiation_data(P_S_Seek_Result res,unsigned short poiation_number )
{
	S_Seek_Require req;
	req.compare_member_length = sizeof(unsigned short);
	req.compare_member_offset = offsetof(S_DB_Posiation,poiation_number);
	req.compare_value = poiation_number;
	req.compare_type = compare_equel;

	req.data_length = sizeof(S_DB_Posiation);
	req.data_type = db_type_posiation;
	
	req.effect_flags_length = 1;
	req.effect_flags_offset = offsetof(S_DB_Posiation,is_this_data_effect);
	req.effect_flags_value = EFFECT;
	req.expect_seek_num = 20;
	req.start_compare_offset = 0;
	return seek_data(&req, res);
}

unsigned char delete_posiation_data(P_S_Seek_Result res,P_S_DB_Posiation data)
{
	S_Seek_Require req;
	req.compare_member_length = sizeof(data->poiation_number);
	req.compare_member_offset = offsetof(S_DB_Posiation,poiation_number);
	req.compare_value = data->poiation_number;
	req.compare_type = compare_equel;

	req.data_length = sizeof(S_DB_Posiation);
	req.data_type = data->data_type;
	
	req.effect_flags_length = 1;
	req.effect_flags_offset = offsetof(S_DB_Posiation,is_this_data_effect);
	req.effect_flags_value = data->is_this_data_effect;
	req.expect_seek_num = 1;
	req.start_compare_offset = 0;
	return delete_data(&req, res);
}

void reset_reslut_point(P_S_Seek_Result res)
{
	res->point = 0;
}

unsigned char pop_posiation_result(P_S_Seek_Result res,P_S_DB_Posiation data)
{
	if((res->point) < (res->equal_count))
		{
			mem_copy((char*)(res->result[res->point++]),(char*)data,sizeof(S_DB_Posiation));
			return 1;
		}
	return 0;	
}

void upload_posiation_data(P_S_Seek_Result res,unsigned char num)
{
	static unsigned char msg[40];
	S_DB_Posiation data;
	if(num > res->equal_count)
		{
			return;//参数错误
		}
	memcopy((char *)(res->result[num]),(char *)&data, sizeof(S_DB_Posiation));
	msg[0] = 0XAA;
	msg[1] = 0X90;
	msg[2] = 0X00;
	msg[3] = 27;
	msg[4] = 0X25;
	msg[5] = (data.h_posiation >> 24)&0XFF;
	msg[6] = (data.h_posiation >> 16)&0XFF;
	msg[7] = (data.h_posiation >> 8)&0XFF;
	msg[8] = (data.h_posiation >> 0)&0XFF;
	msg[9] = (data.v_posiation >> 24)&0XFF;
	msg[10] = (data.v_posiation >> 16)&0XFF;
	msg[11] = (data.v_posiation >> 8)&0XFF;
	msg[12] = (data.h_posiation >> 0)&0XFF;
	msg[13] = (data.start_time>> 24)&0XFF;
	msg[14] = (data.start_time>> 16)&0XFF;
	msg[15] = (data.start_time>> 8)&0XFF;
	msg[16] = (data.start_time>> 0)&0XFF;
	if(data.direction == 0XFF)
		{
			msg[17] = 0X1;
		}
	if(data.direction == 0)
		{
			msg[17] = 0X00;
		}
	msg[18] = (data.runing_time>> 24)&0XFF;
	msg[19] = (data.runing_time>> 16)&0XFF;
	msg[20] = (data.runing_time>> 8)&0XFF;
	msg[21] = (data.runing_time>> 0)&0XFF;
	msg[22] = (data.poiation_number>> 24)&0XFF;
	msg[23] = (data.poiation_number>> 16)&0XFF;
	msg[24] = (data.poiation_number>> 8)&0XFF;
	msg[25] = (data.poiation_number>> 0)&0XFF;
	SetCRC8Sub((unsigned char *)msg,26);
	send(2,msg,27);
}


