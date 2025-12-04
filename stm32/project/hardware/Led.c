#include "stm32f10x.h"    
#include "stm32f10x_tim.h"

//A3

void ledInit(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 1. 开启GPIOA和TIM2的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 2. 配置GPIOA Pin3 为复用推挽输出 (AFIO)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 重要：改为复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. 配置定时器TIM2的基本参数
    // 假设系统时钟为72MHz，预分频设为72-1，则计数器时钟为1MHz
    TIM_TimeBaseStructure.TIM_Period = 100 - 1;        // 自动重装载值(ARR)，决定PWM频率
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;       // 预分频器(PSC)
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 4. 配置TIM2的通道4 (因为GPIOA_Pin3对应TIM2_CH4)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;    // PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 使能输出
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   // 输出极性高
    TIM_OCInitStructure.TIM_Pulse = 0; // 初始占空比为0，LED暗
    TIM_OC4Init(TIM2, &TIM_OCInitStructure); // 初始化通道4

    // 5. 使能TIM2的预装载寄存器（ARR）
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM2, ENABLE);

    // 6. 启动TIM2计数器
    TIM_Cmd(TIM2, ENABLE);
}


void setLedBrightness(uint8_t brightness) {
    uint16_t pulse;
    
    // 确保亮度值在0-100范围内
    if(brightness > 100) {
        brightness = 100;
    }
    
    // 将百分比亮度转换为PWM比较值
    // 因为ARR=99，所以占空比 = (pulse / (99+1)) * 100%
    pulse = (brightness * 100) / 100; // 简化计算：pulse = brightness
    
    // 设置TIM2通道4的比较值（CCR4）
    TIM_SetCompare4(TIM2, pulse);
}



void ledOpen(void) {
    setLedBrightness(100); // 全亮
}

void ledClose(void) {
    setLedBrightness(0);   // 全暗
}
