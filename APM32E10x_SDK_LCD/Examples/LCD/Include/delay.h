#ifndef __DELAY_H
#define __DELAY_H 	

#include "apm32e10x.h"


void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);


#endif

