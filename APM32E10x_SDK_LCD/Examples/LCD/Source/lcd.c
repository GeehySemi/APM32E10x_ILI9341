#include "lcd.h"
#include "apm32e10x_gpio.h"
#include "apm32e10x_rcm.h"
#include "apm32e10x_smc.h"
#include "delay.h"
#include "font.h"

//LCD brush color and background color
  
u16 POINT_COLOR=0x0000;	//brush color
u16 BACK_COLOR=0xFFFF;  //background color
  

_lcd_dev lcddev;
	 
/*!
*@brief   write command
*@parama  regval :command
*/
void LCD_WR_REG(u16 regval)
{   
	LCD->LCD_REG=regval;
}


/*!
*@brief   write data
*@parama  data :data
*/
void LCD_WR_DATA(u16 data)
{	 
	LCD->LCD_RAM=data;		 
}

/*!
*@brief   read data
*@retval  ram 
*/

u16 LCD_RD_DATA(void)
{
	vu16 ram;			
	ram=LCD->LCD_RAM;	
	return ram;	 
}					   

/*!
*@brief   write command and data
*@parama  LCD_Reg:command
*@parama  LCD_RegValue:data
*@retval  None
*/
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//write command	 
	LCD->LCD_RAM = LCD_RegValue;//write data		 
}	   


/*!
*@brief   write command and read data
*@parama  LCD_Reg:command
*@retval  return value of register
*/
u16 LCD_ReadReg(u16 LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);		
	delay_us(5);		  
	return LCD_RD_DATA();		
}   
/*!
*@brief   write GRAM
*/
void LCD_WriteRAM_Prepare(void)
{
 
	LCD->LCD_REG=lcddev.wramcmd;	  
}	 


/*!
*@brief   The data that read from ILI93xx is GBR format,and transfer to RGB format
*@parama  c:GBR format data
*@retval  rgb:RGB data
*/
u16 LCD_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 

/*!
*@brief  delay
*/
void opt_delay(u8 i)
{
	while(i--);
}


/*!
*@brief   Read the color value of a point
*@parama  x:x coordinate
*@parama  y:y coordinate
*@retval  the color value of a point
*/

u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r=0,g=0,b=0;
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//Exceed limit value,return 0	   
	LCD_SetCursor(x,y);	    
	LCD_WR_REG(0X2E);               //Send the command of reading GRAM
  opt_delay(2);				
 	r=LCD_RD_DATA();								//dummy Read	   
	opt_delay(2);	  
 	r=LCD_RD_DATA();  		  				//actural coordinate color R[D15~D11]&B[D7~D2]

	opt_delay(2);	  
	b=LCD_RD_DATA();                //B[D15~D11]
	g=r&0XFF;		
	g<<=8;

	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));

}			

/*!
*@brief  LCD  Display ON
*/
void LCD_DisplayOn(void)
{					   
	LCD_WR_REG(0X29);	
}	 

/*!
*@brief  LCD  Display OFF
*/
void LCD_DisplayOff(void)
{	   
	LCD_WR_REG(0X28);
}  

/*!
*@brief   setting cursor position
*@parama  x:x coordinate position
*@parama  y:y coordinate position
*/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 	    
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF); 			 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF); 		
} 	

/*!
*@brief   set the LCD automatic scanning mode
*@parama  dir:0~7 Eight direction
*/   	   
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u16 dirreg=0;
	
	switch(dir)
	{
		case L2R_U2D://Left to right, top to bottom
			regval|=(0<<7)|(0<<6)|(0<<5); 
			break;
		case L2R_D2U://Left to right, bottom to top
			regval|=(1<<7)|(0<<6)|(0<<5); 
			break;
		case R2L_U2D://Right to Left, top to bottom
			regval|=(0<<7)|(1<<6)|(0<<5); 
			break;
		case R2L_D2U://Right to Left, bottom to top
			regval|=(1<<7)|(1<<6)|(0<<5); 
			break;	 
		case U2D_L2R://Top to bottom,left to right
			regval|=(0<<7)|(0<<6)|(1<<5); 
			break;
		case U2D_R2L://Top to bottom,right to Left
			regval|=(0<<7)|(1<<6)|(1<<5); 
			break;
		case D2U_L2R://Bottom to top,left to right
			regval|=(1<<7)|(0<<6)|(1<<5); 
			break;
		case D2U_R2L://Bottom to top,right to Left
			regval|=(1<<7)|(1<<6)|(1<<5); 
			break;	 
	}
	
	dirreg=0X36;
	LCD_WriteReg(dirreg,regval);

	LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA(0);LCD_WR_DATA(0);
	LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA(0);LCD_WR_DATA(0);
	LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
}     

