#ifndef __USART_H
#define __USART_H

#include <stm32f10x.h>
#include "stdio.h"

#define DATA_BUF_SIZE							64
extern u8 USART1_TX_Finish;
extern u8 USART2_TX_Finish;
extern uint8_t USART1_SEND_DATA[];		  
extern uint8_t USART1_RECEIVE_DATA[];		

extern uint8_t USART2_SEND_DATA[];		   
extern uint8_t USART2_RECEIVE_DATA[];		 

void USART1_Init(void);
void USART1_SendByte(u16 dat);
void USART1_SendString(u8 *buf,uint16_t len);

void USART2_Init(void);
void USART2_SendByte(u16 dat);
void USART2_SendString(uint8_t *ch);
void SendMessage(void);
void USART_DMAConfiguration(void);
#endif
