#include "mpuExti.h"
#include "control.h"
#include "encoder.h"

/*中断读写变量*/
MPU6050_T mpuSensor={0};

/**
 * @brief 外部中断配置
 * @param None
 * @retval None
 */
void MPU6050_EXTI_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);//开启时钟
	
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);	
	
	
	/*外部中断*/
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStruct);
	
	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
	EXTI_InitStruct.EXTI_Line=EXTI_Line5;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
}

/**
 * @brief MPU6050外部中断服务函数
 * @param None
 * @retval None
 */
void EXTI9_5_IRQHandler(void)
{
	/*一级判定*/
	if(EXTI_GetITStatus(EXTI_Line5)!=0)
	{
		/*二级判定*/
		if(PBin(5)==0)
		{
			encoderL = read_encoder(4);
			encoderR = read_encoder(2);
			
			/*角度*/
			mpu_dmp_get_data(&mpuSensor.Pitch,&mpuSensor.Roll,&mpuSensor.Yaw);	
			/*陀螺仪*/		
			MPU6050_Get_Gyroscope(&mpuSensor.gyrox,&mpuSensor.gyroy,&mpuSensor.gyroz);	

			controller();
			
			/*清除中断标志位*/
			EXTI_ClearITPendingBit(EXTI_Line5);
		}
	}
}





