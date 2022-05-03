#ifndef _MPUEXTI_H
#define _MPUEXTI_H

#include "system.h" 
#include "mpu6050.h"
#include "inv_mpu.h"

/**
 * @brief 
 * @param 
 * @retval 
 */
typedef struct 
{
	float Pitch; 	//俯仰角
	float Roll;		//横滚角
	float Yaw;		//偏航角
	short gyrox; 	//x角速度
	short gyroy;	//y角速度
	short gyroz;	//z角速度
	short aacx;		//x角加速度
	short aacy;		//y角加速度
	short aacz;		//z角加速度
}MPU6050_T;


extern MPU6050_T mpuSensor;

void MPU6050_EXTI_Init(void);


#endif //_MPUEXTI_H







