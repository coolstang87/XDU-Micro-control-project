#include "delay.h"

#define SYS_CLOCK 80

//?????SysTick??,????????
void delay_us(uint32_t udelay)
{
  uint32_t startval,tickn,delays,wait;
 
  startval = SysTick->VAL;
  tickn = HAL_GetTick();
  //sysc = 72000;  //SystemCoreClock / (1000U / uwTickFreq);
  delays =udelay * SYS_CLOCK; //sysc / 1000 * udelay;
  if(delays > startval)
    {
      while(HAL_GetTick() == tickn)
        {
 
        }
      wait =SYS_CLOCK*1000 + startval - delays;// 48000 + startval - delays;
      while(wait < SysTick->VAL)
        {
 
        }
    }
  else
    {
      wait = startval - delays;
      while(wait < SysTick->VAL && HAL_GetTick() == tickn)
        {
 
        }
    }
}

//?????SysTick??,????????
void delay_ms(uint32_t udelay)
{
  for(int i=0;i<udelay;i++){
	   delay_us(1000);
	}
}