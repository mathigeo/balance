#ifndef __I2C_OLED_H
#define __I2C_OLED_H

#include "stm32f10x.h"


#define OLED_W_SCL(x)		GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)(x))
#define OLED_W_SDA(x)		GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)(x))


void OLED_Init(void);
void OLED_Clear(void);
void OLED_SetCursor(uint8_t Y, uint8_t X);
void OLED_WriteCommand(uint8_t Command);
void OLED_ShowChar(uint8_t row,uint8_t col,char Chr,uint8_t inv);
void OLED_ShowString(uint8_t row,uint8_t col,char *str,uint8_t inv);
int oledPrint(u8 row,u8 col,const char *formate,...);
int loadbar(u8 page,u8 percent);

#endif /*__I2C_OLED_H*/


