#include "LED.h"

// pa0--led1  pa1--led2
void LED_Init(void){

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef  gpioInitStructA;
	gpioInitStructA.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	gpioInitStructA.GPIO_Speed = GPIO_Speed_50MHz;
	gpioInitStructA.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &gpioInitStructA);
}

// led1  on
void LED1_ON(void){
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}
// led1  off
void LED1_OFF(void){
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}
// led2  on
void LED2_ON(void){
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}
// led2  off
void LED2_OFF(void){
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

