#include "usart_general.h"

char receiveArr[200];
uint8_t receiveFlag = 0;

// 初始化串口接收DMA
void usart1_DMA_Init(void){
	// RCC
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	// dma
	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)receiveArr;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;// DMA_Mode_Circular
	DMA_InitStruct.DMA_BufferSize = sizeof(receiveArr);// 缓冲区大小  --   头文件中定义为200个字节
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA1_Channel5,&DMA_InitStruct);
	
	// dma cmd
	DMA_Cmd(DMA1_Channel5,ENABLE);	
}
// 初始化串口
// USART1_Tx = PA9
// USART1_Rx = PA10
void usart1_Init(void){
	// rcc
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// gpio
	GPIO_InitTypeDef GPIO_InitStrcut;
	// PA9
	GPIO_InitStrcut.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStrcut.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStrcut.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStrcut);
	// PA10
	GPIO_InitStrcut.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStrcut.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStrcut.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStrcut);
	
	// usart struct
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);
	
	// 开DMA接收数据
	usart1_DMA_Init();
	// usart dma cmd
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	
	// usart nvic
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	// usart IT set
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  // 接收不空中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);  // 空闲中断
	
	// cmd
	USART_Cmd(USART1, ENABLE);
}

// 发送一个字节
void usart1_send_Byte(uint8_t send_Byte){
	USART_SendData(USART1,send_Byte);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

// 发送一个数组
void usart1_send_Arr(uint8_t *arr, uint16_t length){
	uint16_t len;
	for(len = 0; len < length; len++){
		usart1_send_Byte(*arr+len);
	}
}

// 发送一个字符串
void usart1_send_String(char *String){
	while(*String){
		usart1_send_Byte(*String++);
	}
}

// fputc   重定向   使用pringf
int fputc(int ch, FILE *f){
	usart1_send_Byte(ch);
	return ch;
}


// usart1  中断服务函数
void USART1_IRQHandler(void){
	
//	static uint16_t index = 0;
	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET){
//		// 每次接收一个字节的data  @ L E D \r  \n
//		uint8_t data = USART_ReceiveData(USART1);
//		
//		receiveArr[index++] = data;
//		
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);// RXNE  可以不用清除
//		
//	}else 
	if(USART_GetITStatus(USART1,  USART_IT_IDLE) == SET){  // 空闲中断
//		receiveArr[index] = '\0';
//		index = 0;
		
		receiveArr[sizeof(receiveArr) - DMA_GetCurrDataCounter(DMA1_Channel5)] = '\0';
		receiveFlag = 1;
		
		// 重新设置DMA计数器的值
		DMA_Cmd(DMA1_Channel5,DISABLE);	
		// This function can only be used when the DMAy_Channelx is disabled
		DMA_SetCurrDataCounter(DMA1_Channel5, sizeof(receiveArr));
		DMA_Cmd(DMA1_Channel5,ENABLE);
		
		// 默认清除
		USART_ClearITPendingBit(USART1,USART_IT_IDLE);
		
		// 操作手册规定   软件序列清除该位(先读USART_SR，然后读USART_DR)。
		uint32_t temp;
		temp = USART1->SR;
		temp = USART1->DR;
		(void)temp;
	}
}
