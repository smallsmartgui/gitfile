/**
  * @file    USART.c 
  * @author  WB R&D Team - openmcu666
  * @version V1.0
  * @date    2016.05.04
  * @brief   USART Driver
  */

#include "USART.h"

uint16_t data_length;								// ���ݳ���
uint8_t rx_buffer[DATA_BUF_SIZE];		// ���ջ�����
uint8_t tx_buffer[DATA_BUF_SIZE];		// ���ͻ�����
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
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA  ,  ENABLE );

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
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                                  //ʹ�ܽ����ж�
	USART_Cmd(USART1, ENABLE);     
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			                            //�жϷ���2
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 		                          //USART1�����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                       //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		                          //��ռ���ȼ�
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                 //��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate            = 115200  ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;  //8������λ
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;     //1��ֹͣλ
	USART_InitStructure.USART_Parity              = USART_Parity_No ;     //����żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//����Ӳ��������
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;//ʹ�ܷ���/����
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);   

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 		                          //USART2�����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                       //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		                          //��ռ���ȼ�
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

