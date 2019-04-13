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
  Delay_Init();                                            //systick初始化
  printf ( menu );                                         //显示菜单    
  printf("\r\n    \r\n");	
	SPI_FLASH_Init();
	id=SPI_FLASH_ReadID();
  printf("\r\n 0x%x   \r\n",id);
	while(1)
	{		
	
		
  }
}