/*!
*@brief   point
*@parama  x:x coordinate
*@parama  y:y coordinate
*@parama  POINT_COLOR:point color 
*/
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		
	LCD_WriteRAM_Prepare();	
	LCD->LCD_RAM=POINT_COLOR; 
}

/*!
*@brief   fast draw point
*@parama  x:x coordinate
*@parama  y:y coordinate
*@parama  color:set point color 
*/
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	   
	LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF);  			 
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF); 		 	 
	 
	LCD->LCD_REG=lcddev.wramcmd; 
	LCD->LCD_RAM=color; 
}	 

/*!
*@brief   Set the LCD display direction
*@parama  dir: 
               @arg 0:portrait screen

               @arg 1:landscape
*/
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)			//portrait screen
	{
		lcddev.dir=0;	
		lcddev.width=240;
		lcddev.height=320;

		lcddev.wramcmd=0X2C;
	 	lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  	 
			
	}else 			  	//landscape
	{	  				
		lcddev.dir=1;	
		lcddev.width=320;
		lcddev.height=240;

		lcddev.wramcmd=0X2C;
	 	lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  	 
		
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);	
}	 


/*!
*@brief   Set window
*@parama  sx: x starting coordinate
*@parama  sy: y starting coordinate
*@parama  width:   window  width
*@parama  height:  window  height
*/
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{    
	u16 twidth,theight;
	
	twidth=sx+width-1;
	theight=sy+height-1;

	LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA(sx>>8); 
	LCD_WR_DATA(sx&0XFF);	 
	LCD_WR_DATA(twidth>>8); 
	LCD_WR_DATA(twidth&0XFF);  
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA(sy>>8); 
	LCD_WR_DATA(sy&0XFF); 
	LCD_WR_DATA(theight>>8); 
	LCD_WR_DATA(theight&0XFF); 
	
}

/***********************************************************************
SMC_NCE4_2(PG11)  --------->  LCD_CS  
SMC_NWE(PD5)      --------->  LCD_WR
SMC_NOE(PD4)      --------->  LCD_RD
SMC_A10(PG0)      --------->  LCD_RS
RESET             --------->  LCD_RESET/PB5

LCD_D[0:15]:

SMC_D0(PD14)
SMC_D1(PD15)
SMC_D2(PD0)
SMC_D3(PD1)
SMC_D4(PE7)
SMC_D5(PE8)
SMC_D6(PE9)
SMC_D7(PE10)
SMC_D8(PE11)
SMC_D9(PE12)
SMC_D10(PE13)
SMC_D11(PE14)
SMC_D12(PE15)
SMC_D13(PD8)
SMC_D14(PD9)
SMC_D15(PD10)

PB0              --------->  Backlight
**********************************************************************/


void LCD_Backligh_Init(void)
{
	 GPIO_Config_T gpioConfig;
	
	 RCM_EnableAPB2PeriphClock(RCM_LCD_Backligh_PERIPH);
	
	 gpioConfig.pin=LCD_Backligh_PIN;
	 gpioConfig.mode=GPIO_MODE_OUT_PP;
	 gpioConfig.speed=GPIO_SPEED_50MHz;
	 GPIO_Config(LCD_Backligh_GPIO,&gpioConfig);
}


void LCD_HReset_Init(void)
{
	
	 GPIO_Config_T gpioConfig;
	
	 RCM_EnableAPB2PeriphClock(RCM_LCD_HReset_PERIPH);
	
	 gpioConfig.pin=LCD_HReset_PIN;
	 gpioConfig.mode=GPIO_MODE_OUT_PP;
	 gpioConfig.speed=GPIO_SPEED_50MHz;
	 GPIO_Config(LCD_HReset_GPIO,&gpioConfig);
	
}


