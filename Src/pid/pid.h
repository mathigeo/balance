#ifndef __PID_H
#define __PID_H

/**
 * @brief 位置式PID结构体
*/
typedef struct
{
    float kp;               //比例系数
    float ki;               //积分系数
    float kd;               //微分系数    
    float lastErr;          //上次误差
    float integra;         //误差积分
}Pid_t;


/**
 * @brief 增量式PID结构体
*/
typedef struct
{
    float kp;               //比例系数
    float ki;               //积分系数
    float kd;               //微分系数    
    float lastErr;          //上次误差
	float prevErr;			//上上次误差
	float realopt;			//上次输出
}IncPid_t;


/*外部方法接口*/
float pid_ctrl(Pid_t *pid,float target,float value);
float incPid_ctrl(IncPid_t *self,float target,float value);

#endif //__PID_H



