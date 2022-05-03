#include "pwm.h"





/**
 * @brief 通用定时器 PWM 输出用到的 GPIO 和 PWM 模式初始化
 * @param 无
 * @retval 无
 */
void PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	
	
	/*-配置时钟-*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);	
	
	
	
	/*-配置GPIO-*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	
	/*-定时器 PWM 时基初始化-*/
	// 自动重装载寄存器的值
	TIM_TimeBaseStructure.TIM_Period=7199;
	// 驱动 CNT 计数器的时钟
	TIM_TimeBaseStructure.TIM_Prescaler= 0;
	// 时钟分频因子
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	// 计数器计数模式
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	// 重复计数器的值
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	// 初始化定时器
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	
	/*-PWM输出比较初始化-*/
	// 配置为 PWM 模式 1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//使能输出比较N状态
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	//当 MOE=0 重置 TIM1 输出比较空闲状态
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	//当 MOE=0 重置 TIM1 输出比较 N 空闲状态
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	// 输出通道电平极性配置
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	//Capture Compare Register
	TIM_OCInitStructure.TIM_Pulse = 0;
	// 输出比较通道 1
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1,ENABLE);
	
	//MOE 主输出使能	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);		
	
	// 使能计数器
	TIM_Cmd(TIM1, ENABLE);
}


