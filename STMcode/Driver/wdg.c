/**
  * @file    wdg.c 
  * @author  WB R&D Team - openmcu666
  * @version V1.0
  * @date    2016.05.04
  * @brief   Watchdog Driver
  */
#include "wdg.h"
 
/**
  * @brief  独立看门狗初始化
  * @param  None
  * @retval None
  */
void IWDG_Init(void)
{	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);	
 	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //使能对寄存器IWDG_PR和IWDG_RLR的写操作
	
	IWDG_SetPrescaler(IWDG_Prescaler_128);         //LSI的128分频	
	IWDG_SetReload(399);                           //设置IWDG计数值为399
	IWDG_ReloadCounter();                          //重载IWDG计数器	
	IWDG_Enable();                                 //启动IWDG
}

/**
  * @brief  喂狗
  * @param  None
  * @retval None
  */
void IWDG_Feed(void)
{   
 	IWDG_ReloadCounter();                              //喂狗									   
}

/**
  * @brief  设置窗口参数
  * @param  None
  * @retval None
  */
void WWDG_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);	
   //设置WWDG预分频值为8，WWDG时钟频率 = (PCLK1/4096)/8 = 244 Hz(~4ms)
   WWDG_SetPrescaler(WWDG_Prescaler_8);
   //设置WWDG初始计数值为0x7F并启动WWDG，此时WWDG	超时时间为
   //4ms * （0x7F - 0x3F） = 264ms 											 
   WWDG_Enable(0x7F);
    
   WWDG_ClearFlag();                 //清除 WWDG 早期唤醒中断(EWI)标志
    
   WWDG_EnableIT();	                 //使能 WWDG 早期唤醒中断(EWI)
}

/**
  * @brief  喂狗
  * @param  Value：喂狗值
  * @retval None
  */
void Wwdg_Feed(u8 Value)
{	
  WWDG_SetCounter(Value);              // 喂狗 
}

