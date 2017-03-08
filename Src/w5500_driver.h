/*
* File Name          : w5500_driver.h
* Description       : 头文件
*                       
*
*
*
*
*
*/

#ifndef __W5500_DRIVER_H__
#define __W5500_DRIVER_H__

#ifdef __cplusplus
 extern "C" {
#endif
////////////////////////////////////
#include "include.h"

#define SOCKET_BUFF_SIZE 	256

typedef struct
{
	osThreadId server_handle_0;
	osThreadId server_handle_1;
	osThreadId server_handle_2;
	osThreadId server_handle_3;
	osThreadId server_handle_4;
	osThreadId server_handle_5;
	osThreadId server_handle_6;
	osThreadId server_handle_7;
	unsigned short socket_recv_num[8];
	unsigned char socket_is_runing;
}S_Tcp_Server,*P_S_Tcp_Server;

typedef struct 
{
	unsigned char server_ip[4];
	unsigned int server_port;
}S_Server_Info,*p_S_Server_Info;

typedef struct
{
	unsigned char this_client_socket_num;
	S_Server_Info target_server;
}S_Client_Info,*p_S_Client_Info;



extern wiz_NetInfo gWIZNETINFO;
extern netmode_type g_net_mode;
extern unsigned char G_socket_buf[8][SOCKET_BUFF_SIZE];


 void SPI_CrisEnter(void);
void SPI_CrisExit(void);
void SPI_WriteByte(unsigned char TxData);
unsigned char SPI_ReadByte(void);
void SPI_CS_Select(void);
void SPI_CS_Deselect(void);
void network_init(void);
void socket_init(void);




void InitW5500(void);//初始化w5500

void tcp_client(p_S_Client_Info client);//开启一个tcp client

void tcp_server(unsigned int socket_num,unsigned int port); //开启一个tcp server


void close_socket_server(unsigned int socket_num); //关闭一个socket  (tcp&udp)

void udp_server(unsigned int socket_num,unsigned int port);//开启一个udp连接


void deal_all_socket_server(void);
void deal_each_socket_server(unsigned char socket_num);
void deal_all_socket_client(void);
void deal_each_socket_client(unsigned char socket_num);
void deal_all_socket_udp(void);
void deal_each_socket_udp(unsigned char socket_num);


////////////////////////////////////

#ifdef __cplusplus
}
#endif
#endif

