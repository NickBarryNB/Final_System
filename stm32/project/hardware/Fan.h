#ifndef __FAN_H
#define __FAN_H

#include "stm32f10x.h"

// 函数声明
void pwmInit(void);
void openFan(void);
void closeFan(void);
//正转
void forward(void);
//反转
void backward(void); 
// 0-100控制转速
void setFanSpeed(uint8_t speed); 

#endif