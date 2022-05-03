#include "MPU6050.h"
#include "system.h"
#include "systick.h"
#include "usart.h"  



/**
 * @brief 初始化MPU6050
 * @param None
 * @retval 返回0,成功
 * 	其他,错误代码
 */
u8 MPU6050_Init(void)
{ 
	u8 res;
	//初始化IIC总线
	IIC_Init();
	//复位MPU6050
	MPU6050_Write_Byte(MPU6050_PWR_MGMT1_REG,0X80);	
    delay_ms(100);
	//唤醒MPU6050
	MPU6050_Write_Byte(MPU6050_PWR_MGMT1_REG,0X00);	
	//陀螺仪传感器,±2000dps  角速度
	MPU6050_Set_Gyro_Fsr(3);
	//加速度传感器,±2g
	MPU6050_Set_Accel_Fsr(0);
	//设置采样率100Hz
	MPU6050_Set_Rate(100);	
	//关闭所有中断
	MPU6050_Write_Byte(MPU6050_INT_EN_REG,0X00);	
	//I2C主模式关闭
	MPU6050_Write_Byte(MPU6050_USER_CTRL_REG,0X00);	
	//关闭FIFO
	MPU6050_Write_Byte(MPU6050_FIFO_EN_REG,0X00);	
	//INT引脚低电平有效
	MPU6050_Write_Byte(MPU6050_INTBP_CFG_REG,0X80);
	
	res=MPU6050_Read_Byte(MPU6050_DEVICE_ID_REG);
	
	//器件ID错误
	if(res !=MPU6050_ADDR)
		return 1;
	//设置CLKSEL, x轴陀螺PLL作为时钟源，以获得更高精度的时钟
	MPU6050_Write_Byte(MPU6050_PWR_MGMT1_REG,0X01);	
	//加速度与陀螺仪都工作
	MPU6050_Write_Byte(MPU6050_PWR_MGMT2_REG,0X00);	
	//设置采样率为100Hz
	MPU6050_Set_Rate(100);						

	return 0;
}


/**
 * @brief 设置MPU6050陀螺仪传感器满量程范围
 * @param fsr 0,±250dps; 1,±500dps; 2,±1000dps; 3,±2000dps
 * @retval 返回0,设置成功
 * 	其他,设置失败 
 */ 
u8 MPU6050_Set_Gyro_Fsr(u8 fsr)
{
	//设置陀螺仪满量程范围
	return MPU6050_Write_Byte(MPU6050_GYRO_CFG_REG,fsr<<3);  
}


/**
 * @brief 设置MPU6050加速度传感器满量程范围
 * @param fsr 0,±2g; 1,±4g; 2,±8g; 3,±16g
 * @retval 返回0,设置成功
 * 	其他,设置失败 
 */   
u8 MPU6050_Set_Accel_Fsr(u8 fsr)
{
	//设置加速度传感器满量程范围  
	return MPU6050_Write_Byte(MPU6050_ACCEL_CFG_REG,fsr<<3);
}


/**
 * @brief 设置MPU6050的数字低通滤波器
 * @param lpf 数字低通滤波频率(Hz)
 * @retval 返回0,设置成功
 * 	其他,设置失败 
 */
u8 MPU6050_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	//设置数字低通滤波器  
	return MPU6050_Write_Byte(MPU6050_CFG_REG,data);
}

/**
 * @brief 设置MPU6050的采样率(假定Fs=1KHz)
 * @param rate 4~1000(Hz)
 * @retval 返回0,设置成功
 * 	其他,设置失败 
 */   
u8 MPU6050_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	//设置数字低通滤波器
	data=MPU6050_Write_Byte(MPU6050_SAMPLE_RATE_REG,data);	
	//自动设置LPF为采样率的一半
 	return MPU6050_Set_LPF(rate/2);	
}


/**
 * @brief 得到温度值
 * @param None
 * @retval 温度值(扩大了100倍)
 */
short MPU6050_Get_Temperature(void)
{
    u8 buf[2]; 
    short raw;
	float temp;
	MPU6050_Read_Len(MPU6050_ADDR,MPU6050_TEMP_OUTH_REG,2,buf); 
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp*100;;
}


/**
 * @brief 得到陀螺仪值(原始值)
 * @param gx 陀螺仪x轴的原始读数(带符号)
 * @param gy 陀螺仪y轴的原始读数(带符号)
 * @param gz 陀螺仪z轴的原始读数(带符号)
 * @retval 返回0,成功
 * 	其他,错误代码
 */   
