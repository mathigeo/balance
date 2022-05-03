#include "control.h"
#include "mpuExti.h"
#include "motor.h"
#include "encoder.h"
#include "pid.h"


float vertical(float target,float value);



/**
 * @brief 控制器
 * @param None
 * @retval None
 */
void controller(void)
{
	//速度环
	static Pid_t s = {0.25,0.25/(float)200,0,0,0};
	static Pid_t t = {-0.8,0,0,0,0};
	
	int pwmL,pwmR,pwm;
	int tn;
	
	pwm = vertical(0+pid_ctrl(&s,0,encoderR+encoderL),mpuSensor.Pitch);
	tn = pid_ctrl(&t,0,mpuSensor.gyroz);
	pwmL = pwm+tn;
	pwmR = pwm-tn;
	if((mpuSensor.Pitch <45 && mpuSensor.Pitch>-45))
		motor_load(pwmL,pwmR);
	else
		motor_load(0,0);
}




float vertical(float target,float value)
{
	static float kp=-360,kd=1.9;
	float err;
	float ret;
	err = target - value;
	ret = kp*err + kd*mpuSensor.gyroy;
	return ret;
}



