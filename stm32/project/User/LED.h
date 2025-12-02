#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"
// pa0--led1  pa1--led2
void LED_Init(void);
// led1  on
void LED1_ON(void);
// led1  off
void LED1_OFF(void);
// led2  on
void LED2_ON(void);
// led2  off
void LED2_OFF(void);

#endif /* __LED_H */