u8 MPU6050_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    u8 buf[6],res;  
	res=MPU6050_Read_Len(MPU6050_ADDR,MPU6050_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}


/**
 * @brief 得到加速度值(原始值)
 * @param gx 陀螺仪x轴的原始读数(带符号)
 * @param gy 陀螺仪y轴的原始读数(带符号)
 * @param gz 陀螺仪z轴的原始读数(带符号)
 * @retval 返回0,成功
 * 	其他,错误代码
 */  
u8 MPU6050_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	res=MPU6050_Read_Len(MPU6050_ADDR,MPU6050_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}



/**
 * @brief IIC连续写
 * @param addr 器件地址 
 * @param reg 寄存器地址
 * @param len 写入长度
 * @param buf 数据区
 * @retval 返回0,正常
 *	其他,错误代码
 */    
u8 MPU6050_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    IIC_Start(); 
	//发送器件地址+写命令	
	IIC_Send_Byte((addr<<1)|0);
	//等待应答
	if(IIC_Wait_Ack())	
	{
		IIC_Stop();		 
		return 1;		
	}
	//写寄存器地址
    IIC_Send_Byte(reg);	
	//等待应答
    IIC_Wait_Ack();		
	for(i=0;i<len;i++)
	{
		//发送数据
		IIC_Send_Byte(buf[i]);	
		//等待ACK
		if(IIC_Wait_Ack())		
		{
			IIC_Stop();	 
			return 1;		 
		}		
	}    
    IIC_Stop();	 
	return 0;	
} 



/**
 * @brief IIC连续读
 * @param addr 器件地址
 * @param reg 要读取的寄存器地址
 * @param len 要读取的长度
 * @param buf 读取到的数据存储区
 * @retval 返回0,正常
 * 	其他,错误代码
 */  
u8 MPU6050_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	IIC_Start(); 
	//发送器件地址+写命令	
	IIC_Send_Byte((addr<<1)|0);
	//等待应答
	if(IIC_Wait_Ack())	
	{
		IIC_Stop();		 
		return 1;		
	}
	//写寄存器地址
    IIC_Send_Byte(reg);
	//等待应答
    IIC_Wait_Ack();		
    IIC_Start();
	//发送器件地址+读命令	
	IIC_Send_Byte((addr<<1)|1);
	//等待应答 
    IIC_Wait_Ack();		
	while(len)
	{
		//读数据,发送nACK 
		if(len==1)*buf=IIC_Read_Byte(0);
		//读数据,发送ACK  
		else *buf=IIC_Read_Byte(1);		
		len--;
		buf++; 
	}   
	//产生一个停止条件 
    IIC_Stop();	
	return 0;	
}



/**
 * @brief IIC写一个字节 
 * @param reg 寄存器地址
 * @param data 数据
 * @retval 返回0,正常
 * 	其他,错误代码
 */   
u8 MPU6050_Write_Byte(u8 reg,u8 data) 				 
{ 
    IIC_Start(); 
	//发送器件地址+写命令	
	IIC_Send_Byte((MPU6050_ADDR<<1)|0);
	//等待应答
	if(IIC_Wait_Ack())	
	{
		IIC_Stop();		 
		return 1;		
	}
	//写寄存器地址
    IIC_Send_Byte(reg);
	//等待应答 	
    IIC_Wait_Ack();	
	//发送数据	
	IIC_Send_Byte(data);
	//等待ACK
	if(IIC_Wait_Ack())	
	{
		IIC_Stop();	 
		return 1;		 
	}		 
    IIC_Stop();	 
	return 0;
}


/**
 * @brief IIC读一个字节 
 * @param reg 寄存器地址 
 * @retval 返回读到的数据
 */
u8 MPU6050_Read_Byte(u8 reg)
{
	u8 res;
    IIC_Start(); 
	//发送器件地址+写命令	
	IIC_Send_Byte((MPU6050_ADDR<<1)|0);
	//等待应答 
	IIC_Wait_Ack();	
	//写寄存器地址
    IIC_Send_Byte(reg);	
	//等待应答
    IIC_Wait_Ack();		
    IIC_Start();
	//发送器件地址+读命令
	IIC_Send_Byte((MPU6050_ADDR<<1)|1);	
	//等待应答 
    IIC_Wait_Ack();		
	//读取数据,发送nACK 
	res=IIC_Read_Byte(0);
	//产生一个停止条件 
    IIC_Stop();			
	return res;		
}


