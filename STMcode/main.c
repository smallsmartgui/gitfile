#include "Gpio.h"
#include "delay.h"
#include "USART.h"
#include "24c02.h"
#include "ili9320.h"
#include "SPI_Flash.h" 
u8 Receive_Buffer[SPI_FLASH_PageSize];
u8 Send_Buffer[]="���ݴ�ѧ";
const char menu[] =
   "\r\n                                                        \n\r"
   "\r\n+******************************************************+\n\r"
   "\r\n|                                                      |\n\r"
   "\r\n|                  �öβ�������ȡ��id���ٶ�1ҳ������� |\n\r"
   "\r\n|                                                      |\n\r"
   "\r\n|                                 ���ڵ�0ҳд�븣�ݴ�ѧ|\n\r"
   "\r\n+------------------------------------------------------+\n\r"
   "\r\n+******************************************************+\n\r";
/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main(void)
{   
	u32 ID;
	u8 Check_Status;
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
	ID=SPI_FLASH_ReadID();
  printf("\r\n�豸idΪ��0x%x \r\n",ID);
	SPI_FLASH_PageErase(0);
	SPI_FLASH_WaitForWriteEnd();
	SPI_Flash_Write(Send_Buffer,0,BufferSize);
	SPI_FLASH_WaitForWriteEnd();
	SPI_Flash_Read(Receive_Buffer,0,BufferSize);
	printf("����������Ϊ��%s ",Receive_Buffer);
	while(1)
	{			
  }
}
