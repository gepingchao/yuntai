#ifndef	__HTTPULT_H__
#define	__HTTPULT_H__
#include "include.h"
//#include "stm32f10x.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "stm32f10x.h"
//#include "w5500.h"
//#include "config.h"
//#include "ult.h"
//#include "spi2.h"
//#include "device.h"
#include "httpd.h"
//#include "socket.h"
//#include "sockult.h"



typedef struct _CONFIG_MSG
{
  uint8 mac[6];
  uint8 sw_ver[2];
  uint8 lip[4];
  uint8 sub[4];
  uint8 gw[4];
  uint8 dns[4];	
  uint8 dhcp;
  uint8 debug;
  uint16 fw_len;
  uint8 state;  
	uint8 crc_flag[6];
}CONFIG_MSG;
extern CONFIG_MSG  ConfigMsg;
#define MAX_URI_SIZE	1460
typedef struct 
{
  uint8	METHOD;						/**< request method(METHOD_GET...). */
  uint8	TYPE;						/**< request type(PTYPE_HTML...).   */
  int8	URI[MAX_URI_SIZE];				/**< request file name.             */
}copy_st_http_request;

void proc_http(SOCKET s, unsigned char * buf);
void do_http(void);
void cgi_ipconfig(copy_st_http_request *http_request);
//void trimp(uint8* src, uint8* dst, uint16 len);
uint16 make_msg_response(uint8* buf,int8* msg);

void make_cgi_response(uint16 a,int8* b,int8* c);
void make_pwd_response(int8 isRight,uint16 delay,int8* cgi_response_content, int8 isTimeout);
#endif


