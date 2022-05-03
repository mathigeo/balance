#include <i2c.h>
#include <systick.h>


/**
 * @brief I2C引脚初始化
 * @param None
 * @retval None
 */
void IIC_Init(void)
{
	//GPIO初始化结构体
	GPIO_InitTypeDef  GPIO_InitStructure;	
	//使能时钟
	RCC_APB2PeriphClockCmd(IIC_SCL_PORT_RCC|IIC_SDA_PORT_RCC,ENABLE);	
	
	GPIO_InitStructure.GPIO_Pin=IIC_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(IIC_SCL_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
	
	IIC_SCL=1;
	IIC_SDA=1;
}


/**
 * @brief SDA设置输出模式
 * @param None
 * @retval None
 */
void SDA_OUT()	
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	//推挽输出
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
}


/**
 * @brief SDA设置输入模式
 * @param None
 * @retval None
 */
void SDA_IN(void)	
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	//上拉输入
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
}


/**
 * @brief IIC起始信号
 * @param None
 * @retval None
 */
void IIC_Start(void)	
{
	//sda线输出
	SDA_OUT();     
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	//要求时间>4.7us
	delay_us(5);	
 	IIC_SDA=0;	
	//要求时间>4us
	delay_us(6);	
	//钳住I2C总线，准备发送或接收数据
	IIC_SCL=0; 
}



/**
 * @brief IIC停止信号
 * @param None
 * @retval None
 */
void IIC_Stop(void)	
{
	//sda线输出
	SDA_OUT();
	IIC_SCL=0;
	IIC_SDA=0;
 	IIC_SCL=1; 
	//要求时间>4us
	delay_us(6); 	
	//发送I2C总线结束信号
	IIC_SDA=1;
	//要求时间>4us		
	delay_us(6);						   	
}



/**
 * @brief 等待应答信号
 * @param None
 * @retval 返回0成功,1失败
 */
u8 IIC_Wait_Ack(void)
{
	u8 tempTime=0;
	
	IIC_SDA=1;
	delay_us(1);
	//SDA设置为输入  
	SDA_IN();      	   
	IIC_SCL=1;
	delay_us(1);
	//等待应答	 
	while(READ_SDA)	
	{
		tempTime++;
		//等待应答的最大时间 超过则失败
		if(tempTime>250)	
		{
			IIC_Stop();
			return 1;
		}
	}
	//时钟输出0 	 
	IIC_SCL=0;  
	return 0;  
} 



/**
 * @brief IIC产生应答
 * @param None
 * @retval None
 */
void IIC_Ack(void)	
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	//要求>4us
	delay_us(5);	
	IIC_SCL=0;
}



/**
 * @brief 产生非应答
 * @param None
 * @retval None
 */
void IIC_NAck(void)	//产生非应答
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	//要求>4us
	delay_us(5);	
	IIC_SCL=0;
}



/**
 * @brief 发送一个字节
 * @param txd 要发送的字节
 * @retval None
 */
void IIC_Send_Byte(u8 txd)	//发送一个字节
{                        
    u8 t;   
	SDA_OUT(); 	
	//拉低时钟开始数据传输    
    IIC_SCL=0;
    for(t=0;t<8;t++)
    {          
		//从最高位开始发送    
        if((txd&0x80)>0) 
		{
			IIC_SDA=1;
		}
		else
		{
			IIC_SDA=0;
        }
		txd<<=1; 	 
		//对TEA5767这三个延时都是必须的 
		delay_us(2);   
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
}



/**
 * @brief 读取一个字节
 * @param ack 1发送应答,0发送非应答
 * @retval 读取的字节
 */
u8 IIC_Read_Byte(u8 ack)	
{
	u8 i,receive=0;
	//SDA设置为输入
	SDA_IN();
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if(!ack)
	{
		//发送nACK
        IIC_NAck();
    }
	else
	{
		//发送ACK   
		IIC_Ack(); 
	}
	return receive;
}




