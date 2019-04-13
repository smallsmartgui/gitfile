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
  * @brief  初始化SPI
  * @param  None
  * @retval None
  */
void SPI_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA |
                         RCC_APB2Periph_GPIO_CS, ENABLE);             //使能SPI和GPIO时钟


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                      //复用推挽
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                     //推挽输出
  GPIO_Init(GPIO_CS, &GPIO_InitStructure);

//  SPI_FLASH_CS_HIGH();                                                  //失能SPI Flash

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;     //双向全双工
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                          //主机模式
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                      //8位帧数据结构
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                            //时钟悬空高
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                           //数据捕获于第二个时钟沿
	//SPI MODE3
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                              //内部NSS由SSI控制
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;     //波特率预分频值为2
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                     //数据传输从MSB开始
  SPI_InitStructure.SPI_CRCPolynomial = 7;                               //用于CRC值计算的多项式 
  SPI_Init(SPI1, &SPI_InitStructure);
  SPI_Cmd(SPI1, ENABLE);                                                 //使能SPI1
}


/**
  * @brief  写入一个字节，接收一个字节同时进行
  * @param  byte:发送的字节
  * @retval 接收的字节
  */
u8 SPI_FLASH_SendByte(u8 byte)
{
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);//检查并等待TX缓冲区为空

  SPI_I2S_SendData(SPI1, byte);                                    //SPI1发送数据
  
	//主机只有在发送数据的时候才能产生时序，因此上两行无论在发送还是接收数据时都应该有
	
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);//等待接收一个字节
  //程序执行到此处，说明发送一个字节完毕。 
	//因为发送数据和接收数据是同时进行的，判断发送字节完成用SPI_I2S_FLAG_TXE不准确
	//应使用SPI_I2S_FLAG_RXNE来判断是否接收到一个字节，若接收到了，就说明也发送出去了一个字节
	
  return SPI_I2S_ReceiveData(SPI1);                                //返回从spi总线读取的字节
}

/**
  * @brief  读取一个字节
  * @param  None
  * @retval 从闪存读取的字节 0xA5
  */
u8 SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}

/**
  * @brief  读取AT45DB16的ID
  * @param  None
  * @retval 芯片ID,正确为0x1F260000
  */
u32 SPI_FLASH_ReadID(void)
{
	u32 flash_id = 0;	  
	SPI_FLASH_CS_LOW();	//片选cs低电平有效
	SPI_FLASH_SendByte(RDID);//发送读取ID命令    
	flash_id|=SPI_FLASH_SendByte(Dummy_Byte)<<24;
	flash_id|=SPI_FLASH_SendByte(Dummy_Byte)<<16;
	flash_id|=SPI_FLASH_SendByte(Dummy_Byte);	
	SPI_FLASH_CS_HIGH(); 					   
	return flash_id;
}

/**
  * @brief  写入一个半字
  * @param  HalfWord:发送的半字数据
  * @retval 接收的半字
  */
u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  SPI_I2S_SendData(SPI1, HalfWord);                              //通过SPI1发送半字

  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);//等待接收一个半字

  return SPI_I2S_ReceiveData(SPI1);                               //返回从SPI总线读取的半字
}

/**
  * @brief  等待写操作完成
  * @param  None
  * @retval None
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;
  SPI_FLASH_CS_LOW();

  SPI_FLASH_SendByte(RDSR);//发送读状态寄存器

  do
  { 
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);
  }
  while ((FLASH_Status & BUSY_Flag) == RESET); 

  SPI_FLASH_CS_HIGH();
}

/**
  * @brief  字符串比较
  * @param  pBuffer1:指向目标缓冲; pBuffer2:指向源缓冲; BufferLength:字节数
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
  * @brief  读取状态寄存器
  * @param  None
  * @retval 寄存器值
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
  * @brief  检查状态寄存器最高位是否为忙，并等待空闲
  * @param  None
  * @retval 忙返回0   非忙返回,1
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
	while((state_reg&0x80)==0&&retry<300)//重试300次
	{
		state_reg=SPI_FLASH_SendByte(0x00);	
		retry++; 		 
	}
	SPI_FLASH_CS_HIGH();
	if(retry>=200)return 0;
	else return 1;
}

/**
  * @brief  将指定主存储器页的数据转入指定缓冲区
  * @param  
  * @retval 忙返回0   非忙返回,1
  */
