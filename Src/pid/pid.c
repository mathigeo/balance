#include "pid.h"



/**
 * @brief 位置式PID控制器
 * @param pid PID指针
 * @param value 当前的实际值
 * @retval 计算的结果
*/
float pid_ctrl(Pid_t *self,float target,float value)
{
    float ret;
    float err;

    err = target - value;

    ret = 	(self->kp)*(err) +\
			(self->ki)*(self->integra) +\
			(self->kd)*(err - (self->lastErr));

    self->integra += err;
    self->lastErr = err;

    return ret;
}



/**
 * @brief 增量式PID控制器
 * @param pid PID指针
 * @param value 当前的实际值
 * @retval 计算的结果
*/
float incPid_ctrl(IncPid_t *self,float target,float value)
{
    float temp;
    float err;

    err = target - value;

	temp = 	(self->kp) * (err - (self->lastErr)) + \
			(self->ki) * (err) + \
			(self->kd) * (err - 2*(self->lastErr) + (self->prevErr));

    self->prevErr = self->lastErr;
    self->lastErr = err;

	self->realopt += temp;

    return self->realopt;    
}



