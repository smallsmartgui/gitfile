/**
  * @file    Gpio.c 
  * @author  WB R&D Team - openmcu666
  * @version V1.0
  * @date    2016.05.04
  * @brief   systick
  */
#include "delay.h"

static	uint8_t  sysclk = 72;                             //Ĭ��ϵͳʱ��Ϊ72MHz
static  uint8_t  fac_us=0;                                //us��ʱ������
static  uint16_t fac_ms=0;                                //ms��ʱ������ 

/**
  * @brief  ��׼��ʱ��ʼ����ʹ��SysTick;
  *         SysTickʱ��Դ��HCLK/8�ṩ����ϵͳƵ��Ϊ72MHzʱ
  *         ��С��������Ϊ1/9MHz,����9��Ϊ1us��fac_us��9Ϊ��λ        
  * @param  None
  * @retval None
  */
void Delay_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);     //ѡ��ϵͳ��ʱ����ʱ��Դ
	fac_us = sysclk/8;		    
	fac_ms=(u16)fac_us*1000;
}

/**
  * @brief  ���뼶��ʱ��SysTick->LOADΪ24λ�Ĵ���,
  *         ���������ʱС�ڻ����1864ms
  * @param  nms(ms number) ������ ���ܳ���1864
  * @retval None
  */
void Delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;                             //ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;                                        //��ռ����� 
	SysTick->CTRL=0x01 ;                                       //��ʼ����   
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));                          //�ȴ�ʱ�䵽��  
	SysTick->CTRL=0x00;                                         //�رռ�����	 
	SysTick->VAL =0x00;                                         //��ռ�����	 	    
} 

/**
  * @brief  ʵ��΢�뼶��ʱ�������ʱ1864ms
  *         ������������ͨ���Ĵ������õģ�û�ж�Ӧ�Ĺ̼��⺯�����ο�misc.h
  * @param  us(number us) ��ʱ΢���� ���1864135
  * @retval None
  */
void Delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us;                             //ʱ����� fac_us=9,��9�ξ���1us	 		 
	SysTick->VAL=0x00;                                    //��ռ����� 
	SysTick->CTRL=0x01 ;                                  //��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));                     //�ȴ�ʱ�䵽��  
	SysTick->CTRL=0x00;                                    //�رռ�����
	SysTick->VAL =0x00;                                    //��ռ�����	
}

/**
  * @brief  ��ʱ
  * @param  Time:0-65535
  * @retval None
  */
void Delay(uint32_t Time)
{
  uint32_t i;
    
  i = 0;
  while (Time--) 
	{
    for (i = 0; i < 8000; i++);
  }
}
