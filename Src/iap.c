#include "include.h"


APP_INFORMATION App_info;
S_Wifi_Iap_Info wifi_iap_info;

typedef  void (*pFunction)(void);
pFunction JumpToApplication;
uint32_t JumpAddress;

void test_jump(void)
{
	HAL_Init();	
	disable_all_irq();
	SystemClock_Config();
	if (((*(__IO uint32_t*)app_3_addr) & 0x2FFE0000 ) == 0x20000000)
				   {
				      /* Jump to user application */
				      JumpAddress = *(__IO uint32_t*) (app_3_addr + 4);
				     JumpToApplication = (pFunction) JumpAddress;
				      /* Initialize user application's Stack Pointer */
				      __set_MSP(*(__IO uint32_t*) app_3_addr);
				      JumpToApplication();
				    } 
	
}

void jump_to_app(unsigned int app_addr)
{
	HAL_Init();
	SystemClock_Config();	
	disable_all_irq();
	HAL_RCC_DeInit();
	if (((*(__IO uint32_t*)app_addr) & 0x2FFE0000 ) == 0x20000000)
				   {
				      /* Jump to user application */
				      JumpAddress = *(__IO uint32_t*) (app_addr + 4);
				     JumpToApplication = (pFunction) JumpAddress;
				      /* Initialize user application's Stack Pointer */
				      __set_MSP(*(__IO uint32_t*) app_addr);
				      JumpToApplication();
				    } 
	
}


void disable_all_irq(void)
{
	unsigned char loopx;
	for(loopx = 0;loopx< 60;loopx ++)
		{			
			HAL_NVIC_DisableIRQ((IRQn_Type)loopx);
			NVIC_ClearPendingIRQ((IRQn_Type)loopx);
		}
}

void try_to_jump_to_next_app(void)
{
	try_to_jump(NEXT_APP);
}
	

void try_to_jump(unsigned char app)
{
	switch(app)
		{
			case app_base:
			App_info.try_to_jump = app_base_addr;
			App_info.app_offset_table[0] = 0;
			break;

			case app_1:
			App_info.try_to_jump = app_1_addr;	
			App_info.app_offset_table[1] = 0;
			break;

			case app_2:
			App_info.try_to_jump = app_2_addr;	
			App_info.app_offset_table[2] = 0;
			break;

			case app_3:
			App_info.try_to_jump = app_3_addr;
			App_info.app_offset_table[3] = 0;
			break;
			
			default :
			App_info.try_to_jump = app_base_addr;
			App_info.app_offset_table[0] = 0;
			break;
		}
	App_info.tem_jump = try_jump_flag;
	write_app_info();
	NVIC_SystemReset();
}

void read_app_info(void)
{
	read_flash(APP_INFOR_SAVE_ADDR, (char *)(&App_info), sizeof(App_info));
}

void write_app_info(void)
{
	write_flash(APP_INFOR_SAVE_ADDR, (char *)(&App_info), sizeof(App_info));
}

void choose_app(void)
{
	if((App_info.tem_jump == try_jump_flag)||(App_info.tem_jump == 0XFF))
		{
			App_info.tem_jump = no_jump;
			if(App_info.try_to_jump == THIS_APP_ADDR)
				{
					App_info.jump_to_app = App_info.try_to_jump;
					App_info.try_to_jump = 0X01234567;
					write_app_info();
					return;
				}
			else
				{					
					write_app_info();
					/////jump to try_to_jump app
					jump_to_app(App_info.try_to_jump);
				}
		}
	else
		{
			if((App_info.try_to_jump == app_base_addr)||(App_info.try_to_jump == app_1_addr)||(App_info.try_to_jump == app_2_addr)||(App_info.try_to_jump == app_3_addr))
				{
					if(App_info.try_to_jump == THIS_APP_ADDR)   //跳转成功
						{
							App_info.jump_to_app = App_info.try_to_jump;
							App_info.try_to_jump = 0X01234567;
							write_app_info();
							return;
						}
					else		//跳转失败
						{
							App_info.try_to_jump = 0X01234567;
							write_app_info();
							jump_to_app(App_info.jump_to_app);
						}
				}
			if(App_info.jump_to_app == THIS_APP_ADDR)
				{
					if(App_info.jump_to_app == app_base_addr)
						{
							if(App_info.app_offset_table[0] != App_info.jump_to_app )
								{
									App_info.app_offset_table[0] = App_info.jump_to_app;
									write_app_info();
								}
						}
					if(App_info.jump_to_app == app_1_addr)
						{
							if(App_info.app_offset_table[1] != App_info.jump_to_app )
								{
									App_info.app_offset_table[1] = App_info.jump_to_app;
									write_app_info();
								}
						}
					if(App_info.jump_to_app == app_2_addr)
						{
							if(App_info.app_offset_table[2] != App_info.jump_to_app )
								{
									App_info.app_offset_table[2] = App_info.jump_to_app;
									write_app_info();
								}
						}
					if(App_info.jump_to_app == app_3_addr)
						{
							if(App_info.app_offset_table[3] != App_info.jump_to_app )
								{
									App_info.app_offset_table[3] = App_info.jump_to_app;
									write_app_info();
								}
						}
					
					return;
				}
			if((App_info.jump_to_app != app_base_addr)&&(App_info.jump_to_app != app_1_addr)&&(App_info.jump_to_app != app_2_addr)&&(App_info.jump_to_app != app_3_addr)) //修改跳转bug

				{	
					App_info.jump_to_app = THIS_APP_ADDR;
					write_app_info();
					return;
				}
			jump_to_app(App_info.jump_to_app);
		}
	return;
}



