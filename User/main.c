#include "system.h"  
#include "systick.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "i2c_oled.h"
#include "mpuExti.h"
#include "encoder.h"
#include "pwm.h"
#include "motor.h"


/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{		
	int i = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	OLED_Init();
	oledPrint(2,3,"LOADING...");
	for(i=0;i<100;i++)
	{
		loadbar(6,i);
		delay_ms(20);
	}
	OLED_Clear();
	encoder_TIM2_Init();
	encoder_TIM4_Init();
	MPU6050_Init();
	mpu_dmp_init();
	MPU6050_EXTI_Init();
	PWM_Init();
	motor_init();
	while(1)
	{
		oledPrint(1,1,"pitch:%4.1f",mpuSensor.Pitch);
		oledPrint(2,1,"gy:%6d",mpuSensor.gyroy);
		oledPrint(3,1,"R:%5d",encoderR);
		oledPrint(4,1,"L:%5d",encoderL);
	} 
}

