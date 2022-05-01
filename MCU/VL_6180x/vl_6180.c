#include "vl_6180.h"
#include "vl_6180delay.h"


#include "stm32L4xx_hal.h"
#define addr_write 0x52
#define addr_read  0x53
/*
  初始化GPIO_0，GPIO_1端口
  并置输出为高电平
*/
//void VL6180X_GPIO_Init()
//{
//    GPIO_InitTypeDef  GPIO_InitStructure;
//	/*初始化GPIO1*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOB,GPIO_Pin_9);
//	/*初始化GPIO0*/
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
//	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//    GPIO_Init(GPIOC, &GPIO_InitStructure);
//    GPIO_SetBits(GPIOC,GPIO_Pin_13);
//}


/*
   参数：addrdata是命令
         *data要发送的数据
		 count发送的字节个数
*/
 void Data_write(uint8_t addrdata,uint8_t *data,uint8_t count)
{
	uint8_t i=0;
	IIC_Start();
	IIC_Send_Byte(addrdata);
	IIC_Wait_Ack(); 
    for(i=0;i<count;i++)
    {			
	  IIC_Send_Byte(*(data+i));	
      IIC_Wait_Ack(); 	
    }	
    IIC_Stop();	
}

/*
   参数：addrdata 读命令
         返回的是字节数据
*/
uint8_t Data_read(uint8_t addrdata)
{
	uint8_t data=0;
	IIC_Start();
	IIC_Send_Byte(addrdata);//自己理解应该是0x53
	IIC_Wait_Ack(); 
	data=IIC_Read_Byte(0);
	IIC_Stop();	
    return data;
}

/*
   参数：reg 寄存器地址
         data 写入寄存器地址数据
*/
uint8_t  VL6180X_WriteByte(uint16_t reg,uint8_t data)
{
	uint8_t Index_H = (uint8_t)(reg >> 8);
	uint8_t Index_L = (uint8_t)(reg & 0xFF);
	
	IIC_Start();
	IIC_Send_Byte((VL6180X_DEFAULT_I2C_ADDR<<1)|0);
	if(IIC_Wait_Ack())	//????
	{
		IIC_Stop();	
		return 1;		
	}
	IIC_Send_Byte(Index_H);
	IIC_Wait_Ack();	//??ACK
	IIC_Send_Byte(Index_L);
	IIC_Wait_Ack();	//??ACK
	IIC_Send_Byte(data);
	if(IIC_Wait_Ack())	//??ACK
	{
		IIC_Stop();	 
		return 1;		 
	}
	IIC_Stop();
	return 0;	
}

/*
   参数：reg要读取寄存器地址

*/
uint8_t VL6180X_ReadByte(uint16_t reg)

{
	uint8_t res;
	uint8_t Index_H = (uint8_t)(reg >> 8);
	uint8_t Index_L = (uint8_t)(reg & 0xff);
    IIC_Start(); 
		IIC_Send_Byte((VL6180X_DEFAULT_I2C_ADDR<<1)|0);//??????+???	
		if(IIC_Wait_Ack())goto err;	//???? 
    IIC_Send_Byte(Index_H);	//??????
    if(IIC_Wait_Ack())goto err;		//????
		IIC_Send_Byte(Index_L);	//??????
		if(IIC_Wait_Ack())goto err;	
	
    IIC_Start();
		IIC_Send_Byte((VL6180X_DEFAULT_I2C_ADDR<<1)|1);//??????+???	
    if(IIC_Wait_Ack())goto err;		//???? 
		res=IIC_Read_Byte(0);//????,??nACK 
    IIC_Stop();	
	err:	//???????? 
	return res;

}