void DF_mm_to_buf(unsigned char buffer,unsigned int page)
{	  
	if(DF_wait_busy()==0)return;//错误
	SPI_FLASH_CS_LOW();
	if (buffer==1)SPI_FLASH_SendByte(MM_PAGE_TO_B1_XFER);	  
	else SPI_FLASH_SendByte(MM_PAGE_TO_B2_XFER);  
	SPI_FLASH_SendByte((unsigned char)(page >> 6));
  SPI_FLASH_SendByte((unsigned char)(page << 2));
  SPI_FLASH_SendByte(0x00);
	SPI_FLASH_CS_HIGH();	   
} 

/**
  * @brief  将指定缓冲区中的数据写入主存储区的指定页
  * @param  
  * @retval 忙返回0   非忙返回,1
  */
void DF_buf_to_mm(u8 buffer,u16 page)
{
	if(DF_wait_busy()==0)return;                              //错误
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
  * @brief  页擦除
  * @param  PageAddr:擦除的页地址
  * @retval None
  */
void SPI_FLASH_PageErase(u32 page)
{
  if(DF_wait_busy()==0)return;                             //错误  
	SPI_FLASH_CS_LOW();                                      //拉低片选
	SPI_FLASH_SendByte(PE);                                  //发送页擦除命令
	SPI_FLASH_SendByte((unsigned char)(page >> 6));
	SPI_FLASH_SendByte((unsigned char)(page << 2));
	SPI_FLASH_SendByte(0x00);
	SPI_FLASH_CS_HIGH();	                                    //拉高片选
}

/**
  * @brief  整片擦除
  * @param  None
  * @retval None
  */
void SPI_FLASH_BulkErase(void)
{
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(BE1);                                               //发送批量擦除指令
  SPI_FLASH_SendByte(BE2);
  SPI_FLASH_SendByte(BE3);
  SPI_FLASH_SendByte(BE4);

  SPI_FLASH_CS_HIGH();

  SPI_FLASH_WaitForWriteEnd();
}

/**
  * @brief  从指定地址开始写入指定个数的数据	
  * @param  
  * @retval None
  */
void SPI_Flash_Write(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u16 offaddr,i;
	u32 paddr;//页地址
	paddr=WriteAddr/SPI_FLASH_PageSize;         //得到开始写入的首页地址
	offaddr=WriteAddr%SPI_FLASH_PageSize;       //得到在首页地址里的偏移
	DF_mm_to_buf(1,paddr);                      //将开始页数据读出到buf1			  
	if(DF_wait_busy()==0)return;                //错误
	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(BUFFER_1_WRITE);         //写入1缓冲区命令	 		   
	SPI_FLASH_SendByte(0x00);                   //14bit 无效数据+10bit地址数据 
	SPI_FLASH_SendByte((u8)(offaddr>>8));       //写入在该页的偏移地址   
	SPI_FLASH_SendByte((u8)offaddr);
	for (i=0;i<NumByteToWrite;)                 //发送数据
	{
		SPI_FLASH_SendByte(*pBuffer);             //写入一个数据
		pBuffer++;
		i++;
		if((i+offaddr)%SPI_FLASH_PageSize==0)
		{
			SPI_FLASH_CS_HIGH();
			DF_buf_to_mm(1,paddr);                  //把BUF1的内容写入主存储器
			paddr++;	   
			if(paddr>=SPI_FLASH_PageNum)return;     //超出了AT45DB161的范围
			DF_mm_to_buf(1,paddr);                  //将开始页数据读出到buf1			  
			if(DF_wait_busy()==0)return;            //错误
			SPI_FLASH_CS_LOW();
			SPI_FLASH_SendByte(BUFFER_1_WRITE);     //写入1缓冲区命令	 	 		   
			SPI_FLASH_SendByte(0x00);               //14bit 无效数据+10bit地址数据 
			SPI_FLASH_SendByte(0x00);               //设置地址到0 
			SPI_FLASH_SendByte(0x00);		    
		}			   
	}
	SPI_FLASH_CS_HIGH();	
	DF_buf_to_mm(1,paddr);                       //把BUF1的内容写入主存储器			  
} 

/**
  * @brief  从指定地址开始读出指定个数的数据	
  * @param  
  * @retval None
  */
void SPI_Flash_Read(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	u16 offaddr,i;
	u32 paddr;                                //页地址
	paddr=ReadAddr/SPI_FLASH_PageSize;        //得到开始写入的首页地址
	offaddr=ReadAddr%SPI_FLASH_PageSize;      //得到在首页地址里的偏移
	DF_mm_to_buf(1,paddr);                    //将开始页数据读出到buf1			  
	if(DF_wait_busy()==0)return;              //错误
	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(BUFFER_1_READ);        //读1缓冲区命令	 		   
	SPI_FLASH_SendByte(0x00);                 //14bit 无效数据+10bit地址数据 
	SPI_FLASH_SendByte((u8)(offaddr>>8));     //写入在该页的偏移地址   
	SPI_FLASH_SendByte((u8)offaddr);
	SPI_FLASH_SendByte(0x00);                 //等待 
	for (i=0;i<NumByteToRead;)                //读取NumByteToRead个数据
	{
		*pBuffer=SPI_FLASH_SendByte(0xff);      //读取一个数据
		pBuffer++;
		i++;
		if((i+offaddr)%SPI_FLASH_PageSize==0)
		{
			SPI_FLASH_CS_HIGH();										  
			paddr++;	    
			if(paddr>=SPI_FLASH_PageNum)return;  //超出了AT45DB161的范围
			DF_mm_to_buf(1,paddr);               //将开始页数据读出到buf1			  
			if(DF_wait_busy()==0)return;         //错误
			SPI_FLASH_CS_LOW();
			SPI_FLASH_SendByte(BUFFER_1_READ);   //读1缓冲区命令	 		   
			SPI_FLASH_SendByte(0x00);            //14bit 无效数据+10bit地址数据 
			SPI_FLASH_SendByte(0x00);            //设置地址到0 
			SPI_FLASH_SendByte(0x00);
			SPI_FLASH_SendByte(0x00);            //等待		    
		}			   
	}
	SPI_FLASH_CS_HIGH();									  			  
} 

/**
  * @brief  填充数据
  * @param  pBuffer:指向填充缓冲;BufferLenght:数据长度;Offset:开始的数值
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
  * @brief  crc16检验
  * @param  data:指向待检验缓冲;length:校验的数据长度;start:检验的起始位置
  * @retval 校验值
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
  * @brief  flash测试.观察串口调试助手打印的数据
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
	Fill_Buffer(buf1, SPI_FLASH_PageSize , 0x00);                 //填充数据
 	printf("Fill_Buffer\r\n");
	for(offset=0;offset<SPI_FLASH_PageSize;offset++)
	{
	  printf("%d ",buf1[offset]);
	}
	crc1=check_crc16(buf1,sizeof(buf1),0);
	printf("\r\n原始数据crc16=0x%x\r\n",crc1);
#if ONLY_READ                                  //flash有一定的擦写寿命,勿反复在同一地址擦写
	for(i = 0;i<PageNum_TEST;i++)
  SPI_FLASH_PageErase(i);                                         //页擦除,spi flash写之前要对其擦除操作.
	
	for(i=0;i<PageNum_TEST;i++)
	{
		FLASH_WriteAddress=FLASH_WriteAddress+SPI_FLASH_PageSize*i;
		printf("\r\nwrite address=%d\r\n",FLASH_WriteAddress);
	  SPI_Flash_Write(buf1,FLASH_WriteAddress,SPI_FLASH_PageSize);    //写入满3页数据
	}
#endif
	FLASH_WriteAddress=0;
	for(i=0;i<PageNum_TEST;i++)                                              //读取3页数据
	{	
   		memset((u8*)buf1,0,sizeof(buf1));
			printf("\r\n");
		  FLASH_WriteAddress =FLASH_WriteAddress+SPI_FLASH_PageSize*i;
		  printf("\r\nread address=%d\r\n",FLASH_WriteAddress);
			SPI_Flash_Read(buf1,FLASH_WriteAddress,SPI_FLASH_PageSize);   
      pagenum=i+1;		
		  printf("\r\n------------------------------------------------------------------------------------\r\n");
			printf("读出第%d页数据\r\n",pagenum);
		  printf("page[%d]=",pagenum);
			for(offset=0;offset<SPI_FLASH_PageSize;offset++)
			{
				printf("%d ",buf1[offset]);
			}
			
			printf("\r\n开始检验第%d页数据\r\ncheck...",pagenum);
			crc2=check_crc16(buf1,sizeof(buf1),0);
			if(crc1==crc2)
			{
			  printf("\r\n第%d页检验正确\r\n",pagenum);
			}
			else
			{  
				printf("\r\n第%d页检验错误,crc16=0x%x\r\n",pagenum,crc2);
			}
			printf("\r\n-----------------------------------------------------------------------------------\r\n");
		}
}
