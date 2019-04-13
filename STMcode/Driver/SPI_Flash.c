/**
  * @file    SPI_Flash.c 
  * @brief   SPI Flash Driver
  */
	
#include "SPI_Flash.h"
#include "Gpio.h"
#include "USART.h"
#include "string.h"
#include "stdbool.h"
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = PASSED;

/**
  * @brief  ��ʼ��SPI
  * @param  None
  * @retval None
  */
void SPI_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA |
                         RCC_APB2Periph_GPIO_CS, ENABLE);             //ʹ��SPI��GPIOʱ��


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                      //��������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                     //�������
  GPIO_Init(GPIO_CS, &GPIO_InitStructure);

//  SPI_FLASH_CS_HIGH();                                                  //ʧ��SPI Flash

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;     //˫��ȫ˫��
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                          //����ģʽ
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                      //8λ֡���ݽṹ
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                            //ʱ�����ո�
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                           //���ݲ����ڵڶ���ʱ����
	//SPI MODE3
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                              //�ڲ�NSS��SSI����
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;     //������Ԥ��ƵֵΪ2
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                     //���ݴ����MSB��ʼ
  SPI_InitStructure.SPI_CRCPolynomial = 7;                               //����CRCֵ����Ķ���ʽ 
  SPI_Init(SPI1, &SPI_InitStructure);
  SPI_Cmd(SPI1, ENABLE);                                                 //ʹ��SPI1
}


/**
  * @brief  д��һ���ֽڣ�����һ���ֽ�ͬʱ����
  * @param  byte:���͵��ֽ�
  * @retval ���յ��ֽ�
  */
u8 SPI_FLASH_SendByte(u8 byte)
{
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);//��鲢�ȴ�TX������Ϊ��

  SPI_I2S_SendData(SPI1, byte);                                    //SPI1��������
  
	//����ֻ���ڷ������ݵ�ʱ����ܲ���ʱ����������������ڷ��ͻ��ǽ�������ʱ��Ӧ����
	
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);//�ȴ�����һ���ֽ�
  //����ִ�е��˴���˵������һ���ֽ���ϡ� 
	//��Ϊ�������ݺͽ���������ͬʱ���еģ��жϷ����ֽ������SPI_I2S_FLAG_TXE��׼ȷ
	//Ӧʹ��SPI_I2S_FLAG_RXNE���ж��Ƿ���յ�һ���ֽڣ������յ��ˣ���˵��Ҳ���ͳ�ȥ��һ���ֽ�
	
  return SPI_I2S_ReceiveData(SPI1);                                //���ش�spi���߶�ȡ���ֽ�
}

/**
  * @brief  ��ȡһ���ֽ�
  * @param  None
  * @retval �������ȡ���ֽ� 0xA5
  */
u8 SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}

/**
  * @brief  ��ȡAT45DB16��ID
  * @param  None
  * @retval оƬID,��ȷΪ0x1F260000
  */
u32 SPI_FLASH_ReadID(void)
{
	u32 flash_id = 0;	  
	SPI_FLASH_CS_LOW();	//Ƭѡcs�͵�ƽ��Ч
	SPI_FLASH_SendByte(RDID);//���Ͷ�ȡID����    
	flash_id|=SPI_FLASH_SendByte(Dummy_Byte)<<24;
	flash_id|=SPI_FLASH_SendByte(Dummy_Byte)<<16;
	flash_id|=SPI_FLASH_SendByte(Dummy_Byte);	
	SPI_FLASH_CS_HIGH(); 					   
	return flash_id;
}

/**
  * @brief  д��һ������
  * @param  HalfWord:���͵İ�������
  * @retval ���յİ���
  */
u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  SPI_I2S_SendData(SPI1, HalfWord);                              //ͨ��SPI1���Ͱ���

  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);//�ȴ�����һ������

  return SPI_I2S_ReceiveData(SPI1);                               //���ش�SPI���߶�ȡ�İ���
}

/**
  * @brief  �ȴ�д�������
  * @param  None
  * @retval None
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;
  SPI_FLASH_CS_LOW();

  SPI_FLASH_SendByte(RDSR);//���Ͷ�״̬�Ĵ���

  do
  { 
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);
  }
  while ((FLASH_Status & BUSY_Flag) == RESET); 

  SPI_FLASH_CS_HIGH();
}

/**
  * @brief  �ַ����Ƚ�
  * @param  pBuffer1:ָ��Ŀ�껺��; pBuffer2:ָ��Դ����; BufferLength:�ֽ���
  * @retval FAILED:   pBuffer1!=pBuffer2; PASSED:pBuffer1==pBuffer2
  */
