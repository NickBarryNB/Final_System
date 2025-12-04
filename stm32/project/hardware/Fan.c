#include "fan.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"

// 全局变量保存当前状态
static uint8_t current_speed = 0;
static uint8_t current_direction = 0; // 0:停止, 1:正转, 2:反转

void pwmInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE);
    
    // 配置PA8为复用推挽输出 (PWM输出)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 配置PA9为推挽输出 (方向控制)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 初始化方向引脚为低电平
    GPIO_ResetBits(GPIOA, GPIO_Pin_9);
    
    // 定时器时基配置
    // PWM频率 = 72MHz / (719 + 1) / (999 + 1) = 100Hz
    TIM_TimeBaseStructure.TIM_Period = 999; // ARR值
    TIM_TimeBaseStructure.TIM_Prescaler = 719; // PSC值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    // PWM模式配置
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_Pulse = 0; // 初始占空比为0
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    
    // 使能定时器
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE); // 高级定时器需要这个
    
    current_speed = 0;
    current_direction = 0;
}

void openFan(void)
{
    // 如果当前有速度设置，恢复之前的转速和方向
    if(current_speed > 0) {
        setFanSpeed(current_speed);
    } else {
        // 默认以50%转速正转启动
        setFanSpeed(50);
        forward();
    }
}

void closeFan(void)
{
    // 停止PWM输出，但保持方向设置
    TIM_SetCompare1(TIM1, 0);
    current_speed = 0;
}

void forward(void)
{
    // 设置方向引脚为低电平（正转）
    GPIO_ResetBits(GPIOA, GPIO_Pin_9);
    current_direction = 1;
    
    // 如果风扇是开启状态，恢复之前的转速
    if(current_speed > 0) {
        setFanSpeed(current_speed);
    }
}

void backward(void)
{
    // 设置方向引脚为高电平（反转）
    GPIO_SetBits(GPIOA, GPIO_Pin_9);
    current_direction = 2;
    
    // 如果风扇是开启状态，恢复之前的转速
    if(current_speed > 0) {
        setFanSpeed(current_speed);
    }
}

void setFanSpeed(uint8_t speed)
{
    // 限制速度在0-100范围内
    if(speed > 100) {
        speed = 100;
    }
    
    // 计算PWM比较值
    uint16_t compare_value = (uint16_t)((speed * 999) / 100);
    TIM_SetCompare1(TIM1, compare_value);
    
    current_speed = speed;
}