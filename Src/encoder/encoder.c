#include "stm32f10x.h"   // Device header
#include "encoder.h"


int encoderL=0;
int encoderR=0;

/**
 * @brief 编码器时基配置(定时器2)
 * @param None
 * @retval None
 */
void encoder_TIM2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	
	/*-配置时钟-*/
	//使能定时器2的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//使能PA端口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	
	
	/*-端口配置-*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;   
	//浮空输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//根据设定参数初始化GPIOA
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	
	
	
	/*-配置时基-*/
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	// 预分频器
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0000; 
	//设定计数器自动重装值
    TIM_TimeBaseStructure.TIM_Period = 0XFFFF; 
	//选择时钟分频：不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//TIM向上计数
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//时基结构体初始化
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	
	
	/*-编码器模式和输入捕获配置-*/
	//使用编码器模式3
    TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 10;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);	
	
	
	
	/*-开启准备-*/
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_SetCounter(TIM2,0);
    TIM_Cmd(TIM2, ENABLE);
}



/**
 * @brief 编码器时基配置(定时器4)
 * @param None
 * @retval None
 */
void encoder_TIM4_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	
	/*-配置时钟-*/
	//使能定时器4的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	//使能PA端口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	
	
	/*-端口配置-*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;   
	//浮空输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//根据设定参数初始化GPIOA
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	
	
	
	/*-配置时基-*/
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	// 预分频器
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0000; 
	//设定计数器自动重装值
    TIM_TimeBaseStructure.TIM_Period = 0XFFFF; 
	//选择时钟分频：不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//TIM向上计数
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//时基结构体初始化
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	
	
	/*-编码器模式和输入捕获配置-*/
	//使用编码器模式3
    TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 10;
    TIM_ICInit(TIM4, &TIM_ICInitStructure);	
	
	
	
	/*-开启准备-*/
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    TIM_SetCounter(TIM4,0);
    TIM_Cmd(TIM4, ENABLE);
}



/**
 * @brief 读取计数值(2为右，4为左)
 * @param None
 * @retval 计数值
 */
short read_encoder(int TIMx)
{
    short Encoder_TIM;
	switch(TIMx)
	{
		case 2:
			Encoder_TIM = (short)TIM_GetCounter(TIM2);
			TIM_SetCounter(TIM2,0);
			break;
		case 4:
			Encoder_TIM = -(short)TIM_GetCounter(TIM4);
			TIM_SetCounter(TIM4,0);
			break;
		default:
			Encoder_TIM = 0;
	}
    return Encoder_TIM;
}


/**
 * @brief 定时器2中断服务函数
 * @param None
 * @retval None
 */
void TIM2_IRQHandler(void)
{
	//溢出中断
    if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=0)
    {
    }
	//清除中断标志位
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}


/**
 * @brief 定时器4中断服务函数
 * @param None
 * @retval None
 */
void TIM4_IRQHandler(void)
{
	//溢出中断
    if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=0)
    {
    }
	//清除中断标志位
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}




