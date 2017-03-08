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


//tcp server

typedef struct
{
	osThreadId tcp_server_handle_0;
	osThreadId tcp_server_handle_1;
	osThreadId tcp_server_handle_2;
	osThreadId tcp_server_handle_3;
	osThreadId tcp_server_handle_4;
	osThreadId tcp_server_handle_5;
	osThreadId tcp_server_handle_6;
	osThreadId tcp_server_handle_7;
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


void SPI_CrisEnter(void);
void SPI_CrisExit(void);
void SPI_WriteByte(unsigned char TxData);
unsigned char SPI_ReadByte(void);
void SPI_CS_Select(void);
void SPI_CS_Deselect(void);
void network_init(void);

//////////////////////////////////////////////////////////////////////以下是开发调用的函数
void network_reinit(void);
void InitW5500(void);

void tcp_client(p_S_Client_Info client);//启动客户端进程

void tcp_server(unsigned int socket_num,unsigned int port);//启动服务器进程
void close_socket(unsigned int socket_num);//关闭服务器/客户端 进程

unsigned char start_udp_server(unsigned int socket_num,unsigned int port);//未完成

void deal_all_socket_server(void); //服务器收到信息后执行的函数
void deal_each_socket_server(unsigned char socket_num);//服务器x 收到信息后执行的函数

void deal_all_socket_client(void);//客户端收到信息收到信息后执行的函数
void deal_each_socket_client(unsigned char socket_num);//客户端X 收到信息后执行的函数
////////////////////////////////////

#ifdef __cplusplus
}
#endif
#endif

