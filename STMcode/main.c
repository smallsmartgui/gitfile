#include "Gpio.h"
#include "delay.h"
#include "USART.h"
#include "24c02.h"
#include "ili9320.h"
const char menu[] =
   "\r\n                                                        \n\r"
   "\r\n+******************************************************+\n\r"
   "\r\n|                                                      |\n\r"
   "\r\n|    这个程序实现了对24C02芯片的读写测试。24C02共 256  |\n\r"
   "\r\n|   字节的存储空间，页大小为8字节，可以按字节或页进行  |\n\r"
   "\r\n|   读写。本程序在Keil MDK 5.10  ST库v3.50下测试通过。 |\n\r"
   "\r\n|                                                      |\n\r"
   "\r\n|          使用举例：                                  |\n\r"
   "\r\n|                   按下S2,从0x50地址处写入数据        |\n\r"
   "\r\n|                   按下S3,从0x50地址处读出数据        |\n\r"
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
  Delay_Init();                                            //systick初始化
	Init_IIC();					                                     //24LC02初始化 
  printf ( menu );                                         //显示菜单    
  printf("\r\n    \r\n");	
	while(1)
	{		
		if(!(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)))                   //S3按下
			 {
				 Delay_ms(10);
				  LED2(1);
				  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))           //等待按键松开
					{
						 LED2(0);
						 printf("WriteData:");

             AT24CXX_Write(50,I2C_Write,Lenth);			             //在0x50地址写数据		    
						 printf("24c02 Write Data:%s\r\n",I2C_Write);		     //打印写入的数据
	           		
						
           }
       }
			 
			  else if(!(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)))        //S4按下
			 {
				  Delay_ms(10);
				  LED2(1);
				  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8))              //等待按键松开
					{						
				    LED2(0);
						printf("Read Data:");	
						AT24CXX_Read(50,I2C_Read,Lenth);			                  //在0x50地址读数据
						printf("24c02 Read Data :%s\r\n",I2C_Read);
			   			
          }

       }
		
  }
}
