#include "include.h"
#include "db_flash.h"

void reinit_db(void)
{
	reinit_data_map();
	creat_database(db_type_user_define_posiation,sizeof(S_DB_Posiation));
}


void init_db(void)
{
	init_data_map();
	creat_database(db_type_user_define_posiation,sizeof(S_DB_Posiation));
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
	req.data_type = db_type_user_define_posiation;
	
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
