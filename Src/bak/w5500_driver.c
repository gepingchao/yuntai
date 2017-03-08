#include "include.h"


#define RECV_DATA_BUF_SIZE	512
#define PC4_1 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET)
#define PC5_1 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET)
#define PC4_0 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET)
#define PC5_0 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET)

wiz_NetInfo gWIZNETINFO;
netmode_type g_net_mode;
unsigned char G_tcp_buf[8][RECV_DATA_BUF_SIZE];

S_Tcp_Server Socket_Server;


extern S_SYS_Task_Info SYS_Task_Info;

void InitW5500(void)
{
	PC4_1;
	PC5_1;
	osDelay(20);
	reg_wizchip_cris_cbfunc(SPI_CrisEnter, SPI_CrisExit);  
	reg_wizchip_cs_cbfunc(SPI_CS_Select, SPI_CS_Deselect);
	reg_wizchip_spi_cbfunc(SPI_ReadByte, SPI_WriteByte); 
	//test_spi1();
	network_init();
}

 void SPI_CrisEnter(void)
{
    __set_PRIMASK(1);
}
/**
  * @brief  退出临界区
  * @retval None
  */
void SPI_CrisExit(void)
{
    __set_PRIMASK(0);
}

void SPI_WriteByte(unsigned char TxData)
{      
	unsigned char sendbuf;
	sendbuf = TxData;
	HAL_StatusTypeDef res;	
	res = HAL_SPI_Transmit(&hspi1,&sendbuf,1,0xff);
	switch(res)
		{
			case HAL_OK :
				//printf("send ok \r\n");
				break;
			case HAL_ERROR :
				printf("send HAL_ERROR\r\n");
				break;
			case HAL_BUSY :
				printf("send HAL_BUSY\r\n");
				break;
			case HAL_TIMEOUT :
				printf("send HAL_TIMEOUT\r\n");
				break;
		}
	/* while((SPI1->SR&SPI_SR_TXE)==0);   //等待发送区空          
	SPI1->DR=TxData;                                         //发送一个byte 
	while((SPI1->SR&SPI_SR_RXNE)==0); //等待接收完一个byte  
	SPI1->DR;  */
	//SPI_SR_TXE
	//SPI_SR_RXNE

}

unsigned char SPI_ReadByte(void)
{
	unsigned char recvbuf;
	HAL_StatusTypeDef res;	
	res =HAL_SPI_Receive(&hspi1,&recvbuf,1,0xff);
	switch(res)
		{
			case HAL_OK :
				//printf("recv ok \r\n");
				break;
			case HAL_ERROR :
				printf("recv HAL_ERROR\r\n");
				break;
			case HAL_BUSY :
				printf("recv HAL_BUSY\r\n");
				break;
			case HAL_TIMEOUT :
				printf("recv HAL_TIMEOUT\r\n");
				break;
		}
	return recvbuf;
    /*while((SPI1->SR&SPI_SR_TXE)==0);   //等待发送区空              
    SPI1->DR=0xFF;                                               //发送一个空数据产生输入数据的时钟 
    while((SPI1->SR&SPI_SR_RXNE)==0); //等待接收完一个byte  
    return SPI1->DR;    */
    
}

void SPI_CS_Select(void)
{
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin,GPIO_PIN_RESET);
	//printf("Select\r\n");
}

void SPI_CS_Deselect(void)
{
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin,GPIO_PIN_SET);
	//printf("Deselect\r\n");
}
void network_init(void)
{
	gWIZNETINFO.mac[0] = 1;
	static unsigned char tmpstr[6];
	gWIZNETINFO.mac[0] = 0x12;
	gWIZNETINFO.mac[1] = 0xa2;
	gWIZNETINFO.mac[2] = 0x11;
	gWIZNETINFO.mac[3] = 0x22;
	gWIZNETINFO.mac[4] = 0x33;
	gWIZNETINFO.mac[5] = 0x44;
	gWIZNETINFO.ip[0] = 192;
	gWIZNETINFO.ip[1] = 168;
	gWIZNETINFO.ip[2] = 1;
	gWIZNETINFO.ip[3] = 251;
	gWIZNETINFO.sn[0] = 255;
	gWIZNETINFO.sn[1] = 255;
	gWIZNETINFO.sn[2] = 255;
	gWIZNETINFO.sn[3] = 0;
	gWIZNETINFO.gw[0] = 192;
	gWIZNETINFO.gw[1] = 168;
	gWIZNETINFO.gw[2] = 1;
	gWIZNETINFO.gw[3] = 1;
	gWIZNETINFO.dns[0] = 8;
	gWIZNETINFO.dns[1] = 8;
	gWIZNETINFO.dns[2] = 8;
	gWIZNETINFO.dns[3] = 8;
	//gWIZNETINFO.dhcp = 2;
	ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);
	ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);

	ctlnetwork(CN_GET_NETMODE, (void*)&g_net_mode);
	
    // Display Network Information
    ctlwizchip(CW_GET_ID,(void*)tmpstr);
    printf("\r\n=== %s NET CONF ===\r\n",(char*)tmpstr);
    printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",gWIZNETINFO.mac[0],gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],
          gWIZNETINFO.mac[3],gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
    printf("SIP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
    printf("GAR: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0],gWIZNETINFO.gw[1],gWIZNETINFO.gw[2],gWIZNETINFO.gw[3]);
    printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);
    printf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0],gWIZNETINFO.dns[1],gWIZNETINFO.dns[2],gWIZNETINFO.dns[3]);
    printf("======================\r\n");
}

