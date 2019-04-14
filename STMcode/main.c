#include "Gpio.h"
#include "delay.h"
#include "USART.h"
#include "24c02.h"
#include "ili9320.h"
#include "SPI_Flash.h" 
u8 buf[SPI_FLASH_PageSize];
u8 Cn_Buffer[]="福州大学";
const char menu[] =
   "\r\n                                                        \n\r"
   "\r\n+******************************************************+\n\r"
   "\r\n|                                                      |\n\r"
   "\r\n|    该段测试先提取了id，再对1页进行清除               |\n\r"
   "\r\n|                         1page=528bit                 |\n\r"
   "\r\n|    并在第0页写入福州大学                             |\n\r"
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
  printf("\r\n设备id为：0x%x \r\n",id);
	SPI_FLASH_PageErase(0);
	SPI_FLASH_WaitForWriteEnd();
	SPI_Flash_Write(Cn_Buffer,0,BufferSize);
	SPI_FLASH_WaitForWriteEnd();
	SPI_Flash_Read(buf,0,BufferSize);
	printf("读出的数据为：%s ",buf);

	while(1)
	{			
  }
}
