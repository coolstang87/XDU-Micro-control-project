#include "TCS34725.h"
#include "delay.h"

//////////////////////////////////

// IIC ?????
#define IIC_ADDR (0x29<<1)

// IIC ??? SCL
#define IIC_SCL_PORT IIC_SCL_GPIO_Port
#define IIC_SCL_PIN  IIC_SCL_Pin

 
// IIC ??? SDA
#define IIC_SDA_PORT IIC_SDA_GPIO_Port
#define IIC_SDA_PIN  IIC_SDA_Pin

/////////////////////////////////



//???????
static void IIC_SCL_Control( u8 c){
	 //?????
   if(c==1) HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_SET);
	 //?????
	 else HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_RESET);
}

//???????
static void IIC_SDA_Control( u8 c){
	 //?????
   if(c==1) HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_SET);
	 //?????
	 else HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_RESET);
}

//???????
static u8 IIC_SDA_Read(){
	
	 return HAL_GPIO_ReadPin(IIC_SDA_PORT,IIC_SDA_PIN);
	
}

//????????????
static void IIC_SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = IIC_SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(IIC_SDA_PORT, &GPIO_InitStruct);
}

//????????????
static void IIC_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = IIC_SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(IIC_SDA_PORT, &GPIO_InitStruct);
}

//----------------------------------------------IIC??-------------------------------

//IIC????
static void IIC_Start(void)
{
	IIC_SDA_OUT(); 
	IIC_SDA_Control(1);	  	  
	IIC_SCL_Control(1);
	delay_us(4);
 	IIC_SDA_Control(0);  
	delay_us(4);
	IIC_SCL_Control(0); 
}	  

//IIC????
static void IIC_Stop(void)
{
	IIC_SDA_OUT();  
	IIC_SCL_Control(0);
	IIC_SDA_Control(0);
 	delay_us(4);
	IIC_SCL_Control(1); 
	IIC_SDA_Control(1);
	delay_us(4);							   	
}

//????????
//???:1,??????
//        0,??????
static u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC_SDA_IN();
	IIC_SDA_Control(1);delay_us(1);	   
	IIC_SCL_Control(1);delay_us(1);	 
	while(IIC_SDA_Read())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_Control(0); 
	return 0;  
} 

//??ACK??
static void IIC_Ack(void)
{
	IIC_SCL_Control(0);
	IIC_SDA_OUT();
	IIC_SDA_Control(0);
	delay_us(2);
	IIC_SCL_Control(1);
	delay_us(2);
	IIC_SCL_Control(0);
}

//???ACK??		    
static void IIC_NAck(void)
{
	IIC_SCL_Control(0);
	IIC_SDA_OUT();
	IIC_SDA_Control(1);
	delay_us(2);
	IIC_SCL_Control(1);
	delay_us(2);
	IIC_SCL_Control(0);
}		

//IIC??????
//????????
//1,???
//0,???			  
static void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	IIC_SDA_OUT(); 	    
    IIC_SCL_Control(0);
    for(t=0;t<8;t++)
    {              
        IIC_SDA_Control((txd&0x80)>>7);
        txd<<=1; 	  
		delay_us(2); 
		IIC_SCL_Control(1);
		delay_us(2); 
		IIC_SCL_Control(0);	
		delay_us(2);
    }	 
} 	

//?1???,ack=1?,??ACK,ack=0,??nACK   
static u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_SDA_IN();    
    for(i=0;i<8;i++ )
	{
        IIC_SCL_Control(0); 
        delay_us(2);
		    IIC_SCL_Control(1);
        receive<<=1;
        if(IIC_SDA_Read())receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();    
    else
        IIC_Ack();  
    return receive;
}

//???????????
//ReadAddr:???????  
//???  :?????
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

#define TCS34725_ADDRESS          (0x29)

#define TCS34725_COMMAND_BIT      (0x80)

