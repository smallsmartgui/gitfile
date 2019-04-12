/**
  * @file    Timer.c 
  * @author  WB R&D Team - openmcu666
  * @version V1.0
  * @date    2016.05.06
  * @brief   Timer Driver
  */

#include "Timer.h"


/**
  * @brief  TIM2��ʼ������
  * @param  None
  * @retval None
  */   							
void TIM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   //���� TIM_TimeBase ��ʼ���ṹ�� TIM_TimeBaseStructure 
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;           //���� TIM_OCInit ��ʼ���ṹ�� TIM_OCInitStructure 
 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;              //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;            //��תƵ��50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 20000;                       //��װ��ֵ20000
	TIM_TimeBaseStructure.TIM_Prescaler = 0;                        //Ԥ��Ƶ0
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                    //ʱ�ӷָ�0
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	    //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseStructure);                //��ʼ��ʱ����Ԫ

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //PWM1ģʽ
	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	  //���ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //�������Ϊ��

	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
		
	TIM_OC1PreloadConfig(TIM2 , TIM_OCPreload_Enable);	            //ʹ��Ԥװ�ؼĴ��� 

	TIM_ARRPreloadConfig(TIM2, ENABLE);
	
	TIM_Cmd(TIM2 , ENABLE);	                                        //���� TIM ���� 
}

void TIM5_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//���� TIM_TimeBase ��ʼ���ṹ�� TIM_TimeBaseStructure 
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;        //���� TIM_OCInit ��ʼ���ṹ�� TIM_OCInitStructure 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE );
	
	TIM_TimeBaseStructure.TIM_Period = 20000;                       //��װ��ֵ20000
	TIM_TimeBaseStructure.TIM_Prescaler = 0;                        //Ԥ��Ƶ0
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                    //ʱ�ӷָ�0
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	    //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5 , &TIM_TimeBaseStructure);                //��ʼ��ʱ����Ԫ

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //PWM1ģʽ
	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	  //���ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //�������Ϊ��

	TIM_OC2Init(TIM5, &TIM_OCInitStructure);
		
	TIM_OC2PreloadConfig(TIM5 , TIM_OCPreload_Enable);	            //ʹ��Ԥװ�ؼĴ��� 

	TIM_ARRPreloadConfig(TIM5, ENABLE);
	
	TIM_Cmd(TIM5 , ENABLE);	                                        //���� TIM ���� 
}