void LCD_Init(void)
{
	
	GPIO_Config_T gpioConfig;
	SMC_NORSRAMConfig_T smcNORSRAMConfig;
	
	SMC_NORSRAMTimingConfig_T  readWriteTiming; 
	SMC_NORSRAMTimingConfig_T  writeTiming;
	
	RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_SMC);
	RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOD|RCM_APB2_PERIPH_GPIOE|RCM_APB2_PERIPH_GPIOG);
	
	
	gpioConfig.pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15;				
 	gpioConfig.mode=GPIO_MODE_AF_PP ;
	gpioConfig.speed=GPIO_SPEED_50MHz;
 	GPIO_Config(GPIOD, &gpioConfig); 
	
	gpioConfig.pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;				
 	gpioConfig.mode=GPIO_MODE_AF_PP ;
	gpioConfig.speed=GPIO_SPEED_50MHz;
 	GPIO_Config(GPIOE, &gpioConfig); 
	
	gpioConfig.pin = GPIO_PIN_0|GPIO_PIN_12;				
 	gpioConfig.mode=GPIO_MODE_AF_PP ;
	gpioConfig.speed=GPIO_SPEED_50MHz;
 	GPIO_Config(GPIOG, &gpioConfig); 
	
	readWriteTiming.addressSetupTime = 0x01;	 
  readWriteTiming.addressHodeTime = 0x00;	 
  readWriteTiming.dataSetupTime= 0x0f;		 
  readWriteTiming.busTurnaroundTime = 0x00;
  readWriteTiming.clockDivision = 0x00;
  readWriteTiming.dataLatency = 0x00;
  readWriteTiming.accessMode= SMC_ACCESS_MODE_A;	 
    

	writeTiming.addressSetupTime = 0x00;	 
  writeTiming.addressHodeTime= 0x00;	 		
  writeTiming.dataSetupTime = 0x03;		
  writeTiming.busTurnaroundTime = 0x00;
  writeTiming.clockDivision = 0x00;
  writeTiming.dataLatency = 0x00;
  writeTiming.accessMode = SMC_ACCESS_MODE_A;	 
	
	
	
	smcNORSRAMConfig.bank=SMC_BANK1_NORSRAM_4;
	smcNORSRAMConfig.dataAddressMux=SMC_DATA_ADDRESS_MUX_DISABLE;
	smcNORSRAMConfig.memoryType=SMC_MEMORY_TYPE_SRAM;
	smcNORSRAMConfig.memoryDataWidth=SMC_MEMORY_DATA_WIDTH_16BIT;
	smcNORSRAMConfig.burstAcceesMode=SMC_BURST_ACCESS_MODE_DISABLE;
	smcNORSRAMConfig.waitSignalPolarity=SMC_WAIT_SIGNAL_POLARITY_LOW;
	smcNORSRAMConfig.asynchronousWait=SMC_ASYNCHRONOUS_WAIT_DISABLE;
	smcNORSRAMConfig.wrapMode=SMC_WRAP_MODE_DISABLE;
	smcNORSRAMConfig.waitSignalActive=SMC_WAIT_SIGNAL_ACTIVE_BEFORE_WAIT_STATE;
	smcNORSRAMConfig.writeOperation=SMC_WRITE_OPERATION_ENABLE;
	smcNORSRAMConfig.waiteSignal=SMC_WAITE_SIGNAL_DISABLE;
	smcNORSRAMConfig.extendedMode=SMC_EXTENDEN_MODE_ENABLE;
	smcNORSRAMConfig.writeBurst= SMC_WRITE_BURST_DISABLE;
	smcNORSRAMConfig.readWriteTimingStruct=&readWriteTiming;
	smcNORSRAMConfig.writeTimingStruct=&writeTiming;
	SMC_ConfigNORSRAM(&smcNORSRAMConfig);
	
	SMC_EnableNORSRAM(SMC_BANK1_NORSRAM_4);
	
	delay_ms(100); 					
	
	LCD_WR_REG(0XD3);				   
	lcddev.id=LCD_RD_DATA();	  //dummy read 	
 	lcddev.id=LCD_RD_DATA();	  //read value 0X00
  lcddev.id=LCD_RD_DATA();   	//read value 0x93								   
 	lcddev.id<<=8;
	lcddev.id|=LCD_RD_DATA();  	//read value 0x41 
		
 
	LCD_WR_REG(0xCF);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0xC1); 
	LCD_WR_DATA(0X30); 
	LCD_WR_REG(0xED);  
	LCD_WR_DATA(0x64); 
	LCD_WR_DATA(0x03); 
	LCD_WR_DATA(0X12); 
	LCD_WR_DATA(0X81); 
	LCD_WR_REG(0xE8);  
	LCD_WR_DATA(0x85); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x7A); 
	LCD_WR_REG(0xCB);  
	LCD_WR_DATA(0x39); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x34); 
	LCD_WR_DATA(0x02); 
	LCD_WR_REG(0xF7);  
	LCD_WR_DATA(0x20); 
	LCD_WR_REG(0xEA);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0xC0);    //Power control 
	LCD_WR_DATA(0x1B);   //VRH[5:0] 
	LCD_WR_REG(0xC1);    //Power control 
	LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
	LCD_WR_REG(0xC5);    //VCM control 
	LCD_WR_DATA(0x30); 	 //3F
	LCD_WR_DATA(0x30); 	 //3C
	LCD_WR_REG(0xC7);    //VCM control2 
	LCD_WR_DATA(0XB7); 
	LCD_WR_REG(0x36);    // Memory Access Control 
	LCD_WR_DATA(0x48); 
	LCD_WR_REG(0x3A);   
	LCD_WR_DATA(0x55); 
	LCD_WR_REG(0xB1);   
	LCD_WR_DATA(0x00);   
	LCD_WR_DATA(0x1A); 
	LCD_WR_REG(0xB6);    // Display Function Control 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0xA2); 
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0x26);    //Gamma curve selected 
	LCD_WR_DATA(0x01); 
	LCD_WR_REG(0xE0);    //Set Gamma 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x2A); 
	LCD_WR_DATA(0x28); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x0E); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x54); 
	LCD_WR_DATA(0XA9); 
	LCD_WR_DATA(0x43); 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 		 
	LCD_WR_REG(0XE1);    //Set Gamma 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x15); 
	LCD_WR_DATA(0x17); 
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x11); 
	LCD_WR_DATA(0x06); 
	LCD_WR_DATA(0x2B); 
	LCD_WR_DATA(0x56); 
	LCD_WR_DATA(0x3C); 
	LCD_WR_DATA(0x05); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_REG(0x2B); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x3f);
	LCD_WR_REG(0x2A); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xef);	 
	LCD_WR_REG(0x11); //Exit Sleep
	delay_ms(120);
	LCD_WR_REG(0x29); //display on	
		
	LCD_Display_Dir(0);		
	LCD_Backligh_ON;				   
	LCD_Clear(WHITE);
}


