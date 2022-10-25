#ifndef __LCD_H
#define __LCD_H		

#include "apm32e10x.h"

//LCD Reset

#define LCD_HReset_GPIO          GPIOB
#define LCD_HReset_PIN           GPIO_PIN_5
#define RCM_LCD_HReset_PERIPH    RCM_APB2_PERIPH_GPIOB

#define	LCD_HReset_Hight  GPIO_SetBit(LCD_HReset_GPIO,LCD_HReset_PIN);
#define	LCD_HReset_Low    GPIO_ResetBit(LCD_HReset_GPIO,LCD_HReset_PIN);  

//LCD backligh  
#define LCD_Backligh_GPIO          GPIOB
#define LCD_Backligh_PIN           GPIO_PIN_0
#define RCM_LCD_Backligh_PERIPH    RCM_APB2_PERIPH_GPIOB

#define	LCD_Backligh_ON   GPIO_SetBit(LCD_Backligh_GPIO,LCD_Backligh_PIN);   

typedef struct  
{										    
	u16 width;			
	u16 height;			
	u16 id;				  
	u8  dir;			  //0£¬portrait screen£»1£¬landscape¡£	
	u16	wramcmd;		//gram command
	u16  setxcmd;		//Set the x coordinate
	u16  setycmd;		//Set the y coordinate
}_lcd_dev; 	


extern _lcd_dev lcddev;	

typedef struct
{
	vu16 LCD_REG;  //command 
	vu16 LCD_RAM;  //data
} LCD_TypeDef;

//NOR/SRAM Bank1.sector4,HADDR[27,26]=11 A10			  

#define LCD_BASE        ((u32)(0x6C000000 | 0x000007FE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)


extern _lcd_dev lcddev;
   
extern u16  POINT_COLOR;   
extern u16  BACK_COLOR; 

//scan direction definition 
#define L2R_U2D  0 //Left to right, top to bottom
#define L2R_D2U  1 //Left to right, bottom to top
#define R2L_U2D  2 //Right to Left, top to bottom
#define R2L_D2U  3 //Right to Left, bottom to top

#define U2D_L2R  4 //Top to bottom,left to right
#define U2D_R2L  5 //Top to bottom,right to Left
#define D2U_L2R  6 //Bottom to top,left to right
#define D2U_R2L  7 //Bottom to top,right to Left

#define DFT_SCAN_DIR  L2R_U2D  


#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	   0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 
#define BRRED 			     0XFC07 
#define GRAY  			     0X8430 


#define DARKBLUE      	 0X01CF	
#define LIGHTBLUE      	 0X7D7C	
#define GRAYBLUE       	 0X5458 

 
#define LIGHTGREEN     	 0X841F 
//#define LIGHTGRAY        0XEF5B 
#define LGRAY 			     0XC618 

#define LGRAYBLUE        0XA651 
#define LBBLUE           0X2B12 
	  
void LCD_Backligh_Init(void);
void LCD_HReset_Init(void);

void LCD_Init(void);													   	
void LCD_DisplayOn(void);													
void LCD_DisplayOff(void);													
void LCD_Clear(u16 Color);	 												
void LCD_SetCursor(u16 Xpos, u16 Ypos);										
void LCD_DrawPoint(u16 x,u16 y);											
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);								
u16  LCD_ReadPoint(u16 x,u16 y); 											
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);						 			
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);						
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   				
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);				
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		

void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
u16 LCD_ReadReg(u16 LCD_Reg);
void LCD_WriteRAM_Prepare(void);

void LCD_Scan_Dir(u8 dir);									
void LCD_Display_Dir(u8 dir);								
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);		   						   																			 


#endif  

