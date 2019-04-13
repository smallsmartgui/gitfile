#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#include "stm32f10x.h"

#define ONLY_READ                            1   //ֻ��ģʽ
#define PageNum_TEST                         3   //����Ҫ���Ե�ҳ��
#define GPIO_CS                              GPIOA
#define RCC_APB2Periph_GPIO_CS               RCC_APB2Periph_GPIOA
#define GPIO_Pin_CS                          GPIO_Pin_4 


#define SPI_FLASH_CS_LOW()                   GPIO_ResetBits(GPIO_CS, GPIO_Pin_CS)  //CS�������� �ߵ͵��ƽ
#define SPI_FLASH_CS_HIGH()                  GPIO_SetBits(GPIO_CS, GPIO_Pin_CS)

#define SPI_FLASH_PageSize                   0x210
#define SPI_FLASH_PageNum                    4096

#define WRITE                                0x82  //д������ָ��
#define READ                                 0xD3  //��ȡ����ָ��
#define RDSR                                 0xD7  //���Ĵ���״̬
#define RDID                                 0x9F  //Read identification 
#define BUSY_Flag                            0x80  //æ��־
#define PE                                   0x81  //ҳ����
#define B1_TO_MM_PAGE_PROG_WITH_ERASE        0x83	 // ����һ������������д�����洢��������ģʽ��
#define B2_TO_MM_PAGE_PROG_WITH_ERASE        0x86	 // ���ڶ�������������д�����洢��������ģʽ��
#define BE1                                  0xC7  // Bulk Erase instruction 
#define BE2                                  0x94  // Bulk Erase instruction 
#define BE3                                  0x80  // Bulk Erase instruction 
#define BE4                                  0x9A  // Bulk Erase instruction 

#define BUFFER_1_WRITE                       0x84	 // д���һ������
#define BUFFER_2_WRITE                       0x87	 // д��ڶ�������
#define BUFFER_1_READ                        0xD4	 // ��ȡ��һ������
#define BUFFER_2_READ                        0xD6  // ��ȡ�ڶ�������
#define MM_PAGE_TO_B1_XFER                   0x53	 // �����洢����ָ��ҳ���ݼ��ص���һ������
#define MM_PAGE_TO_B2_XFER                   0x55	 // �����洢����ָ��ҳ���ݼ��ص��ڶ�������

#define Dummy_Byte                           0xA5  // �������� 

#define  AT45DB161D_FLASH_ID    0x1F260000
#define  BufferSize             (countof(Tx_Buffer)-1)
#define  countof(a)             (sizeof(a) / sizeof(*(a)))

void SPI_FLASH_Init(void);
void SPI_FLASH_PageErase(u32 SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_Flash_Write(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_Flash_Read(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32  SPI_FLASH_ReadID(void);

u8 SPI_FLASH_ReadByte(void);
u8 SPI_FLASH_SendByte(u8 byte);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WaitForWriteEnd(void);
void SPI_FLASH_Test(void);
#endif 