#define TCS34725_ENABLE           (0x00)
#define TCS34725_ENABLE_AIEN      (0x10)    /* RGBC Interrupt Enable */
#define TCS34725_ENABLE_WEN       (0x08)    /* Wait enable - Writing 1 activates the wait timer */
#define TCS34725_ENABLE_AEN       (0x02)    /* RGBC Enable - Writing 1 actives the ADC, 0 disables it */
#define TCS34725_ENABLE_PON       (0x01)    /* Power on - Writing 1 activates the internal oscillator, 0 disables it */
#define TCS34725_ATIME            (0x01)    /* Integration time */
//#define TCS34725_WTIME            (0x03)    /* Wait time (if TCS34725_ENABLE_WEN is asserted) */
#define TCS34725_WTIME_2_4MS      (0xFF)    /* WLONG0 = 2.4ms   WLONG1 = 0.029s */
#define TCS34725_WTIME_204MS      (0xAB)    /* WLONG0 = 204ms   WLONG1 = 2.45s  */
#define TCS34725_WTIME_614MS      (0x00)    /* WLONG0 = 614ms   WLONG1 = 7.4s   */
#define TCS34725_AILTL            (0x04)    /* Clear channel lower interrupt threshold */
#define TCS34725_AILTH            (0x05)
#define TCS34725_AIHTL            (0x06)    /* Clear channel upper interrupt threshold */
#define TCS34725_AIHTH            (0x07)
#define TCS34725_PERS             (0x0C)    /* Persistence register - basic SW filtering mechanism for interrupts */
#define TCS34725_PERS_NONE        (0b0000)  /* Every RGBC cycle generates an interrupt                                */
#define TCS34725_PERS_1_CYCLE     (0b0001)  /* 1 clean channel value outside threshold range generates an interrupt   */
#define TCS34725_PERS_2_CYCLE     (0b0010)  /* 2 clean channel values outside threshold range generates an interrupt  */
#define TCS34725_PERS_3_CYCLE     (0b0011)  /* 3 clean channel values outside threshold range generates an interrupt  */
#define TCS34725_PERS_5_CYCLE     (0b0100)  /* 5 clean channel values outside threshold range generates an interrupt  */
#define TCS34725_PERS_10_CYCLE    (0b0101)  /* 10 clean channel values outside threshold range generates an interrupt */
#define TCS34725_PERS_15_CYCLE    (0b0110)  /* 15 clean channel values outside threshold range generates an interrupt */
#define TCS34725_PERS_20_CYCLE    (0b0111)  /* 20 clean channel values outside threshold range generates an interrupt */
#define TCS34725_PERS_25_CYCLE    (0b1000)  /* 25 clean channel values outside threshold range generates an interrupt */
#define TCS34725_PERS_30_CYCLE    (0b1001)  /* 30 clean channel values outside threshold range generates an interrupt */
#define TCS34725_PERS_35_CYCLE    (0b1010)  /* 35 clean channel values outside threshold range generates an interrupt */
#define TCS34725_PERS_40_CYCLE    (0b1011)  /* 40 clean channel values outside threshold range generates an interrupt */
#define TCS34725_PERS_45_CYCLE    (0b1100)  /* 45 clean channel values outside threshold range generates an interrupt */
#define TCS34725_PERS_50_CYCLE    (0b1101)  /* 50 clean channel values outside threshold range generates an interrupt */
#define TCS34725_PERS_55_CYCLE    (0b1110)  /* 55 clean channel values outside threshold range generates an interrupt */
#define TCS34725_PERS_60_CYCLE    (0b1111)  /* 60 clean channel values outside threshold range generates an interrupt */
#define TCS34725_CONFIG           (0x0D)
#define TCS34725_CONFIG_WLONG     (0x02)    /* Choose between short and long (12x) wait times via TCS34725_WTIME */
#define TCS34725_CONTROL          (0x0F)    /* Set the gain level for the sensor */
#define TCS34725_ID               (0x12)    /* 0x44 = TCS34721/TCS34725, 0x4D = TCS34723/TCS34727 */
#define TCS34725_STATUS           (0x13)
#define TCS34725_STATUS_AINT      (0x10)    /* RGBC Clean channel interrupt */
#define TCS34725_STATUS_AVALID    (0x01)    /* Indicates that the RGBC channels have completed an integration cycle */
#define TCS34725_CDATAL           (0x14)    /* Clear channel data */
#define TCS34725_CDATAH           (0x15)
#define TCS34725_RDATAL           (0x16)    /* Red channel data */
#define TCS34725_RDATAH           (0x17)
#define TCS34725_GDATAL           (0x18)    /* Green channel data */
#define TCS34725_GDATAH           (0x19)
#define TCS34725_BDATAL           (0x1A)    /* Blue channel data */
#define TCS34725_BDATAH           (0x1B)

#define TCS34725_INTEGRATIONTIME_2_4MS   0xFF   /**<  2.4ms - 1 cycle    - Max Count: 1024  */
#define TCS34725_INTEGRATIONTIME_24MS    0xF6   /**<  24ms  - 10 cycles  - Max Count: 10240 */
#define TCS34725_INTEGRATIONTIME_50MS    0xEB   /**<  50ms  - 20 cycles  - Max Count: 20480 */
#define TCS34725_INTEGRATIONTIME_101MS   0xD5   /**<  101ms - 42 cycles  - Max Count: 43008 */
#define TCS34725_INTEGRATIONTIME_154MS   0xC0   /**<  154ms - 64 cycles  - Max Count: 65535 */
#define TCS34725_INTEGRATIONTIME_240MS   0x9C   /**<  240ms - 100 cycles - Max Count: 65535 */
#define TCS34725_INTEGRATIONTIME_700MS   0x00   /**<  700ms - 256 cycles - Max Count: 65535 */

#define TCS34725_GAIN_1X                 0x00   /**<  No gain  */
#define TCS34725_GAIN_4X                 0x01   /**<  4x gain  */
#define TCS34725_GAIN_16X                0x02   /**<  16x gain */
#define TCS34725_GAIN_60X                0x03   /**<  60x gain */



