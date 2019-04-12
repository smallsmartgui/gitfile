#ifndef __RTC_H
#define __RTC_H

#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "Gpio.h"
#include "USART.h"

void RTC_Configuration(void);
u8 USART_Scanf(u32 value);
u32 Time_Regulate(void);
void Time_Adjust(void);
void Time_Display(u32 TimeVar);
void RTC_Init(void);
#endif
