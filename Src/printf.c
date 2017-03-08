
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
	pArg = (char*) &fmt;          //ע�ⲻҪд��p = fmt !!��Ϊ����Ҫ��//����ȡַ��������ȡֵ
	pArg += sizeof(fmt);         //�ȼ���ԭ����va_start      
	do
	{
			c =*fmt;
		if (c != '%')
			{
				//send(1,(unsigned char*)c,1);            //��ԭ������ַ�
			}
		else

			{
			//����ʽ�ַ��������
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
			pArg += sizeof(int);              //�ȼ���ԭ����va_arg
			}
		++fmt;
		}
	while (*fmt != '/0');
	pArg = NULL;                               //�ȼ���va_end*/
return;	
}



