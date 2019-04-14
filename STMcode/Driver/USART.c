/**
  * @file    USART.c 
  * @author  WB R&D Team - openmcu666
  * @version V1.0
  * @date    2016.05.04
  * @brief   USART Driver
  */

#include "USART.h"

u8 USART1_TX_Finish=1;
u8 USART2_TX_Finish=1;
uint8_t USART1_SEND_DATA[DATA_BUF_SIZE];		  // ���ջ�����
uint8_t USART1_RECEIVE_DATA[DATA_BUF_SIZE];		// ���ͻ�����

uint8_t USART2_SEND_DATA[DATA_BUF_SIZE];		   // ���ջ�����
uint8_t USART2_RECEIVE_DATA[DATA_BUF_SIZE];		 // ���ͻ�����
/**
  * @brief  USART1��ʼ������ ����GPIO��ʼ�� TX��������Ϊ�������
  * @param  None
  * @retval None
  */
void USART1_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA  |RCC_APB2Periph_AFIO,  ENABLE );

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                   //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;              //��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate            = 115200  ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;           //8������λ
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;              //1��ֹͣλ
	USART_InitStructure.USART_Parity              = USART_Parity_No ;              //����żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//����Ӳ��������
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  //ʹ�ܷ���/����
	USART_Init(USART1, &USART_InitStructure);                           

	USART_ITConfig(USART1, USART_IT_IDLE , ENABLE);                                   //ʹ�ܿ����ж�
	USART_Cmd(USART1, ENABLE);     
	USART_ClearFlag(USART1, USART_FLAG_TC);                                          //�巢����ɱ�־
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			                            //�жϷ���2
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 		                          //USART1�����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;                       //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		                          //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
}

/**
  * @brief  USART1��������
  * @param  None
  * @retval None
  */
void USART1_SendByte(u16 Data)
{ 
   while (!(USART1->SR & USART_FLAG_TXE));
   USART1->DR = (Data & (uint16_t)0x01FF);	 
   
}

/**
  * @brief  USART1�����ַ���
  * @param  *buf:���ͻ�������ַ len:��Ҫ���͵����ݳ���
  * @retval None
  */
void USART1_SendString(u8 *buf,uint16_t len)
{
	uint16_t t; 
  for(t=0;t<len;t++)		                                     //ѭ����������
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		
		USART_SendData(USART1,buf[t]);                           //��������
	}	 
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);   //�ȴ����ͽ���	
}

/**
  * @brief  USART2��ʼ������ ����GPIO��ʼ�� TX��������Ϊ�������
  * @param  None
  * @retval None
  */
void USART2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA  ,  ENABLE );
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3, ENABLE );
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                       //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                          //��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate            = 9600  ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;           //8������λ
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;              //1��ֹͣλ
	USART_InitStructure.USART_Parity              = USART_Parity_No ;              //����żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//����Ӳ��������
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;//ʹ�ܷ���/����
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_IDLE , ENABLE);                                //��������,֡��,����,У����ж� 
	USART_Cmd(USART2, ENABLE);   
  USART_ClearFlag(USART2, USART_FLAG_TC);                                         //�巢����ɱ�־
	
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 		                          //USART2�����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;                       //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		                          //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

/**
  * @brief  USART2��������
  * @param  None
  * @retval None
  */
void USART2_SendByte(u16 Data)
{ 
   while (!(USART2->SR & USART_FLAG_TXE));
   USART2->DR = (Data & (uint16_t)0x01FF);	   
}

/**
  * @brief  USART2�����ַ���
  * @param  None
  * @retval None
  */
void USART2_SendString(uint8_t *ch)
{
	while(*ch!=0)
	{		
		while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
		USART_SendData(USART2, *ch);
		ch++;
	}   	
}

/**
  * @brief  USART1��USART2�����ַ���
  * @param  None
  * @retval None
  */
void SendMessage(void)
{
	const char menu[] =
   "\n\r"
   "\n\r+******************** ��ţ������  **********************+\n\r"
   "\n\r|                  USART �첽ͨ������                   |\n\r"
   "\n\r|                 ��ӭ������̳��������                  |\n\r"
   "\n\r|                 www.openmcu.com/bbs                   |\n\r"
   "\n\r|                     2016.05.04                        |\n\r"
   "\n\r|                ����֧��Ⱥ��259180202                  |\n\r"
   "\n\r+-------------------------------------------------------+\n\r";

   printf(menu);
   printf("\n\rHello World! USART1_Test\n\r");
	 USART2_SendString("\n\rUSART2_Test>>������a~c������ĸ����LED������\n\r");
}

/**
  * @brief  USART1��USART2 DMA����
  * @param  None
  * @retval None
  */
void USART_DMAConfiguration(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  /* DMA clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA1

  /* DMA1 Channel4 (triggered by USART1 Tx event) Config */
  DMA_DeInit(DMA1_Channel4);  
  DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40013804;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_SEND_DATA;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = 512;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel4, &DMA_InitStructure);
  
  DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
  DMA_ITConfig(DMA1_Channel4, DMA_IT_TE, ENABLE);
  /* Enable USART1 DMA TX request */
  USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
  
  DMA_Cmd(DMA1_Channel4, DISABLE);
  
  /* DMA1 Channel5 (triggered by USART2 Tx event) Config */
  DMA_DeInit(DMA1_Channel7);  
  DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40004404;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART2_SEND_DATA;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = 512;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel7, &DMA_InitStructure);
  
  DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
  DMA_ITConfig(DMA1_Channel7, DMA_IT_TE, ENABLE);
  /* Enable USART1 DMA TX request */
  USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
  
  DMA_Cmd(DMA1_Channel7, DISABLE);
  //2012-7-13add(e)
  
  /* DMA1 Channel5 (triggered by USART1 Rx event) Config */
  DMA_DeInit(DMA1_Channel5);  
  DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40013804;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_RECEIVE_DATA;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 512;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel5, &DMA_InitStructure);
  
  DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
  DMA_ITConfig(DMA1_Channel5, DMA_IT_TE, ENABLE);
  
  /* Enable USART1 DMA RX request */
  USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
  
  DMA_Cmd(DMA1_Channel5, ENABLE);
  
  /* DMA1 Channel6 (triggered by USART1 Rx event) Config */
  DMA_DeInit(DMA1_Channel6);  
  DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40004404;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART2_RECEIVE_DATA;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 512;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel6, &DMA_InitStructure);
  
  DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);
  DMA_ITConfig(DMA1_Channel6, DMA_IT_TE, ENABLE);
  
  /* Enable USART2 DMA RX request */
  USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
  
  DMA_Cmd(DMA1_Channel6, ENABLE);
}


#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/*******************************************************************************
* Function Name  : PUTCHAR_PROTOTYPE
* Description    : ��USART�������printf
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (u8) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

