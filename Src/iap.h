#ifndef __IAP_H__
#define __IAP_H__

#include "Include.h"

#define  cMaxRAM  200
#define  MAX_DATALENGTH  150

#define DEVICE_SOFT_VER  20160414

#define app_base 		0X11
#define app_base_addr 0X8000000

#define app_1 		0X22
#define app_1_addr 	0X801C000

#define app_2  		0X33
#define app_2_addr   	0X8028000

#define app_3 		0X44
#define app_3_addr 	0X8034000

#define no_jump  		0X88
#define try_jump_flag	0X89




///////////////////////////////////////////////////////////////////////需要根据情况更改
#define THIS_APP  app_base
#define THIS_APP_ADDR  app_base_addr


#define NEXT_APP		app_1
#define WRITE_TO_ADDR app_1_addr
///////////////////////////////////////////////////////////////////////



typedef struct
{
	unsigned int cur_app_offset;
	unsigned char app_num;
	unsigned char app_point;
	unsigned char data_effect;
	unsigned char tem_jump;
	unsigned int jump_to_app;
	unsigned int try_to_jump;
	unsigned int app_offset_table[6];	
}APP_INFORMATION;

typedef struct 
{
	unsigned char page:4;
	unsigned char block;
	unsigned char cur_page_size;
	unsigned char type;
	unsigned int total_size;
	unsigned int recved_page_sum;
	unsigned int recved_page;
	unsigned int request_page;
}S_Wifi_Iap_Info,*p_S_Wifi_Iap_Info;


typedef struct
{
	unsigned char cmd;
	unsigned char parameter_1;
	unsigned char parameter_2;
	unsigned char parameter_3;
	unsigned char parameter_4;
	unsigned char parameter_5;
}S_Wifi_Iap_Cmd,*P_S_Wifi_Iap_Cmd;


extern S_Wifi_Iap_Info wifi_iap_info;

//////////IAP.Page_buffer较大,设计可以作为公用缓存,一下宏指引当前使用缓存的地方
#define OWNER_ON_ONE			0X00
#define OWNER_IAP_BUFFER		0X01
#define OWNER_FLASH_READER 	0X02
#define OWNER_RAM_BUFFER		0X03
#define OWNER_WIFI_IAP_BUFFER	0X04


#define WIFI_IAP_OPERAT_START			0X01
#define WIFI_IAP_OPERAT_START_BACK		0X02
#define WIFI_IAP_OPERAT_BEGIN			0X03
#define WIFI_IAP_OPERAT_BEGIN_BACK		0X04

#define WIFI_IAP_OPERAT_END				0X05
#define WIFI_IAP_OPERAT_TRANSMISSION 	0X07

void test_jump(void);
void disable_all_irq(void);
void SystemClock_Config(void);

void jump_to_app(unsigned int app_addr);

void write_iap_buff(unsigned int length);

void try_to_jump(unsigned char app);
void reset_iap_buff(void);
void read_app_info(void);
void write_app_info(void);
void choose_app(void);
void init_save_data(void);


void wifi_iap_request_page(unsigned int page);
void wifi_iap_operater(void);
void wifi_iap_cmd_operat(P_S_Wifi_Iap_Cmd cmd_parameter);
void wifi_iap_send_ready_code(void);
void wifi_iap_write_to_flash(void);
void wifi_iap_send_end_code(void);
void wifi_iap_copy_to_flash(void);

void start_app(void);


#endif


