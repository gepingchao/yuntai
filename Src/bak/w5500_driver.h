/*
* File Name          : w5500_driver.h
* Description       : ͷ�ļ�
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

//////////////////////////////////////////////////////////////////////�����ǿ������õĺ���
void network_reinit(void);
void InitW5500(void);

void tcp_client(p_S_Client_Info client);//�����ͻ��˽���

void tcp_server(unsigned int socket_num,unsigned int port);//��������������
void close_socket(unsigned int socket_num);//�رշ�����/�ͻ��� ����

unsigned char start_udp_server(unsigned int socket_num,unsigned int port);//δ���

void deal_all_socket_server(void); //�������յ���Ϣ��ִ�еĺ���
void deal_each_socket_server(unsigned char socket_num);//������x �յ���Ϣ��ִ�еĺ���

void deal_all_socket_client(void);//�ͻ����յ���Ϣ�յ���Ϣ��ִ�еĺ���
void deal_each_socket_client(unsigned char socket_num);//�ͻ���X �յ���Ϣ��ִ�еĺ���
////////////////////////////////////

#ifdef __cplusplus
}
#endif
#endif

