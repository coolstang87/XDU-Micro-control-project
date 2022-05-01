#ifndef __MY_TCS34725_H
#define __MY_TCS34725_H
#include "main.h"


#define u8 uint8_t
#define u16 uint16_t

struct MY_TCS_DATA{
    u16 cv_or,rv_or,gv_or,bv_or;
    float rv_f,gv_f,bv_f;
    u16 rv,gv,bv;
};

extern struct MY_TCS_DATA tcs_data;


u8 my_tcs34725_init();

//读取颜色数据
u8 my_tcs34725_read();

u8 my_tcs34725_colour_balance();

void my_tcs34725_get_color();

#endif
