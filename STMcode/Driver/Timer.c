/**
  * @file    Timer.c 
  * @author  WB R&D Team - openmcu666
  * @version V1.0
  * @date    2016.05.06
  * @brief   Timer Driver
  */

#include "Timer.h"


/**
  * @brief  TIM2初始化配置
  * @param  None
  * @retval None
  */   							
void TIM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   //定义 TIM_TimeBase 初始化结构体 TIM_TimeBaseStructure 
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;           //定义 TIM_OCInit 初始化结构体 TIM_OCInitStructure 
 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;              //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;            //翻转频率50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 20000;                       //重装载值20000
	TIM_TimeBaseStructure.TIM_Prescaler = 0;                        //预分频0
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                    //时钟分割0
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	    //向上计数模式
	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseStructure);                //初始化时基单元

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //PWM1模式
	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	  //输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //输出极性为高

	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
		
	TIM_OC1PreloadConfig(TIM2 , TIM_OCPreload_Enable);	            //使能预装载寄存器 

	TIM_ARRPreloadConfig(TIM2, ENABLE);
	
	TIM_Cmd(TIM2 , ENABLE);	                                        //启动 TIM 计数 
}

void TIM5_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//定义 TIM_TimeBase 初始化结构体 TIM_TimeBaseStructure 
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;        //定义 TIM_OCInit 初始化结构体 TIM_OCInitStructure 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE );
	
	TIM_TimeBaseStructure.TIM_Period = 20000;                       //重装载值20000
	TIM_TimeBaseStructure.TIM_Prescaler = 0;                        //预分频0
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                    //时钟分割0
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	    //向上计数模式
	TIM_TimeBaseInit(TIM5 , &TIM_TimeBaseStructure);                //初始化时基单元

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //PWM1模式
	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	  //输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //输出极性为高

	TIM_OC2Init(TIM5, &TIM_OCInitStructure);
		
	TIM_OC2PreloadConfig(TIM5 , TIM_OCPreload_Enable);	            //使能预装载寄存器 

	TIM_ARRPreloadConfig(TIM5, ENABLE);
	
	TIM_Cmd(TIM5 , ENABLE);	                                        //启动 TIM 计数 
}