TestStatus Buffercmp(u8* pBuffer1, u8* pBuffer2, u16 BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}

/**
  * @brief  ��ȡ״̬�Ĵ���
  * @param  None
  * @retval �Ĵ���ֵ
  */
u8 DF_read_reg(void)
{
	unsigned char temp;	  
	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(RDSR);
	SPI_FLASH_SendByte(0x00);
	SPI_FLASH_SendByte(0x00);                        
	SPI_FLASH_SendByte(0x00);   
	temp= SPI_FLASH_SendByte(0xff);
	SPI_FLASH_CS_HIGH();	  
	return temp;
}

/**
  * @brief  ���״̬�Ĵ������λ�Ƿ�Ϊæ�����ȴ�����
  * @param  None
  * @retval æ����0   ��æ����,1
  */
u8 DF_wait_busy(void)
{
	unsigned char state_reg=0x00;
	u8 retry=0;
	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(RDSR);
	SPI_FLASH_SendByte(0x00);
	SPI_FLASH_SendByte(0x00);                        
	SPI_FLASH_SendByte(0x00);
	while((state_reg&0x80)==0&&retry<300)//����300��
	{
		state_reg=SPI_FLASH_SendByte(0x00);	
		retry++; 		 
	}
	SPI_FLASH_CS_HIGH();
	if(retry>=200)return 0;
	else return 1;
}

/**
  * @brief  ��ָ�����洢��ҳ������ת��ָ��������
  * @param  
  * @retval æ����0   ��æ����,1
  */
void DF_mm_to_buf(unsigned char buffer,unsigned int page)
{	  
	if(DF_wait_busy()==0)return;//����
	SPI_FLASH_CS_LOW();
	if (buffer==1)SPI_FLASH_SendByte(MM_PAGE_TO_B1_XFER);	  
	else SPI_FLASH_SendByte(MM_PAGE_TO_B2_XFER);  
	SPI_FLASH_SendByte((unsigned char)(page >> 6));
  SPI_FLASH_SendByte((unsigned char)(page << 2));
  SPI_FLASH_SendByte(0x00);
	SPI_FLASH_CS_HIGH();	   
} 

/**
  * @brief  ��ָ���������е�����д�����洢����ָ��ҳ
  * @param  
  * @retval æ����0   ��æ����,1
  */
void DF_buf_to_mm(u8 buffer,u16 page)
{
	if(DF_wait_busy()==0)return;                              //����
	if (page<SPI_FLASH_PageNum)
	{
		SPI_FLASH_CS_LOW();
		if (buffer==1)SPI_FLASH_SendByte(B1_TO_MM_PAGE_PROG_WITH_ERASE); 
		else SPI_FLASH_SendByte(B2_TO_MM_PAGE_PROG_WITH_ERASE);	 
		SPI_FLASH_SendByte((unsigned char)(page>>6));
		SPI_FLASH_SendByte((unsigned char)(page<<2));
		SPI_FLASH_SendByte(0x00);
		SPI_FLASH_CS_HIGH();
	}
}

/**
  * @brief  ҳ����
  * @param  PageAddr:������ҳ��ַ
  * @retval None
  */
void SPI_FLASH_PageErase(u32 page)
{
  if(DF_wait_busy()==0)return;                             //����  
	SPI_FLASH_CS_LOW();                                      //����Ƭѡ
	SPI_FLASH_SendByte(PE);                                  //����ҳ��������
	SPI_FLASH_SendByte((unsigned char)(page >> 6));
	SPI_FLASH_SendByte((unsigned char)(page << 2));
	SPI_FLASH_SendByte(0x00);
	SPI_FLASH_CS_HIGH();	                                    //����Ƭѡ
}

/**
  * @brief  ��Ƭ����
  * @param  None
  * @retval None
  */
void SPI_FLASH_BulkErase(void)
{
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(BE1);                                               //������������ָ��
  SPI_FLASH_SendByte(BE2);
  SPI_FLASH_SendByte(BE3);
  SPI_FLASH_SendByte(BE4);

  SPI_FLASH_CS_HIGH();

  SPI_FLASH_WaitForWriteEnd();
}

