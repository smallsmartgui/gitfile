/**
  * @file    RTC.c 
  * @author  WB R&D Team - openmcu666
  * @version V1.0
  * @date    2016.05.04
  * @brief   Real-Time Clock Driver
  */
	
#include "RTC.h"

u32 THH = 0, TMM = 0, TSS = 0;

/**
  * @brief  RTC配置
  * @param  None
  * @retval None
  */
void RTC_Configuration(void)
{	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE );
	PWR_BackupAccessCmd(ENABLE);                           //允许访问BKP备份域
	
	BKP_DeInit();	                                         //复位备份域
	
	RCC_LSEConfig(RCC_LSE_ON);	                           //开启LSE
	
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	   //等待LSE起振
	{}
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);	               //选择LSE为RTC时钟源
		
	RCC_RTCCLKCmd(ENABLE);	                               //RTC时钟使能
	
	RTC_WaitForSynchro();	                                 //等待RTC寄存器同步
	
	RTC_WaitForLastTask();	                               //等待最后对RTC寄存器的写操作完成
	

	RTC_ITConfig(RTC_IT_SEC, ENABLE);	                     //RTC中断使能
	
	RTC_WaitForLastTask();	                               //等待最后对RTC寄存器的写操作完成
	
	RTC_SetPrescaler(32767); //设置RTC时钟分频值32767，则计数频率= (32.768 KHz)/(32767+1)=1Hz(1s)
	
	RTC_WaitForLastTask();	  //等待最后对RTC寄存器的写操作完成
		
}

/**
  * @brief  从超级终端获取数字值
  * @param  value:时间值
  * @retval None
  */
u8 USART_Scanf(u32 value)
{
  u32 index = 0;
  u32 tmp[2] = {0, 0};

  while (index < 2)
  {

    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET) //循环直到rxne = 1
    {}
    tmp[index++] = (USART_ReceiveData(USART1));
    if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
    {
      printf("\n\rPlease enter valid number between 0 and 9");
      index--;
    }
  }
  /* Calculate the Corresponding value */
  index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
  
  if (index > value)
  {
    printf("\n\rPlease enter valid number between 0 and %d", value);
    return 0xFF;
  }
	data_length=0;
  return index;
}

/**
  * @brief  返回用户输入的时间
  * @param  None
  * @retval 返回当前时间时钟计数器的值
  */
u32 Time_Regulate(void)
{
  u32 Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;
  u8 error=0;
  printf("\r\n==============Time Settings======================");
	printf("\r\nexample:08:00:00\r\n");

  do
  { 
	  while(data_length<8){;}                                      //等待输入00:00:00
	  error=0;
    Tmp_HH=(rx_buffer[1] - 0x30) + ((rx_buffer[0] - 0x30) * 10);
	  if(Tmp_HH>23) {error=1;}
	  Tmp_MM=(rx_buffer[4] - 0x30) + ((rx_buffer[3] - 0x30) * 10);
	  if(Tmp_MM>59) {error=2;}
	  Tmp_SS=(rx_buffer[7] - 0x30) + ((rx_buffer[6] - 0x30) * 10);//判断时间是否有误
	  if(Tmp_SS>59) {error=3;}
	  if(error!=0){printf("\r\nInput error,  please try again\r\n");memset(rx_buffer,0x00,DATA_BUF_SIZE); data_length=0;}
  } while(error!=0);

  return((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS));

}

/**
  * @brief  调整时间
  * @param  None
  * @retval None
  */
void Time_Adjust(void)
{
  RTC_WaitForLastTask();	          //等待最后对RTC寄存器的写操作完成

  RTC_SetCounter(Time_Regulate());	//改变当前时间
 
  RTC_WaitForLastTask();		        //等待最后对RTC寄存器的写操作完成
}

/**
  * @brief  显示当前时间
  * @param  TimeVar	RTC计数器的值
  * @retval None
  */
void Time_Display(u32 TimeVar)
{
  
  THH = TimeVar / 3600;			       //时
 
  TMM = (TimeVar % 3600) / 60;	   //分
 
  TSS = (TimeVar % 3600) % 60;	   //秒

  printf("\r\nTime: %0.2d:%0.2d:%0.2d\r\n", THH, TMM, TSS);
}

/**
  * @brief  RTC初始化
  * @param  None
  * @retval None
  */
void RTC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	  //设置优先级分组

  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)             //判断BKP_DR1寄存器值是否为0xA5A5
	{
		LED1(1);
		printf("\r\n RTC not yet configured....");		         //RTC尚未配置
		
		RTC_Configuration();                                   //RTC配置
		
		printf("\r\n RTC configured....");
		
		Time_Adjust();		                                     //调整时间值
		
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);	             //把0xA5A5写入BKP_DR1寄存器
	}
	else
	{
		LED2(1);
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)       //检查POWER是否发生复位标志
		{
			printf("\r\n\n Power On Reset occurred....");
		}
		
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)	 //检查PIN复位标志设置
		{
			printf("\r\n\n External Reset occurred....");
		}
		
		printf("\r\n No need to configure RTC....");
		RTC_WaitForSynchro();	                                //等待RTC寄存器同步
		
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	                    //RTC中断使能
		
		RTC_WaitForLastTask();	                              //等待最后对RTC寄存器的写操作完成
	}
	
#ifdef RTCClockOutput_Enable

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);	                        //允许访问BKP备份域
	
	BKP_TamperPinCmd(DISABLE);                            //禁用Tamper引脚
	
	BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);  //使能时钟输出篡改引脚
#endif

	RCC_ClearFlag();                                      //清除标志位

}
