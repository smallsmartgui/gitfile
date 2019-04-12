/**
  * @file    wdg.c 
  * @author  WB R&D Team - openmcu666
  * @version V1.0
  * @date    2016.05.04
  * @brief   Watchdog Driver
  */
#include "wdg.h"
 
/**
  * @brief  �������Ź���ʼ��
  * @param  None
  * @retval None
  */
void IWDG_Init(void)
{	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);	
 	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //ʹ�ܶԼĴ���IWDG_PR��IWDG_RLR��д����
	
	IWDG_SetPrescaler(IWDG_Prescaler_128);         //LSI��128��Ƶ	
	IWDG_SetReload(399);                           //����IWDG����ֵΪ399
	IWDG_ReloadCounter();                          //����IWDG������	
	IWDG_Enable();                                 //����IWDG
}

/**
  * @brief  ι��
  * @param  None
  * @retval None
  */
void IWDG_Feed(void)
{   
 	IWDG_ReloadCounter();                              //ι��									   
}

/**
  * @brief  ���ô��ڲ���
  * @param  None
  * @retval None
  */
void WWDG_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);	
   //����WWDGԤ��ƵֵΪ8��WWDGʱ��Ƶ�� = (PCLK1/4096)/8 = 244 Hz(~4ms)
   WWDG_SetPrescaler(WWDG_Prescaler_8);
   //����WWDG��ʼ����ֵΪ0x7F������WWDG����ʱWWDG	��ʱʱ��Ϊ
   //4ms * ��0x7F - 0x3F�� = 264ms 											 
   WWDG_Enable(0x7F);
    
   WWDG_ClearFlag();                 //��� WWDG ���ڻ����ж�(EWI)��־
    
   WWDG_EnableIT();	                 //ʹ�� WWDG ���ڻ����ж�(EWI)
}

/**
  * @brief  ι��
  * @param  Value��ι��ֵ
  * @retval None
  */
void Wwdg_Feed(u8 Value)
{	
  WWDG_SetCounter(Value);              // ι�� 
}

