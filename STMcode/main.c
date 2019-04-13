#include "Gpio.h"
#include "delay.h"
#include "USART.h"
#include "24c02.h"
#include "ili9320.h"
#include "SPI_Flash.h" 
u8 buf[528];
u8 buf1[528]={1,2,3,4,5};
const char menu[] =
   "\r\n                                                        \n\r"
   "\r\n+******************************************************+\n\r"
   "\r\n|                                                      |\n\r"
   "\r\n|    该段测试先提取了id，再对1页进行清除               |\n\r"
   "\r\n|                         1page=528bit                 |\n\r"
   "\r\n|    并在第0页的第300字节写入数据                      |\n\r"
   "\r\n+------------------------------------------------------+\n\r";

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main(void)
{   
	u32 id;
	u16 i;
	
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
  printf("\r\n 0x%x \r\n",id);
	SPI_FLASH_PageErase(0);
	SPI_FLASH_WaitForWriteEnd();
	SPI_Flash_Write(buf1,300,5);
	SPI_FLASH_WaitForWriteEnd();
	SPI_Flash_Read(buf,0,528);
	for(i=0;i<528;i++)
		{
			printf("0x%x ",buf[i]);
			if(i%10==0)
			{
				printf("\r\n");
			}
		}
	while(1)
	{			
  }
}
