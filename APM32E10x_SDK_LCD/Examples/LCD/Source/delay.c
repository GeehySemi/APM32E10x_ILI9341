#include "delay.h"
#include "apm32e10x_misc.h"

static u8  fac_us=0;							//us	   
static u16 fac_ms=0;							//ms


void delay_init()
{
	SysTick_ConfigCLKSource(SYSTICK_CLK_SOURCE_HCLK_DIV8);
	fac_us=SystemCoreClock/8000000;				//system clock 1/8 
	fac_ms=(u16)fac_us*1000;					    
}



void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//time load	  		 
	SysTick->VAL=0x00;        					//clear counter
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//counting down	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//waiting time to arrive
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//Turn off counter
	SysTick->VAL =0X00;      					 //clear counter	 
}


void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;				//time load	
	SysTick->VAL =0x00;							//clear counter
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//counting down	 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//waiting time to arrive  
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//Turn off counter
	SysTick->VAL =0X00;       					//clear counter	    
} 

