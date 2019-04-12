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
  * @brief  RTC����
  * @param  None
  * @retval None
  */
void RTC_Configuration(void)
{	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE );
	PWR_BackupAccessCmd(ENABLE);                           //�������BKP������
	
	BKP_DeInit();	                                         //��λ������
	
	RCC_LSEConfig(RCC_LSE_ON);	                           //����LSE
	
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	   //�ȴ�LSE����
	{}
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);	               //ѡ��LSEΪRTCʱ��Դ
		
	RCC_RTCCLKCmd(ENABLE);	                               //RTCʱ��ʹ��
	
	RTC_WaitForSynchro();	                                 //�ȴ�RTC�Ĵ���ͬ��
	
	RTC_WaitForLastTask();	                               //�ȴ�����RTC�Ĵ�����д�������
	

	RTC_ITConfig(RTC_IT_SEC, ENABLE);	                     //RTC�ж�ʹ��
	
	RTC_WaitForLastTask();	                               //�ȴ�����RTC�Ĵ�����д�������
	
	RTC_SetPrescaler(32767); //����RTCʱ�ӷ�Ƶֵ32767�������Ƶ��= (32.768 KHz)/(32767+1)=1Hz(1s)
	
	RTC_WaitForLastTask();	  //�ȴ�����RTC�Ĵ�����д�������
		
}

/**
  * @brief  �ӳ����ն˻�ȡ����ֵ
  * @param  value:ʱ��ֵ
  * @retval None
  */
u8 USART_Scanf(u32 value)
{
  u32 index = 0;
  u32 tmp[2] = {0, 0};

  while (index < 2)
  {

    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET) //ѭ��ֱ��rxne = 1
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
  * @brief  �����û������ʱ��
  * @param  None
  * @retval ���ص�ǰʱ��ʱ�Ӽ�������ֵ
  */
u32 Time_Regulate(void)
{
  u32 Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;
  u8 error=0;
  printf("\r\n==============Time Settings======================");
	printf("\r\nexample:08:00:00\r\n");

  do
  { 
	  while(data_length<8){;}                                      //�ȴ�����00:00:00
	  error=0;
    Tmp_HH=(rx_buffer[1] - 0x30) + ((rx_buffer[0] - 0x30) * 10);
	  if(Tmp_HH>23) {error=1;}
	  Tmp_MM=(rx_buffer[4] - 0x30) + ((rx_buffer[3] - 0x30) * 10);
	  if(Tmp_MM>59) {error=2;}
	  Tmp_SS=(rx_buffer[7] - 0x30) + ((rx_buffer[6] - 0x30) * 10);//�ж�ʱ���Ƿ�����
	  if(Tmp_SS>59) {error=3;}
	  if(error!=0){printf("\r\nInput error,  please try again\r\n");memset(rx_buffer,0x00,DATA_BUF_SIZE); data_length=0;}
  } while(error!=0);

  return((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS));

}

/**
  * @brief  ����ʱ��
  * @param  None
  * @retval None
  */
void Time_Adjust(void)
{
  RTC_WaitForLastTask();	          //�ȴ�����RTC�Ĵ�����д�������

  RTC_SetCounter(Time_Regulate());	//�ı䵱ǰʱ��
 
  RTC_WaitForLastTask();		        //�ȴ�����RTC�Ĵ�����д�������
}

/**
  * @brief  ��ʾ��ǰʱ��
  * @param  TimeVar	RTC��������ֵ
  * @retval None
  */
void Time_Display(u32 TimeVar)
{
  
  THH = TimeVar / 3600;			       //ʱ
 
  TMM = (TimeVar % 3600) / 60;	   //��
 
  TSS = (TimeVar % 3600) % 60;	   //��

  printf("\r\nTime: %0.2d:%0.2d:%0.2d\r\n", THH, TMM, TSS);
}

/**
  * @brief  RTC��ʼ��
  * @param  None
  * @retval None
  */
void RTC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	  //�������ȼ�����

  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)             //�ж�BKP_DR1�Ĵ���ֵ�Ƿ�Ϊ0xA5A5
	{
		LED1(1);
		printf("\r\n RTC not yet configured....");		         //RTC��δ����
		
		RTC_Configuration();                                   //RTC����
		
		printf("\r\n RTC configured....");
		
		Time_Adjust();		                                     //����ʱ��ֵ
		
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);	             //��0xA5A5д��BKP_DR1�Ĵ���
	}
	else
	{
		LED2(1);
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)       //���POWER�Ƿ�����λ��־
		{
			printf("\r\n\n Power On Reset occurred....");
		}
		
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)	 //���PIN��λ��־����
		{
			printf("\r\n\n External Reset occurred....");
		}
		
		printf("\r\n No need to configure RTC....");
		RTC_WaitForSynchro();	                                //�ȴ�RTC�Ĵ���ͬ��
		
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	                    //RTC�ж�ʹ��
		
		RTC_WaitForLastTask();	                              //�ȴ�����RTC�Ĵ�����д�������
	}
	
#ifdef RTCClockOutput_Enable

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);	                        //�������BKP������
	
	BKP_TamperPinCmd(DISABLE);                            //����Tamper����
	
	BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);  //ʹ��ʱ������۸�����
#endif

	RCC_ClearFlag();                                      //�����־λ

}
