/*

*/
#include "stm32f10x.h"
#include "usart_general.h"
#include "OLED.h"
#include <stdio.h>
#include "LED.h"
int main(void){
	usart1_Init();
	OLED_Init();
	LED_Init();
	// 杨鹏成测试
	while(1){
		if(receiveFlag == 1){
//			printf("get: %s\n",receiveArr);
//			printf("sizeof-get: %d\n",sizeof(receiveArr));
			if(strcmp(receiveArr,"LED1ON") == 0){
				LED1_ON();
				printf("get:LED1ON\n");
			}else if(strcmp(receiveArr,"LED1OFF") == 0){
				LED1_OFF();
				printf("get:LED1OFF\n");
			}
//			// ��ջ�����
//			
//			memset(receiveArr,0,sizeof(receiveArr));
//			
			receiveFlag = 0;	
		}
	}
}