/**
??????
????????,0???,1???
**/
u8 my_tcs34725_init(){
  u8 addr=0;
	
	//------------????-----------
	//????ID
  addr=IIC_Dev_Read_Byte(IIC_ADDR,TCS34725_ID| TCS34725_COMMAND_BIT);
	//????????
	if(addr!=0x44) return 0;
	
	//------------????-----------
	//?????
	IIC_Dev_Write_Byte(IIC_ADDR,TCS34725_ATIME | TCS34725_COMMAND_BIT, TCS34725_INTEGRATIONTIME_240MS);
	//?????
	IIC_Dev_Write_Byte(IIC_ADDR,TCS34725_CONTROL | TCS34725_COMMAND_BIT,TCS34725_GAIN_60X);
	
	//-----------????----------
	//?????
	u8 cmd = TCS34725_ENABLE_PON;
	IIC_Dev_Write_Byte(IIC_ADDR,TCS34725_ENABLE | TCS34725_COMMAND_BIT,cmd);
	HAL_Delay(100);
	//??RGBC
	cmd = TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN;
	IIC_Dev_Write_Byte(IIC_ADDR,TCS34725_ENABLE | TCS34725_COMMAND_BIT,cmd);
	HAL_Delay(100);
	
	return 1;
}


struct MY_TCS_DATA tcs_data;


// ????????
uint16_t my_tcs34725_crgb_v(unsigned char reg)
{
    unsigned char tmp[2] = {0,0};
    uint16_t data = 0;

		IIC_Dev_Read_Bytes(IIC_ADDR,reg,2,tmp);
    data = ((uint16_t)tmp[1] << 8) | tmp[0];

    return data;
}

//??????
u8 my_tcs34725_read()
{
    unsigned char status=0;

    status = TCS34725_STATUS_AVALID;

    status=IIC_Dev_Read_Byte(IIC_ADDR,TCS34725_STATUS | TCS34725_COMMAND_BIT);

    if(status & TCS34725_STATUS_AVALID)
    {
        tcs_data.cv_or = my_tcs34725_crgb_v(TCS34725_CDATAL | TCS34725_COMMAND_BIT);
        tcs_data.rv_or = my_tcs34725_crgb_v(TCS34725_RDATAL | TCS34725_COMMAND_BIT); 
        tcs_data.gv_or = my_tcs34725_crgb_v(TCS34725_GDATAL | TCS34725_COMMAND_BIT);  
        tcs_data.bv_or = my_tcs34725_crgb_v(TCS34725_BDATAL | TCS34725_COMMAND_BIT);
				
        return 1;
    }
    return 0;
}

u8 my_tcs34725_colour_balance(){
	 if(my_tcs34725_read()==0) return 0;
	 if(tcs_data.cv_or==0) return 0;
   tcs_data.rv_f=255.0/tcs_data.rv_or;
   tcs_data.gv_f=255.0/tcs_data.gv_or;
	 tcs_data.bv_f=255.0/tcs_data.bv_or;
	 return 1;
}

//?????
void my_tcs34725_get_color(){
	 float minus_rate=0;
	
   tcs_data.rv=tcs_data.rv_or;
   tcs_data.gv=tcs_data.gv_or; 
   tcs_data.bv=tcs_data.bv_or;
	 
    float i=1;
    //Limit data range
    if(tcs_data.rv >= tcs_data.gv &&  tcs_data.rv >= tcs_data.bv){ 
        i = tcs_data.rv / 255 + 1;
    }
    else if(tcs_data.gv >= tcs_data.rv && tcs_data.gv >= tcs_data.bv){ 
        i = tcs_data.gv / 255 + 1;
    }
    else if(tcs_data.bv >=  tcs_data.gv && tcs_data.bv >= tcs_data.rv){ 
        i = tcs_data.bv / 255 + 1;  
    }
    if(i!=0)
    {
        tcs_data.rv = (tcs_data.rv) / i;
        tcs_data.gv = (tcs_data.gv) / i;
        tcs_data.bv = (tcs_data.bv) / i;
    }
    //Amplify data differences
    /*Please don't try to make the data negative, 
        unless you don't change the data type*/
    if(tcs_data.rv > 30)
        tcs_data.rv = tcs_data.rv - 30;
    if(tcs_data.gv > 30)
        tcs_data.gv = tcs_data.gv - 30;
    if(tcs_data.bv > 30)
        tcs_data.bv = tcs_data.bv - 30;
    tcs_data.rv = tcs_data.rv * 255 / 225;
    tcs_data.gv = tcs_data.gv * 255 / 225;
    tcs_data.bv = tcs_data.bv * 255 / 225;
    
    if(tcs_data.rv>255)
           tcs_data.rv = 255; 
    if(tcs_data.gv>255)
           tcs_data.gv = 255; 
    if(tcs_data.bv>255)
           tcs_data.bv = 255; 

}
