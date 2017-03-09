#ifndef __SAVE_DATA_H__
#define __SAVE_DATA_H__
#include "include.h"
#include "db_flash.h"
#define APP_INFOR_SAVE_ADDR 	((FLASH_BASE)+(27*FLASH_PAGE_SIZE))



typedef struct
{
	E_Save_Data_Type data_type;
	unsigned char is_this_data_effect;
	unsigned short poiation_number;//这个成员名字可以自定如ID主要用处查找数据
////////////////////////////////以上成员是必须存在的

	unsigned char direction;
	unsigned int h_posiation;
	unsigned int v_posiation;
	unsigned int speed;
	unsigned int time;
}S_DB_Posiation,*P_S_DB_Posiation;

void reinit_db(void);
void init_db(void);
unsigned char save_posiation_data(P_S_Seek_Result res,P_S_DB_Posiation data);
unsigned char seek_posiation_data(P_S_Seek_Result res,unsigned short poiation_number );
unsigned char delete_posiation_data(P_S_Seek_Result res,P_S_DB_Posiation data);
void reset_reslut_point(P_S_Seek_Result res);
unsigned char pop_posiation_result(P_S_Seek_Result res,P_S_DB_Posiation data);


void upload_posiation_data(P_S_Seek_Result res,unsigned char num);
#endif

