
#include "vl_6180delay.h"
#include "main.h"
// IIC ??? SCL
#define IIC2_SCL_PORT IIC2_SCL_GPIO_Port
#define IIC2_SCL_PIN  IIC2_SCL_Pin

 
// IIC ??? SDA
#define IIC2_SDA_PORT IIC2_SDA_GPIO_Port
#define IIC2_SDA_PIN  IIC2_SDA_Pin

//#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//IIC 驱动函数	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/6/10 
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

//初始化IIC
//void IIC_Init(void)
//{					     
//	GPIO_InitTypeDef GPIO_InitStructure;
//	//RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟 
//	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );	
//	   
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
// 
//	IIC_SCL=1;
//	IIC_SDA=1;

//}
#define SYS_CLOCK 80

//?????SysTick??,????????
//void delay_us(uint32_t udelay)
//{
//  uint32_t startval,tickn,delays,wait;
// 
//  startval = SysTick->VAL;
//  tickn = HAL_GetTick();
//  //sysc = 72000;  //SystemCoreClock / (1000U / uwTickFreq);
//  delays =udelay * SYS_CLOCK; //sysc / 1000 * udelay;
//  if(delays > startval)
//    {
//      while(HAL_GetTick() == tickn)
//        {
// 
//        }
//      wait =SYS_CLOCK*1000 + startval - delays;// 48000 + startval - delays;
//      while(wait < SysTick->VAL)
//        {
// 
//        }
//    }
//  else
//    {
//      wait = startval - delays;
//      while(wait < SysTick->VAL && HAL_GetTick() == tickn)
//        {
// 
//        }
//    }
//}
//void delay_ms(uint32_t udelay)
//{
//  for(int i=0;i<udelay;i++){
//	   delay_us(1000);
//	}
//}
static void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = IIC2_SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(IIC2_SDA_PORT, &GPIO_InitStruct);
}
//????????????
static void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = IIC2_SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(IIC2_SDA_PORT, &GPIO_InitStruct);
}
//产生IIC起始信号
//???????
static void IIC_SCL( u8 c){
	 //?????
   if(c==1) HAL_GPIO_WritePin(IIC2_SCL_PORT, IIC2_SCL_PIN, GPIO_PIN_SET);
	 //?????
	 else HAL_GPIO_WritePin(IIC2_SCL_PORT, IIC2_SCL_PIN, GPIO_PIN_RESET);
}

//???????
static void IIC_SDA( u8 c){
	 //?????
   if(c==1) HAL_GPIO_WritePin(IIC2_SDA_PORT, IIC2_SDA_PIN, GPIO_PIN_SET);
	 //?????
	 else HAL_GPIO_WritePin(IIC2_SDA_PORT, IIC2_SDA_PIN, GPIO_PIN_RESET);
}
static u8 READ_SDA(){
	
	 return HAL_GPIO_ReadPin(IIC2_SDA_PORT,IIC2_SDA_PIN);
	
}
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA(1);	  	  
	IIC_SCL(1);
	delay_us(4);
 	IIC_SDA(0);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL(0);//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL(0);
	IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL(1); 
	IIC_SDA(1);//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA(1);delay_us(1);	   
	IIC_SCL(1);delay_us(1);	 
	while(READ_SDA())//返回一个低电平证明接收到了
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL(0);//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(0);//产生应答就把总线拉低
	delay_us(2);
	IIC_SCL(1);
	delay_us(2);
	IIC_SCL(0);
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(1);
	delay_us(2);
	IIC_SCL(1);
	delay_us(2);
	IIC_SCL(0);
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
    IIC_SCL(0);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA((txd&0x80)>>7);
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL(1);
		delay_us(2); 
		IIC_SCL(0);	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL(0); 
        delay_us(2);
		IIC_SCL(1);
        receive<<=1;
        if(READ_SDA())receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}
static u8 IIC_Dev_Read_Byte(u8 devaddr,u8 addr)
{				  
	u8 temp=0;		  	    																 
	IIC_Start();  
	IIC_Send_Byte(devaddr);//?????
	IIC_Wait_Ack(); 
	IIC_Send_Byte(addr);//?????
	IIC_Wait_Ack();	

	IIC_Start();  	 	   
	IIC_Send_Byte(devaddr|1); //???		   
	IIC_Wait_Ack();	 
	temp=IIC_Read_Byte(0);			   
	IIC_Stop();  
	return temp;
}

//???????
//addr:????
//rbuf:?????
//len:????
static void IIC_Dev_Read_Bytes(u8 devaddr,u8 addr,u8 len,u8 *rbuf)
{
	int i=0;
	IIC_Start();  
	IIC_Send_Byte(devaddr);  
	IIC_Wait_Ack();	
	IIC_Send_Byte(addr);
	IIC_Wait_Ack();	
	IIC_Start();  	
	IIC_Send_Byte(devaddr|1);  	
	IIC_Wait_Ack();	
	for(i=0; i<len; i++)
	{
		if(i==len-1)
		{
			rbuf[i]=IIC_Read_Byte(0); 
		}
		else
			rbuf[i]=IIC_Read_Byte(1);
	}
	IIC_Stop( );	
}

//???????????
//WriteAddr :?????????    
//DataToWrite:??????
static void IIC_Dev_Write_Byte(u8 devaddr,u8 addr,u8 data)
{				   	  	    																 
	IIC_Start();  
	IIC_Send_Byte(devaddr); 
	IIC_Wait_Ack();	   
	IIC_Send_Byte(addr); 
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(data);			   
	IIC_Wait_Ack();  		    	   
	IIC_Stop();	
}

//???????
//addr:????
//wbuf:?????
//len:?????
static void IIC_Dev_Write_Bytes(u8 devaddr,u8 addr,u8 len,u8 *wbuf)
{
	int i=0;
	IIC_Start();  
	IIC_Send_Byte(devaddr);  	
	IIC_Wait_Ack();	
	IIC_Send_Byte(addr);  //????
	IIC_Wait_Ack();	
	for(i=0; i<len; i++)
	{
		IIC_Send_Byte(wbuf[i]);  
		IIC_Wait_Ack();		
	}
	IIC_Stop( );	
}

//----------------------------------------------??---------------------------------


























