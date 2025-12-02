#include "delay.h"

static uint8_t fac_us = 0;  // 微秒延时倍乘数
static uint16_t fac_ms = 0; // 毫秒延时倍乘数

/**
 * @brief  初始化延时函数
 * @note   针对STM32F103Z8T6 72MHz主频，不分频配置
 * @param  无
 * @retval 无
 */
void Delay_Init(void) {
    // 配置SysTick定时器时钟源为HCLK（72MHz，不分频）
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    
    // 计算微秒延时的倍乘数
    // 72MHz时钟下，1us需要72个计数
    fac_us = 72;
    
    // 计算毫秒延时的倍乘数
    fac_ms = (uint16_t)fac_us * 1000;
}

/**
 * @brief  微秒级延时
 * @note   最大延时约为：0xFFFFFF / 72 = 1864135us ≈ 1.86s
 * @param  us: 延时微秒数
 * @retval 无
 */
void Delay_us(uint32_t us) {
    uint32_t temp;
    
    // 设置SysTick重载值，最大不能超过0xFFFFFF
    SysTick->LOAD = us * fac_us;
    
    // 清空计数器
    SysTick->VAL = 0x00;
    
    // 启动计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    
    // 等待计数结束
    do {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));
    
    // 关闭计数器
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    
    // 清空计数器
    SysTick->VAL = 0x00;
}

/**
 * @brief  毫秒级延时
 * @param  ms: 延时毫秒数
 * @retval 无
 */
void Delay_ms(uint32_t ms) {
    uint32_t i;
    for (i = 0; i < ms; i++) {
        Delay_us(1000);
    }
}