void network_reinit(void)
{
	PC4_0;
	PC5_0;
	osDelay(1000);
	PC4_1;
	PC5_1;
	osDelay(100);
	unsigned char tmpstr[6] = {0};	
	ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);
	ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);
	ctlnetwork(CN_GET_NETMODE, (void*)&g_net_mode);
	
    // Display Network Information
    ctlwizchip(CW_GET_ID,(void*)tmpstr);
    printf("\r\n=== %s NET CONF ===\r\n",(char*)tmpstr);
    printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",gWIZNETINFO.mac[0],gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],
          gWIZNETINFO.mac[3],gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
    printf("SIP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
    printf("GAR: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0],gWIZNETINFO.gw[1],gWIZNETINFO.gw[2],gWIZNETINFO.gw[3]);
    printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);
    printf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0],gWIZNETINFO.dns[1],gWIZNETINFO.dns[2],gWIZNETINFO.dns[3]);
    printf("======================\r\n");
}


unsigned char start_tcp_server(unsigned int socket_num,unsigned int port)
{
	volatile unsigned char socket_status;
	volatile unsigned int num;
	volatile unsigned int server_port;
	num = socket_num;
	server_port = port;
	save_task_info();
	while(1)
		{
			osDelay(50);
			socket_status = getSn_SR(num);

			switch(socket_status)
				{
					case SOCK_INIT:
						listen(num);//初始化完成就开始监听
						
						break;
					case SOCK_ESTABLISHED://建立连接
						if(getSn_SR(num)&Sn_IR_CON)
							{
								setSn_IR(num,Sn_IR_CON);
							}
						Socket_Server.socket_recv_num[num]= recv(num,G_tcp_buf[num],RECV_DATA_BUF_SIZE) ;
						if(Socket_Server.socket_recv_num[num] > 0)
							{
								send(num,G_tcp_buf[num],Socket_Server.socket_recv_num[num]) ;//收到数据
								deal_all_socket_server();
								deal_each_socket_server(num);
								Socket_Server.socket_recv_num[num] = 0;
							}
						break;
					case SOCK_CLOSE_WAIT://socket等待关闭
						if(disconnect(num) != SOCK_OK)
							{
								break;
							}
						printf("socket %d closed!\r\n",socket_num);
					case SOCK_CLOSED://socket关闭
						if(socket(num,Sn_MR_TCP,port,Sn_MR_ND) != num)
							{
								printf("socket error\r\n");
								while(1);
							}
				}
		}
}

void start_tcp_server_thread(void const* pvParameters)
{
	unsigned char socket_num;
	unsigned int port;
	port =  (unsigned int)pvParameters & 0X00FFFFFF;
	socket_num = ((unsigned int)pvParameters >> 24) & 0X000000FF;
	start_tcp_server(socket_num,port);
}

