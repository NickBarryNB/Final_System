#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Fan.h"
#include "Led.h"

int main(void){
	
	
	// 系统时钟初始化
    SystemInit();
    
    // PWM初始化
    pwmInit();
    
    
        // 正转，50%转速
        forward();
        setFanSpeed(50);
        delayByMs(3000);
        
        // 增加到80%转速
        setFanSpeed(80);
        delayByMs(3000);
        
        // 反转，30%转速
        backward();
        setFanSpeed(30);
        delayByMs(3000);
        
        // 关闭风扇
        closeFan();
        delayByMs(2000);
        
        // 重新开启（会保持之前的设置）
        openFan();
        delayByMs(3000);

	
	
	
		
	//led测试代码
	
		ledInit();
	 // 测试LED亮度控制
    setLedBrightness(30);    // LED 30%亮度
    delayByMs(1000);
    
    setLedBrightness(80);    // LED 80%亮度  
    delayByMs(1000);
    
    ledClose();              // 关闭LED
    delayByMs(1000);
    
		
		
		//红外测试代码
		
   
}	
	