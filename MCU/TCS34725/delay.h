#ifndef __MY_DELAY__
#define __MY_DELAY__
#include "main.h"


//利用已有的SysTick配置，读取计数判断延迟
void delay_us(uint32_t udelay);

//利用已有的SysTick配置，读取计数判断延迟
void delay_ms(uint32_t udelay);

#endif