/**
  * @brief  ��ָ����ַ��ʼд��ָ������������	
  * @param  
  * @retval None
  */
void SPI_Flash_Write(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u16 offaddr,i;
	u32 paddr;//ҳ��ַ
	paddr=WriteAddr/SPI_FLASH_PageSize;         //�õ���ʼд�����ҳ��ַ
	offaddr=WriteAddr%SPI_FLASH_PageSize;       //�õ�����ҳ��ַ���ƫ��
	DF_mm_to_buf(1,paddr);                      //����ʼҳ���ݶ�����buf1			  
	if(DF_wait_busy()==0)return;                //����
	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(BUFFER_1_WRITE);         //д��1����������	 		   
	SPI_FLASH_SendByte(0x00);                   //14bit ��Ч����+10bit��ַ���� 
	SPI_FLASH_SendByte((u8)(offaddr>>8));       //д���ڸ�ҳ��ƫ�Ƶ�ַ   
	SPI_FLASH_SendByte((u8)offaddr);
	for (i=0;i<NumByteToWrite;)                 //��������
	{
		SPI_FLASH_SendByte(*pBuffer);             //д��һ������
		pBuffer++;
		i++;
		if((i+offaddr)%SPI_FLASH_PageSize==0)
		{
			SPI_FLASH_CS_HIGH();
			DF_buf_to_mm(1,paddr);                  //��BUF1������д�����洢��
			paddr++;	   
			if(paddr>=SPI_FLASH_PageNum)return;     //������AT45DB161�ķ�Χ
			DF_mm_to_buf(1,paddr);                  //����ʼҳ���ݶ�����buf1			  
			if(DF_wait_busy()==0)return;            //����
			SPI_FLASH_CS_LOW();
			SPI_FLASH_SendByte(BUFFER_1_WRITE);     //д��1����������	 	 		   
			SPI_FLASH_SendByte(0x00);               //14bit ��Ч����+10bit��ַ���� 
			SPI_FLASH_SendByte(0x00);               //���õ�ַ��0 
			SPI_FLASH_SendByte(0x00);		    
		}			   
	}
	SPI_FLASH_CS_HIGH();	
	DF_buf_to_mm(1,paddr);                       //��BUF1������д�����洢��			  
} 

/**
  * @brief  ��ָ����ַ��ʼ����ָ������������	
  * @param  
  * @retval None
  */
void SPI_Flash_Read(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	u16 offaddr,i;
	u32 paddr;                                //ҳ��ַ
	paddr=ReadAddr/SPI_FLASH_PageSize;        //�õ���ʼд�����ҳ��ַ
	offaddr=ReadAddr%SPI_FLASH_PageSize;      //�õ�����ҳ��ַ���ƫ��
	DF_mm_to_buf(1,paddr);                    //����ʼҳ���ݶ�����buf1			  
	if(DF_wait_busy()==0)return;              //����
	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(BUFFER_1_READ);        //��1����������	 		   
	SPI_FLASH_SendByte(0x00);                 //14bit ��Ч����+10bit��ַ���� 
	SPI_FLASH_SendByte((u8)(offaddr>>8));     //д���ڸ�ҳ��ƫ�Ƶ�ַ   
	SPI_FLASH_SendByte((u8)offaddr);
	SPI_FLASH_SendByte(0x00);                 //�ȴ� 
	for (i=0;i<NumByteToRead;)                //��ȡNumByteToRead������
	{
		*pBuffer=SPI_FLASH_SendByte(0xff);      //��ȡһ������
		pBuffer++;
		i++;
		if((i+offaddr)%SPI_FLASH_PageSize==0)
		{
			SPI_FLASH_CS_HIGH();										  
			paddr++;	    
			if(paddr>=SPI_FLASH_PageNum)return;  //������AT45DB161�ķ�Χ
			DF_mm_to_buf(1,paddr);               //����ʼҳ���ݶ�����buf1			  
			if(DF_wait_busy()==0)return;         //����
			SPI_FLASH_CS_LOW();
			SPI_FLASH_SendByte(BUFFER_1_READ);   //��1����������	 		   
			SPI_FLASH_SendByte(0x00);            //14bit ��Ч����+10bit��ַ���� 
			SPI_FLASH_SendByte(0x00);            //���õ�ַ��0 
			SPI_FLASH_SendByte(0x00);
			SPI_FLASH_SendByte(0x00);            //�ȴ�		    
		}			   
	}
	SPI_FLASH_CS_HIGH();									  			  
} 

