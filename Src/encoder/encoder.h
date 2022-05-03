
#ifndef __ENCODER_H
#define __ENCODER_H

extern int encoderL;
extern int encoderR;

void encoder_TIM2_Init(void);
void encoder_TIM4_Init(void);
short read_encoder(int TIMx);

#endif //__ENCODER_H


