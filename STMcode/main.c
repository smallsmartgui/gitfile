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
   "\r\n|                                                      |\n\r"
   "\r\n|                                                      |\n\r"
   "\r\n|                                                      |\n\r"
   "\r\n|                                                      |\n\r"
   "\r\n|                                                      |\n\r"
   "\r\n|                                                      |\n\r"
   "\r\n|                                                      |\n\r"
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
	
	while(1)
	{		
	
		
  }
}
