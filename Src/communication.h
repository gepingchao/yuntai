#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__
#include "include.h"

#define SERIAL_RECV_BUFF_SIZE 256
#define SERIAL_SEND_BUFF_SIZE 256

typedef struct
{
	unsigned short data_len;
	unsigned short dma_cndtr;
	unsigned char is_dma_tx_sending;
	unsigned char rx_data[SERIAL_RECV_BUFF_SIZE];
	unsigned char tx_data[SERIAL_SEND_BUFF_SIZE];	
	unsigned char recv_ok;	
	unsigned short point;
	unsigned char enable_rxdata;
}S_Serial_Recv_Data,*p_S_Serial_Recv_Data;
extern S_Serial_Recv_Data data_from_serial;




#endif

