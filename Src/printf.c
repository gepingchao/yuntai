
#include "include.h"

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&COM, (unsigned char *)&ch, 1, 0xFFFF);

  return ch;
}


void printf_by_web(const char *fmt, ...)
{
	/*char buf[200];
	char* pArg = NULL;
	char c;
	pArg = (char*) &fmt;          //注意不要写成p = fmt !!因为这里要对//参数取址，而不是取值
	pArg += sizeof(fmt);         //等价于原来的va_start      
	do
	{
			c =*fmt;
		if (c != '%')
			{
				//send(1,(unsigned char*)c,1);            //照原样输出字符
			}
		else

			{
			//按格式字符输出数据
			switch(*++fmt)
				{
					case 'd':
					sprintf(buf,"%d",*((int*)pArg));    
					send(1,(unsigned char*)buf,string_length(buf));
					break;

					case 'x':
					sprintf(buf,"%#x",*((int*)pArg));
					send(1,(unsigned char*)buf,string_length(buf));
					break;

					default:
					break;
				}
			pArg += sizeof(int);              //等价于原来的va_arg
			}
		++fmt;
		}
	while (*fmt != '/0');
	pArg = NULL;                               //等价于va_end*/
return;	
}



