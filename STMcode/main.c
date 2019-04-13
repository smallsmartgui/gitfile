#include "Gpio.h"
#include "delay.h"
#include "USART.h"
#include "24c02.h"
#include "ili9320.h"
#include "SPI_Flash.h" 
const char menu[] =
   "\r\n                                                        \n\r"
   "\r\n+******************************************************+\n\r"
   "\r\n|                                                      |\n\r"
   "\r\n|    �������ʵ���˶�24C02оƬ�Ķ�д���ԡ�24C02�� 256  |\n\r"
   "\r\n|   �ֽڵĴ洢�ռ䣬ҳ��СΪ8�ֽڣ����԰��ֽڻ�ҳ����  |\n\r"
   "\r\n|   ��д����������Keil MDK 5.10  ST��v3.50�²���ͨ���� |\n\r"
   "\r\n|                                                      |\n\r"
   "\r\n|          ʹ�þ�����                                  |\n\r"
   "\r\n|                   ����S2,��0x50��ַ��д������        |\n\r"
   "\r\n|                   ����S3,��0x50��ַ����������        |\n\r"
   "\r\n|                                                      |\n\r"
   "\r\n|                                                      |\n\r"
   "\r\n|                                                      |\n\r"                                                        
   "\r\n+------------------------------------------------------+\n\r";

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main(void)
{   
	u32 id;
	ili9320_Initializtion();
	ili9320_Clear(Black);
	CheckBitmapFilesStatus();
  LED_Init();
  USART1_Init();
	USART2_Init();
  Delay_Init();                                            //systick��ʼ��
  printf ( menu );                                         //��ʾ�˵�    
  printf("\r\n    \r\n");	
	SPI_FLASH_Init();
	id=SPI_FLASH_ReadID();
  printf("\r\n 0x%x   \r\n",id);
	while(1)
	{		
	
		
  }
}