uint8_t VL6180X_Read_ID(void)
{
	return VL6180X_ReadByte(VL6180X_REG_IDENTIFICATION_MODEL_ID);
}
/*
   VL6180X初始化
*/
  uint8_t VL6180X_Init()
  {
    if(VL6180X_Read_ID() == VL6180X_DEFAULT_ID)
	{	
		VL6180X_WriteByte(0x0207, 0x01);
		VL6180X_WriteByte(0x0208, 0x01);
		VL6180X_WriteByte(0x0096, 0x00);
		VL6180X_WriteByte(0x0097, 0xfd);
		VL6180X_WriteByte(0x00e3, 0x00);
		VL6180X_WriteByte(0x00e4, 0x04);
		VL6180X_WriteByte(0x00e5, 0x02);
		VL6180X_WriteByte(0x00e6, 0x01);
		VL6180X_WriteByte(0x00e7, 0x03);
		VL6180X_WriteByte(0x00f5, 0x02);
		VL6180X_WriteByte(0x00d9, 0x05);
		VL6180X_WriteByte(0x00db, 0xce);
		VL6180X_WriteByte(0x00dc, 0x03);
		VL6180X_WriteByte(0x00dd, 0xf8);
		VL6180X_WriteByte(0x009f, 0x00);
		VL6180X_WriteByte(0x00a3, 0x3c);
		VL6180X_WriteByte(0x00b7, 0x00);
		VL6180X_WriteByte(0x00bb, 0x3c);
		VL6180X_WriteByte(0x00b2, 0x09);
		VL6180X_WriteByte(0x00ca, 0x09);
		VL6180X_WriteByte(0x0198, 0x01);
		VL6180X_WriteByte(0x01b0, 0x17);
		VL6180X_WriteByte(0x01ad, 0x00);
		VL6180X_WriteByte(0x00ff, 0x05);
		VL6180X_WriteByte(0x0100, 0x05);
		VL6180X_WriteByte(0x0199, 0x05);
		VL6180X_WriteByte(0x01a6, 0x1b);
		VL6180X_WriteByte(0x01ac, 0x3e);
		VL6180X_WriteByte(0x01a7, 0x1f);
		VL6180X_WriteByte(0x0030, 0x00);
		
		// Recommended : Public registers - See data sheet for more detail
		VL6180X_WriteByte(0x0011, 0x10);       // Enables polling for 'New Sample ready'
									// when measurement completes
		VL6180X_WriteByte(0x010a, 0x30);       // Set the averaging sample period
									// (compromise between lower noise and
									// increased execution time)
		VL6180X_WriteByte(0x003f, 0x46);       // Sets the light and dark gain (upper
									// nibble). Dark gain should not be
									// changed. !???????0x4	?????1.0
		VL6180X_WriteByte(0x0031, 0xFF);       // sets the # of range measurements after
									// which auto calibration of system is
									// performed
		VL6180X_WriteByte(0x0040, 0x63);       // Set ALS integration time to 100ms
		VL6180X_WriteByte(0x002e, 0x01);       // perform a single temperature calibration
									// of the ranging sensor

		// Optional: Public registers - See data sheet for more detail
		VL6180X_WriteByte(0x001b, 0x09);    //????	????
									// period to 100ms	??10ms->0-10ms
		VL6180X_WriteByte(0x003e, 0x31);      //????	ALS??
									// to 500ms		
		VL6180X_WriteByte(0x0014, 0x24);       // Configures interrupt on 'New Sample
									// Ready threshold event'
		return 0;
	}
	else return 1;
}

  
/*
    开始测量模式选择
    选择单次测量模式
*/
 uint8_t VL6180X_Start_Range()
  {
     VL6180X_WriteByte(0x018,0x01);
	 return 0;
  }
  
uint16_t timeoutcnt=0; 
/*poll for new sample ready */
uint8_t VL6180X_Poll_Range()
{
		uint16_t t=0;
    uint8_t status;
	  uint8_t range_status;
	  status=VL6180X_ReadByte(0x04f);
    range_status=status&0x07;
		while(range_status!=0x04)
		{
			  timeoutcnt++;
				if(timeoutcnt > 2)
				{
					break;
				}
				status=VL6180X_ReadByte(0x04f);
				range_status=status&0x07;
				delay_ms(1);
			
		}		  
		return 0;
}
   
  
  /*read range result (mm)*/
uint8_t VL6180X_Read_Range(void)
{
	uint8_t range = 0;
	//????
	while(!(VL6180X_ReadByte(VL6180X_REG_RESULT_RANGE_STATUS) & 0x01));
		delay_ms(2);
	VL6180X_WriteByte(VL6180X_REG_SYSRANGE_START,0x01);	//??????
	//???????????(New Sample Ready threshold event)
		delay_ms(2);
	while(!(VL6180X_ReadByte(VL6180X_REG_RESULT_INTERRUPT_STATUS_GPIO) & 0x04))
	{if(!(VL6180X_ReadByte(VL6180X_REG_RESULT_INTERRUPT_STATUS_GPIO) & 0x04));
		else return 255;
	}
	range = VL6180X_ReadByte(VL6180X_REG_RESULT_RANGE_VAL);
	//?????,?????
		delay_ms(2);
	VL6180X_WriteByte(VL6180X_REG_SYSTEM_INTERRUPT_CLEAR,0x07);	//0111b ?????????
	delay_ms(2);
	return range;
}

  /*clear interrupt*/
  void VL6180X_Clear_Interrupt()
  {
     VL6180X_WriteByte(0x015,0x07); 
  }
