#ifndef __USART_GENERAL_H
#define __USART_GENERAL_H

#include "stm32f10x.h"
#include <stdio.h>
//#include <stlib.h>
#include <string.h>
#include "LED.h"
extern char receiveArr[200];
extern uint8_t receiveFlag;

// 初始化串口
void usart1_Init(void);
// 初始化串口接收DMA
void usart1_DMA_Init(void);
// 发送一个字节
void usart1_send_Byte(uint8_t send_Byte);
// 发送一个数组
void usart1_send_Arr(uint8_t *arr, uint16_t length);
// 发送一个字符串
void usart1_send_String(char *String);
#endif /* __USART_GENERAL_H */

