#include "include.h"

S_Serial_Recv_Data data_from_serial;

void set_ok_msg(char* msg)
{
	
}

void send_msg(char* msg,E_Communiction_Msg msg_type)
{
	*( msg + 0 ) = 0XAA;
	*( msg + 1 ) = 0X90;
	*( msg + 2 ) = machine_info.this_machime_adress;
	*( msg + 3 ) = 0X07;
	*( msg + 4 ) = 0XDD;
	*( msg + 5 ) = msg_type;
	SetCRC8Sub((unsigned char*)msg, 6);
	send(machine_info.current_ctrl_socket_num,(unsigned char*)msg,7);
}



