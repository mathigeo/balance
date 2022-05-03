#include "motor.h"

#define AIN2(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_12,(BitAction)x)
#define AIN1(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_13,(BitAction)x)

#define BIN1(x) GPIO_WriteBit(GPIOB,GPIO_Pin_14,(BitAction)x)
#define BIN2(x) GPIO_WriteBit(GPIOB,GPIO_Pin_15,(BitAction)x)

/**
 * @brief 电机驱动极性端初始化
 * @param None
 * @retval None
 */
void motor_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = 	GPIO_Pin_12|\
								GPIO_Pin_13|\
								GPIO_Pin_14|\
								GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}


/**
 * @brief 电机加载
 * @param left 左电机值
 * @param right 右电机值
 * @retval None
 */
void motor_load(int left,int right)
{
	if(left > 0)
	{
		BIN1(1);
		BIN2(0);		
	}
	else if(left < 0)
	{
		BIN1(0);
		BIN2(1);
		left = -left;
	}
	else
	{
		BIN1(0);
		BIN2(0);		
	}
	
	if(right > 0)
	{
		AIN1(1);
		AIN2(0);
	}
	else if(right < 0)
	{
		AIN1(0);
		AIN2(1);
		right = -right;
	}
	else
	{
		AIN1(0);
		AIN2(0);
	}
	
	TIM1->CCR1 = right;
	TIM1->CCR4 = left;
}