/**
  * @brief  �������
  * @param  pBuffer:ָ����仺��;BufferLenght:���ݳ���;Offset:��ʼ����ֵ
  * @retval None
  */
static void Fill_Buffer(u8 *pBuffer, u16 BufferLenght, u32 Offset)
{
  u16 IndexTmp = 0;

  for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
  }
}

/**
  * @brief  crc16����
  * @param  data:ָ������黺��;length:У������ݳ���;start:�������ʼλ��
  * @retval У��ֵ
  */
u16 check_crc16(u8 *data,u16 length,u16 start)
{
  u16 crc = 0;
	u8 i,bt;
  while (length-- > 0)
  {
     bt = data[start++];
    for ( i = 0; i < 8; i++)
    {
      bool b1 = (crc & 0x8000U) != 0;
      bool b2 = (bt & 0x80U) != 0;
      if (b1 != b2) crc = (u16)((crc << 1) ^ 0x1021);
      else crc <<= 1;
      bt <<= 1;
    }
  }
  return crc;
}


/**
  * @brief  flash����.�۲촮�ڵ������ִ�ӡ������
  * @param  None
  * @retval None
  */
void SPI_FLASH_Test(void)
{
	u8 i,pagenum;
	vu32 db_ID = 0;
	u16 offset,crc1,crc2;
	u32  FLASH_WriteAddress=0x00000000;
  u8 buf1[SPI_FLASH_PageSize]={0};
  db_ID = SPI_FLASH_ReadID();
	printf("\r\nid=0x%x\r\n",db_ID);              //Flash ID:0x1F260000
	Fill_Buffer(buf1, SPI_FLASH_PageSize , 0x00);                 //�������
 	printf("Fill_Buffer\r\n");
	for(offset=0;offset<SPI_FLASH_PageSize;offset++)
	{
	  printf("%d ",buf1[offset]);
	}
	crc1=check_crc16(buf1,sizeof(buf1),0);
	printf("\r\nԭʼ����crc16=0x%x\r\n",crc1);
#if ONLY_READ                                  //flash��һ���Ĳ�д����,�𷴸���ͬһ��ַ��д
	for(i = 0;i<PageNum_TEST;i++)
  SPI_FLASH_PageErase(i);                                         //ҳ����,spi flashд֮ǰҪ�����������.
	
	for(i=0;i<PageNum_TEST;i++)
	{
		FLASH_WriteAddress=FLASH_WriteAddress+SPI_FLASH_PageSize*i;
		printf("\r\nwrite address=%d\r\n",FLASH_WriteAddress);
	  SPI_Flash_Write(buf1,FLASH_WriteAddress,SPI_FLASH_PageSize);    //д����3ҳ����
	}
#endif
	FLASH_WriteAddress=0;
	for(i=0;i<PageNum_TEST;i++)                                              //��ȡ3ҳ����
	{	
   		memset((u8*)buf1,0,sizeof(buf1));
			printf("\r\n");
		  FLASH_WriteAddress =FLASH_WriteAddress+SPI_FLASH_PageSize*i;
		  printf("\r\nread address=%d\r\n",FLASH_WriteAddress);
			SPI_Flash_Read(buf1,FLASH_WriteAddress,SPI_FLASH_PageSize);   
      pagenum=i+1;		
		  printf("\r\n------------------------------------------------------------------------------------\r\n");
			printf("������%dҳ����\r\n",pagenum);
		  printf("page[%d]=",pagenum);
			for(offset=0;offset<SPI_FLASH_PageSize;offset++)
			{
				printf("%d ",buf1[offset]);
			}
			
			printf("\r\n��ʼ�����%dҳ����\r\ncheck...",pagenum);
			crc2=check_crc16(buf1,sizeof(buf1),0);
			if(crc1==crc2)
			{
			  printf("\r\n��%dҳ������ȷ\r\n",pagenum);
			}
			else
			{  
				printf("\r\n��%dҳ�������,crc16=0x%x\r\n",pagenum,crc2);
			}
			printf("\r\n-----------------------------------------------------------------------------------\r\n");
		}
}