void init_save_data(void)
{
	//reset_iap_buff();
	read_app_info();
	if(App_info.data_effect != 0X88)//0X88作为标志位,表示之前保存过数据,数据有效
		{	
			App_info.data_effect = 0X88;
			App_info.tem_jump= no_jump;
			App_info.jump_to_app = THIS_APP_ADDR;
			
			App_info.app_offset_table[0] = 0;
			App_info.app_offset_table[1] = 0;
			App_info.app_offset_table[2] = 0;
			App_info.app_offset_table[3] = 0;
			App_info.cur_app_offset= App_info.app_offset_table[App_info.app_point] ;
			write_app_info();
		}
	
}

void start_app(void)
{
	init_save_data();
	choose_app();
}


/*

void wifi_iap_cmd_operat(P_S_Wifi_Iap_Cmd cmd_parameter)
{
	upload_data_by_wifi(0XF1,(unsigned char *)cmd_parameter,6);
}

void wifi_iap_request_page(unsigned int page)
{
	unsigned char page_num[2] = {0};
	page_num[1] = page&0X000000FF;
	page_num[0] = (page>>8)&0X000000FF;
	upload_data_by_wifi(0XF0,page_num,2);
}

void wifi_iap_send_ready_code(void)
{
	S_Wifi_Iap_Cmd send_cmd_parameter = {0};
	send_cmd_parameter.cmd = WIFI_IAP_OPERAT_START_BACK;
	send_cmd_parameter.parameter_1 = THIS_APP >> 4;
	wifi_iap_cmd_operat(&send_cmd_parameter);
}

void wifi_iap_send_end_code(void)
{
	S_Wifi_Iap_Cmd send_cmd_parameter = {0};
	send_cmd_parameter.cmd = WIFI_IAP_OPERAT_END;
	send_cmd_parameter.parameter_1 = THIS_APP >> 4;
	wifi_iap_cmd_operat(&send_cmd_parameter);
}

void wifi_iap_operater(void)
{
	//unsigned char operat_type;
	
	if(data_from_wifi.data[9]==0XF1) //iap_cmd
		{
			switch(data_from_wifi.data[10])
				{
					case WIFI_IAP_OPERAT_START:
						wifi_iap_send_ready_code();
						break;
						
					case WIFI_IAP_OPERAT_BEGIN:
						wifi_iap_info.total_size = 0;
						wifi_iap_info.recved_page_sum = 0;
						wifi_iap_info.request_page = 0;
						wifi_iap_info.block = 0;
						wifi_iap_info.page = 0;
						
						wifi_iap_info.total_size |= data_from_wifi.data[11];
						wifi_iap_info.total_size <<= 8;
						wifi_iap_info.total_size |= data_from_wifi.data[12];
						
						wifi_iap_request_page(0);
						break;

					case WIFI_IAP_OPERAT_TRANSMISSION:
						break;

					case WIFI_IAP_OPERAT_END:
						break;
				}

		}

	if(data_from_wifi.data[9]==0XF0)//transfer
		{
			wifi_iap_write_to_flash();
		}

}


void wifi_iap_write_to_flash(void)
{
	vIAP.owner = OWNER_WIFI_IAP_BUFFER;

	wifi_iap_info.recved_page_sum++;
	wifi_iap_info.recved_page =0;
	wifi_iap_info.recved_page |= data_from_wifi.data[10];
	wifi_iap_info.recved_page <<= 8;
	wifi_iap_info.recved_page |= data_from_wifi.data[11];
	wifi_iap_info.request_page = wifi_iap_info.recved_page+1;
	wifi_iap_info.cur_page_size = data_from_wifi.point - 13;

	wifi_iap_info.page = wifi_iap_info.recved_page % 16;
	wifi_iap_info.block = wifi_iap_info.recved_page /16;

	wifi_iap_copy_to_flash();
	
	if(wifi_iap_info.recved_page == wifi_iap_info.total_size- 1)
		{
			if(wifi_iap_info.page)
				{
					//wifi_iap_info.block ++;
					memcopy((char*)&data_from_wifi.data[12],(char*)&vIAP.Page_buffer[wifi_iap_info.page *128],(wifi_iap_info.cur_page_size));
					write_flash(WRITE_TO_ADDR+(wifi_iap_info.block * FLASH_PAGE_SIZE),(char*)vIAP.Page_buffer,FLASH_PAGE_SIZE);
					reset_iap_buff();
				}
			
			vIAP.owner = OWNER_ON_ONE;
			wifi_iap_send_end_code();
		}
	else
		{
			wifi_iap_request_page(wifi_iap_info.request_page);
		}
}

void wifi_iap_copy_to_flash(void)
{	
	if(wifi_iap_info.recved_page < wifi_iap_info.total_size -1)
		{
			if(wifi_iap_info.cur_page_size < 128)
				{
					wifi_iap_info.request_page = wifi_iap_info.recved_page;
					return;
				}
		}
	memcopy((char*)&data_from_wifi.data[12],(char*)&vIAP.Page_buffer[wifi_iap_info.page *128],(wifi_iap_info.cur_page_size));
	if(wifi_iap_info.page == 15)
		{
			write_flash(WRITE_TO_ADDR+(wifi_iap_info.block * FLASH_PAGE_SIZE),(char*)vIAP.Page_buffer,FLASH_PAGE_SIZE);			
			reset_iap_buff();	
		}
	else
		{
			//memcopy((char*)&data_from_wifi.data[12],(char*)&vIAP.Page_buffer[wifi_iap_info.page *128],(wifi_iap_info.cur_page_size));
		}
	
}
*/

