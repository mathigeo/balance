#include "SysTick.h"
#include "core_cm3.h"
#include "misc.h"


/**
 * @brief 系统定时器延时us
 * @param us 毫秒数
 * @retval None
 * @note couter减1的时间 等于 1/systick_clk
 * 	当counter 从 reload 的值减小到0的时候，为一个循环，如果开启了中断则执行中断服务程序，
 * 	同时 CTRL 的 countflag 位会置1
 * 	这一个循环的时间为 reload * (1/systick_clk)
 */
void SysTick_Delay_Us( __IO uint32_t us)
{
	uint32_t i;
	SysTick_Config(SystemCoreClock/1000000);
	
	for(i=0;i<us;i++)
	{
		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1	
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	// 关闭SysTick定时器
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
}


/**
 * @brief 系统定时器延时ms
 * @param ms 延时的毫秒数
 * @retval None
 */
void SysTick_Delay_Ms( __IO uint32_t ms)
{
	uint32_t i;	
	SysTick_Config(SystemCoreClock/1000);
	
	for(i=0;i<ms;i++)
	{
		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1
		// 当置1时，读取该位会清0
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	// 关闭SysTick定时器
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}