void tcp_server(unsigned int socket_num,unsigned int port)
{
	close(socket_num);
	unsigned char loopx;
	unsigned int thread_parameter;
	thread_parameter = 0;
	thread_parameter = socket_num;
	thread_parameter <<= 24;
	port &= 0X00FFFFFF;
	thread_parameter |= port;
	if(socket_num == 0)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.tcp_server_handle_0)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(socket_0, start_tcp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.tcp_server_handle_0 = osThreadCreate(osThread(socket_0), (void*)thread_parameter);
		}
	if(socket_num == 1)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.tcp_server_handle_1)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(socket_1, start_tcp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.tcp_server_handle_1 = osThreadCreate(osThread(socket_1), (void*)thread_parameter);

		}
	if(socket_num == 2)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.tcp_server_handle_2)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(socket_2, start_tcp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.tcp_server_handle_2 = osThreadCreate(osThread(socket_2), (void*)thread_parameter);
		}
	if(socket_num == 3)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.tcp_server_handle_3)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(socket_3, start_tcp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.tcp_server_handle_3 = osThreadCreate(osThread(socket_3), (void*)thread_parameter);
		}
	if(socket_num == 4)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.tcp_server_handle_4)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(socket_4, start_tcp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.tcp_server_handle_4 = osThreadCreate(osThread(socket_4), (void*)thread_parameter);
		}
	if(socket_num == 5)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.tcp_server_handle_5)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(socket_5, start_tcp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.tcp_server_handle_5 = osThreadCreate(osThread(socket_5), (void*)thread_parameter);
		}
	if(socket_num == 6)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.tcp_server_handle_6)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(socket_6, start_tcp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.tcp_server_handle_6 = osThreadCreate(osThread(socket_6), (void*)thread_parameter);
		}
	if(socket_num == 7)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.tcp_server_handle_7)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(socket_7, start_tcp_server_thread, osPriorityNormal, 0, 128);
			Socket_Server.tcp_server_handle_7 = osThreadCreate(osThread(socket_7), (void*)thread_parameter);
		}
}

void close_socket(unsigned int socket_num)
{
	switch(socket_num)
		{
			case 0:
				vTaskDelete(Socket_Server.tcp_server_handle_0);
				delete_task_info(Socket_Server.tcp_server_handle_0);
				break;
			case 1:
				vTaskDelete(Socket_Server.tcp_server_handle_1);
				delete_task_info(Socket_Server.tcp_server_handle_1);
				break;
			case 2:
				vTaskDelete(Socket_Server.tcp_server_handle_2);
				delete_task_info(Socket_Server.tcp_server_handle_2);
				break;
			case 3:
				vTaskDelete(Socket_Server.tcp_server_handle_3);
				delete_task_info(Socket_Server.tcp_server_handle_3);
				break;
			case 4:
				vTaskDelete(Socket_Server.tcp_server_handle_4);
				delete_task_info(Socket_Server.tcp_server_handle_4);
				break;
			case 5:
				vTaskDelete(Socket_Server.tcp_server_handle_5);
				delete_task_info(Socket_Server.tcp_server_handle_5);
				break;
			case 6:
				vTaskDelete(Socket_Server.tcp_server_handle_6);
				delete_task_info(Socket_Server.tcp_server_handle_6);
				break;
			case 7:
				vTaskDelete(Socket_Server.tcp_server_handle_7);
				delete_task_info(Socket_Server.tcp_server_handle_7);
				break;
			default:
				break;
		}		
}




unsigned char start_tcp_client(p_S_Client_Info client)
{
	volatile unsigned char socket_status =0 ;
	volatile unsigned int num;
	num = client->this_client_socket_num;
	
	save_task_info();
	
	close(num);	
	//unsigned char server_ip[4] = {192,168,1,214};	
	//unsigned int server_port;
	while(1)
		{
			osDelay(50);
			socket_status = getSn_SR(num);

			switch(socket_status)
				{
					case SOCK_INIT:
						//listen(num);//初始化完成就开始尝试连接
						connect(num,client->target_server.server_ip,client->target_server.server_port);						
						break;
					case SOCK_ESTABLISHED://建立连接
						if(getSn_SR(num)&Sn_IR_CON)
							{
								setSn_IR(num,Sn_IR_CON);
							}
						Socket_Server.socket_recv_num[num] = recv(num,G_tcp_buf[num],RECV_DATA_BUF_SIZE) ;
						if(Socket_Server.socket_recv_num[num] > 0)
							{
								//send(num,G_tcp_buf[num],Socket_Server.socket_recv_num[num]) ;//收到数据
								deal_all_socket_client();
								deal_each_socket_client(num);
								Socket_Server.socket_recv_num[num] = 0;
							}
						break;
					case SOCK_CLOSE_WAIT://socket等待关闭
						if(disconnect(num) != SOCK_OK)
							{
								break;
							}
						printf("socket %d closed!\r\n",num);
					case SOCK_CLOSED://socket关闭
						if(socket(num,Sn_MR_TCP,8888+num,Sn_MR_ND) != num)
							{
								printf("socket error\r\n");
								while(1);
							}
				}
		}
}


