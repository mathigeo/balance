#ifndef __SPI_OLED_H
#define __SPI_OLED_H

#include "stm32f10x.h"

#define OLED_W_D0(x)		GPIO_WriteBit(GPIOB,GPIO_Pin_9,(BitAction)(x))
#define OLED_W_D1(x)		GPIO_WriteBit(GPIOB,GPIO_Pin_8,(BitAction)(x))
#define OLED_W_RES(x)		GPIO_WriteBit(GPIOB,GPIO_Pin_7,(BitAction)(x))
#define OLED_W_DC(x)		GPIO_WriteBit(GPIOB,GPIO_Pin_6,(BitAction)(x))
#define OLED_W_CS(x)		GPIO_WriteBit(GPIOA,GPIO_Pin_8,(BitAction)(x))


void OLED_Init(void);
void OLED_Clear(void);
void OLED_SetCursor(uint8_t Y, uint8_t X);
void OLED_WriteCommand(uint8_t Command);
void OLED_ShowChar(uint8_t row,uint8_t col,char Chr,uint8_t inv);
void OLED_ShowString(uint8_t row,uint8_t col,char *str,uint8_t inv);
int oledPrint(u8 row,u8 col,const char *formate,...);


#endif /*__SPI_OLED_H*/



