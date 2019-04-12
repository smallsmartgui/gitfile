#ifndef _WWDG_H_
#define _WWDG_H_

#include "stm32f10x.h"
#include <stdio.h>		
#include "Gpio.h"

void IWDG_Init(void);
void IWDG_Feed(void);
void WWDG_Init(void);
void Wwdg_Feed(u8 Value);

#endif
