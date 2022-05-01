#ifndef __MY_DELAY__
#define __MY_DELAY__
//#include "sys.h"
#include <stdint.h>
#include "stm32l4xx.h"
#include "stm32l4xx_hal_gpio.h"
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
//#define GPIO_PIN_11                ((uint16_t)0x0800)  /* Pin 11 selected   */
//#define GPIO_PIN_12                ((uint16_t)0x1000)  /* Pin 12 selected   */
   	   		   
//IO方向设置


//IO操作函数	 

#define u8 uint8_t
#define u16 uint16_t

//IIC所有操作函数
//void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号
static void IIC_Dev_Write_Bytes(u8 devaddr,u8 addr,u8 len,u8 *wbuf);
static void IIC_Dev_Write_Byte(u8 devaddr,u8 addr,u8 data);
static void IIC_Dev_Read_Bytes(u8 devaddr,u8 addr,u8 len,u8 *rbuf);
static u8 IIC_Dev_Read_Byte(u8 devaddr,u8 addr);
//利用已有的SysTick配置，读取计数判断延迟
void delay_us(uint32_t udelay);

//利用已有的SysTick配置，读取计数判断延迟
void delay_ms(uint32_t udelay);




#endif