void start_tcp_client_thread(void const* pvParameters)
{
	p_S_Client_Info client;
	client = (p_S_Client_Info) pvParameters;
	start_tcp_client(client);
}

void tcp_client(p_S_Client_Info client)//调用这个函数会产生一个任务为相应的sock作为client服务
{
	unsigned char loopx;
	void* thread_parameter;
	thread_parameter = (void*) client;
	if(client->this_client_socket_num == 0)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.tcp_server_handle_0)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(socket_0, start_tcp_client_thread, osPriorityNormal, 0, 128);
			Socket_Server.tcp_server_handle_0 = osThreadCreate(osThread(socket_0), thread_parameter);
		}
	if(client->this_client_socket_num == 1)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.tcp_server_handle_1)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(socket_1, start_tcp_client_thread, osPriorityNormal, 0, 128);
			Socket_Server.tcp_server_handle_1 = osThreadCreate(osThread(socket_1), thread_parameter);
		}
	if(client->this_client_socket_num == 2)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.tcp_server_handle_2)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(socket_2, start_tcp_client_thread, osPriorityNormal, 0, 128);
			Socket_Server.tcp_server_handle_2 = osThreadCreate(osThread(socket_2), thread_parameter);
		}
	if(client->this_client_socket_num == 3)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.tcp_server_handle_3)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(socket_3, start_tcp_client_thread, osPriorityNormal, 0, 128);
			Socket_Server.tcp_server_handle_3 = osThreadCreate(osThread(socket_3), thread_parameter);
		}
	if(client->this_client_socket_num == 4)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.tcp_server_handle_4)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(socket_4, start_tcp_client_thread, osPriorityNormal, 0, 128);
			Socket_Server.tcp_server_handle_4 = osThreadCreate(osThread(socket_4), thread_parameter);
		}
	if(client->this_client_socket_num == 5)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.tcp_server_handle_5)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(socket_5, start_tcp_client_thread, osPriorityNormal, 0, 128);
			Socket_Server.tcp_server_handle_5 = osThreadCreate(osThread(socket_5), thread_parameter);
		}
	if(client->this_client_socket_num == 6)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.tcp_server_handle_6)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(socket_6, start_tcp_client_thread, osPriorityNormal, 0, 128);
			Socket_Server.tcp_server_handle_6 = osThreadCreate(osThread(socket_6), thread_parameter);
		}

	if(client->this_client_socket_num == 7)
		{
			for(loopx = 0;loopx < SYS_Task_Info.save_number; loopx++ )
				{
					if((SYS_Task_Info.task[loopx].task_handle == Socket_Server.tcp_server_handle_7)&&(SYS_Task_Info.task[loopx].task_handle))
						{	
							return;
						}
				}
			osThreadDef(socket_7, start_tcp_client_thread, osPriorityNormal, 0, 128);
			Socket_Server.tcp_server_handle_7 = osThreadCreate(osThread(socket_7), thread_parameter);
		}

}


unsigned char start_udp_server(unsigned int socket_num,unsigned int port)
{
	volatile unsigned char socket_status;
	volatile unsigned int num;
	volatile unsigned int server_port;
	num = socket_num;
	server_port = port;
	save_task_info();
	while(1)
		{
			osDelay(50);
			socket_status = getSn_SR(num);

			switch(socket_status)
				{
					case SOCK_UDP:
						
						break;
					case SOCK_CLOSED://socket关闭
						if(socket(num,Sn_MR_UDP,port,0) != num)
							{
								printf("socket error\r\n");
								while(1);
							}
				}
		}
}


void start_cfg_server(void)
{
	do_http();
}


void deal_all_socket_server(void)
{
	
}

void deal_each_socket_server(unsigned char socket_num)
{
	switch (socket_num)
		{
			case 0:
				break;

			case 1:
				break;

			case 2:
				break;

			case 3:
				break;

			case 4:
				break;

			case 5:
				break;

			case 6:
				break;

			case 7:
				break;

				
		}
}

void deal_all_socket_client(void)
{
	//deal_protocol();
}


void deal_each_socket_client(unsigned char socket_num)
{
	deal_protocol(G_tcp_buf[socket_num]);
	switch (socket_num)
		{
			case 0:
				break;

			case 1:
				break;

			case 2:
				break;

			case 3:
				break;

			case 4:
				break;

			case 5:
				break;

			case 6:
				break;

			case 7:
				break;

				
		}
}



