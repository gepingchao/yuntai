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
	unsigned int x_posiation;
	unsigned int y_posiation;
	unsigned int speed;
	unsigned int time;
}S_DB_Posiation,*P_S_DB_Posiation;


#endif

