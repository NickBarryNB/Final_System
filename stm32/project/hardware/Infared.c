#include "infared.h"


//直接在主函数弄个变量读取状态就行

//A9
void initInfared(void){

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef gpio_Struct;
	gpio_Struct.GPIO_Mode = GPIO_Mode_IPU;	// 输出模式	
	gpio_Struct.GPIO_Pin = GPIO_Pin_5;
	gpio_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_Struct);
	
}







