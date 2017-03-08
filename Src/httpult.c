#include "httpd.h"
#include "httpult.h"
//#include "device.h"
#include "webpge.h"
#define SOCK_HTTP 7
#define DEBUG_HTTP

CONFIG_MSG  ConfigMsg;
extern char tx_buf[MAX_URI_SIZE];
extern char rx_buf[MAX_URI_SIZE];


uint8 boundary[64];
uint8 tmp_buf[1460]={0xff,};
uint8 pub_buf[1024];

static void make_basic_config_setting_json_callback(int8* buf, CONFIG_MSG config_msg)
{
  sprintf(buf,"settingsCallback({\"ver\":\"%d.%d\",\
                \"mac\":\"%02X:%02X:%02X:%02X:%02X:%02X\",\
                \"ip\":\"%d.%d.%d.%d\",\
                \"gw\":\"%d.%d.%d.%d\",\
                \"sub\":\"%d.%d.%d.%d\",\
								\"dns\":\"%d.%d.%d.%d\",\
                });",config_msg.sw_ver[0],config_msg.sw_ver[1],
                config_msg.mac[0],config_msg.mac[1],config_msg.mac[2],config_msg.mac[3],config_msg.mac[4],config_msg.mac[5],
                config_msg.lip[0],config_msg.lip[1],config_msg.lip[2],config_msg.lip[3],
                config_msg.gw[0],config_msg.gw[1],config_msg.gw[2],config_msg.gw[3],
                config_msg.sub[0],config_msg.sub[1],config_msg.sub[2],config_msg.sub[3],
								config_msg.dns[0],config_msg.dns[1],config_msg.dns[2],config_msg.dns[3]
                );
}

void do_http(void)
{
  uint8 ch=SOCK_HTTP;
  uint16 len;

  st_http_request *http_request;
  memset(rx_buf,0x00,MAX_URI_SIZE);
  http_request = (st_http_request*)rx_buf;		// struct of http request  
  /* http service start */
  switch(getSn_SR(ch))
  {
    case SOCK_INIT:
      listen(ch);
      break;
    case SOCK_LISTEN:
      break;
    case SOCK_ESTABLISHED:
    //case SOCK_CLOSE_WAIT:
      if(getSn_IR(ch) & Sn_IR_CON)
      {
        setSn_IR(ch, Sn_IR_CON);
      }
      if ((len = getSn_RX_RSR(ch)) > 0)		
      {
        len = recv(ch, (uint8*)http_request, len); 
			//	printf("rx:%s\r\n",(uint8*)http_request);
        *(((uint8*)http_request)+len) = 0;
        proc_http(ch, (uint8*)http_request); // request is processed
        disconnect(ch);
      }
      break;
    case SOCK_CLOSE_WAIT:   
      if ((len = getSn_RX_RSR(ch)) > 0)
      {
        //printf("close wait: %d\r\n",len);
        len = recv(ch, (uint8*)http_request, len);       
        *(((uint8*)http_request)+len) = 0;
        proc_http(ch, (uint8*)http_request); // request is processed
      }
      disconnect(ch);
      break;
    case SOCK_CLOSED:                   
      socket(ch, Sn_MR_TCP, 80, 0x00);    /* reinitialize the socket */
      break;
    default:
    break;
  }// end of switch
}

void proc_http(SOCKET s, uint8 * buf)
{

  int8* name; //get method request file name
  int8 req_name[32]={0x00,}; //post method request file name
  unsigned long file_len=0;
  uint16 send_len=0;
  uint8* http_response;
  st_http_request *http_request;
  memset(tx_buf,0x00,MAX_URI_SIZE);
  http_response = (uint8*)rx_buf;
  http_request = (st_http_request*)tx_buf;
  parse_http_request(http_request, buf);    // After analyze request, convert into http_request
  //method Analyze
  switch (http_request->METHOD)		
  {
    case METHOD_ERR :
      memcpy(http_response, ERROR_REQUEST_PAGE, sizeof(ERROR_REQUEST_PAGE));
      send(s, (uint8 *)http_response, strlen((int8 const*)http_response));
    break;
    case METHOD_HEAD:
    case METHOD_GET:
      //get file name from uri
      name = http_request->URI;

      if(strcmp(name,"/index.htm")==0 || strcmp(name,"/")==0 || (strcmp(name,"/index.html")==0))
      {
        file_len = strlen(INDEX_HTML);
        make_http_response_head((uint8*)http_response, PTYPE_HTML,file_len);
        send(s,http_response,strlen((char const*)http_response));
        send_len=0;
        while(file_len)
        {
          if(file_len>1024)
          {
            /*if(getSn_SR(s)!=SOCK_ESTABLISHED)
            {
              return;
            }*/
            send(s, (uint8 *)INDEX_HTML+send_len, 1024);
            send_len+=1024;
            file_len-=1024;
            osDelay(5);
          }
          else
          {
            send(s, (uint8 *)INDEX_HTML+send_len, file_len);
            send_len+=file_len;
            file_len-=file_len;
          } 
       }
     }
     else if(strcmp(name,"/w5500.js")==0)
     {
        memset(tx_buf,0,MAX_URI_SIZE);
        make_basic_config_setting_json_callback(tx_buf,ConfigMsg);
        sprintf((char *)http_response,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s",strlen(tx_buf),tx_buf);
        send(s, (unsigned char *)http_response, strlen((char const*)http_response));
     }
      break;
      /*POST method*/
    case METHOD_POST:

      //get file name from uri
      mid(http_request->URI, "/", " ", req_name);

      if(strcmp(req_name,"config.cgi")==0)
      {
        cgi_ipconfig((copy_st_http_request*)http_request);
        make_cgi_response(5,(int8*)ConfigMsg.lip,tx_buf);         
        sprintf((char *)http_response,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s",strlen(tx_buf),tx_buf);
        send(s, (unsigned char *)http_response, strlen((char *)http_response));	
        disconnect(s);

        return;
      }
      break;
     default :
      break;
  }
}

void cgi_ipconfig(copy_st_http_request *http_request)
{  
  uint8 * param;
  //Device setting

  param = get_http_param_value(http_request->URI,"ip");
  if(param)
  {
    inet_addr_((uint8*)param, ConfigMsg.lip);	
  }

  param = get_http_param_value(http_request->URI,"gw");
  if(param)
  {
    inet_addr_((uint8*)param, ConfigMsg.gw);	
  }
  
  param = get_http_param_value(http_request->URI,"sub");
  if(param)
  {
    inet_addr_((uint8*)param, ConfigMsg.sub);		
  }
  
	param = get_http_param_value(http_request->URI,"dns");//得到修给后的子网掩码SUB
  if(param)
  {
    inet_addr_((uint8*)param, ConfigMsg.dns);		
  }
  /* Program the network parameters received into eeprom */
  //write_config_to_eeprom();
}


void make_cgi_response(uint16 delay, int8* url,int8* cgi_response_buf)
{
  sprintf(cgi_response_buf,"<html><head><title>iWeb - Configuration</title><script language=javascript>j=%d;function func(){document.getElementById('delay').innerText=' '+j + ' ';j--;setTimeout('func()',1000);if(j==0)location.href='http://%d.%d.%d.%d/';}</script></head><body onload='func()'>please wait for a while, the module will boot in<span style='color:red;' id='delay'></span> seconds.</body></html>",delay,url[0],url[1],url[2],url[3]);
  return;
}


