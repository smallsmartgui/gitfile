#include "Gpio.h"
#include "delay.h"
#include "USART.h"
#include "24c02.h"
#include "ili9320.h"
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

 uint8_t I2C_Write[]={"www.openmcu.com,I2C Test!"};
#define  Lenth  sizeof(I2C_Write)
uint8_t I2C_Read[Lenth];
/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main(void)
{   
	ili9320_Initializtion();
	ili9320_Clear(Black);
	CheckBitmapFilesStatus();
  LED_Init();
  USART1_Init();
	USART2_Init();
  Delay_Init();                                            //systick��ʼ��
	Init_IIC();					                                     //24LC02��ʼ�� 
  printf ( menu );                                         //��ʾ�˵�    
  printf("\r\n    \r\n");	
	while(1)
	{		
		if(!(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)))                   //S3����
			 {
				 Delay_ms(10);
				  LED2(1);
				  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))           //�ȴ������ɿ�
					{
						 LED2(0);
						 printf("WriteData:");

             AT24CXX_Write(50,I2C_Write,Lenth);			             //��0x50��ַд����		    
						 printf("24c02 Write Data:%s\r\n",I2C_Write);		     //��ӡд�������
	           		
						
           }
       }
			 
			  else if(!(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)))        //S4����
			 {
				  Delay_ms(10);
				  LED2(1);
				  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8))              //�ȴ������ɿ�
					{						
				    LED2(0);
						printf("Read Data:");	
						AT24CXX_Read(50,I2C_Read,Lenth);			                  //��0x50��ַ������
						printf("24c02 Read Data :%s\r\n",I2C_Read);
			   			
          }

       }
		
  }
}