/*!
*@brief   Screen clearing function
*@parama  color: fill color
*/
void LCD_Clear(u16 color)
{
	u32 index=0;      
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			
  LCD_SetCursor(0x00,0x0000);	    
	LCD_WriteRAM_Prepare();     		  
	for(index=0;index<totalpoint;index++)
	{
		LCD->LCD_RAM=color;	
	}
}  


/*!
*@brief   Fills the specified area with a single color
*@parama  sx: x starting coordinate
*@parama  sy: y starting coordinate
*@parama  ex: x diagonal coordinates
*@parama  ey: y diagonal coordinates
*@parama  color: fill colr
*/
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//设置光标位置 
		LCD_WriteRAM_Prepare();     //开始写入GRAM
		for(j=0;j<width;j++)LCD->LCD_RAM=color[i*width+j];//写入数据 
	}		  
}  


/*!
*@brief   drawing line
*@parama  x1: x starting coordinate
*@parama  y1: y starting coordinate
*@parama  x2: x end coordinates
*@parama  y2: y end coordinates
*/
 
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1;       //Calculate coordinate increment
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //Set the single step direction
	else if(delta_x==0)incx=0;//vertical line
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//horizontal line
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//drawing line
	{  
		LCD_DrawPoint(uRow,uCol);//drawing point
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}  

/*!
*@brief   Draw a rectangular
*@parama  x1: x starting coordinate
*@parama  y1: y starting coordinate
*@parama  x2: x diagonal coordinates
*@parama  y2: y diagonal coordinates
*/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}


/*!
*@brief   draw a circle
*@parama  x0: central point x coordinate
*@parama  y0: central point y  coordinate
*@parama  r: radius
*/

void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //judge next point position flag
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);           //7     	         
		a++;
		//use Bresenham algorithm to draw circle   
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 		

/*!
*@brief   Displays a character at the specified position
*@parama  x:  x starting coordinate
*@parama  y:  y starting  coordinate
*@parama  num: The character of display 12/16/24
*@parama  mode: 
                @arg 0:non overlapping mode

                @arg 1:overlapping mode
*/
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//Get the number of bytes occupied by a character corresponding to the font lattice set
 	num=num-' ';//the offset value
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	  //Call the 1206 font
		else if(size==16)temp=asc2_1608[num][t];	//Call the 1608 font
		else if(size==24)temp=asc2_2412[num][t];	//Call the 2412 font
		else return;								
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	
				break;
			}
		}  	 
	}  	    	   	 	  
}   


/*!
*@brief    m^n
*@retval   m^n
*/

u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			



/*!
*@brief   Display digits
*@parama  x:   x starting coordinate
*@parama  y:   y starting  coordinate
*@parama  len:  Number of digits
*@parama  size: font size
*@parama  color
*@parama  num: digits (0~4294967295)
*/

void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 


/*!
*@brief   Display digits
*@parama  x:   x starting coordinate
*@parama  y:   y starting  coordinate
*@parama  len:  Number of digits
*@parama  size: font size
*@parama  num: digits (0~999999999)
*@parama  mode:
               [7]:0,not be filled;1,fill 0.
               [6:1]:reserve
               [0]:0,Non overlapping display;1,overlapping display.
*/

void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 


/*!
*@brief   Display string
*@parama  x:   x starting coordinate
*@parama  y:   y starting  coordinate
*@parama  width
*@parama  height
*@parama  size: font size
*@parama  *p:String start address
*/
	  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//Determine whether the characters are illegal!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

