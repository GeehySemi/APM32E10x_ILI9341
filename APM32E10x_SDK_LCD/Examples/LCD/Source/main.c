/*!
 * @file        main.c
 *
 * @brief       Main program body
 *
 * @version     V1.0.1
 *
 * @date        2022-07-29
 *
 * @attention
 *
 *  Copyright (C) 2021-2022 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be usefull and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */

/* Includes */
#include "main.h"
#include "delay.h"
#include "lcd.h"
#include "stdio.h"
/** @addtogroup Examples
  @{
  */

/** @addtogroup Template
  @{
  */

/** @defgroup Template_Functions
  @{
  */

/*!
 * @brief       Main program
 *
 * @param       None
 *
 * @retval      None
 *
 * @note       
 */
int main(void)
{
	  u8 x=0;
	  u8 lcd_id[12];
	  delay_init();
	  LCD_HReset_Init();
	
	  LCD_HReset_Low;
	  delay_ms(200);
	  LCD_HReset_Hight;
	
	  LCD_Backligh_Init();
	  LCD_Init();
	  sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);
    while(1)
   {
			switch(x)
			{
				case 0:LCD_Clear(WHITE);break;
				case 1:LCD_Clear(BLACK);break;
				case 2:LCD_Clear(BLUE);break;
				case 3:LCD_Clear(RED);break;
				case 4:LCD_Clear(MAGENTA);break;
				case 5:LCD_Clear(GREEN);break;
				case 6:LCD_Clear(CYAN);break;

				case 7:LCD_Clear(YELLOW);break;
				case 8:LCD_Clear(BRRED);break;
				case 9:LCD_Clear(GRAY);break;
				case 10:LCD_Clear(LGRAY);break;
				case 11:LCD_Clear(BROWN);break;
			}
			POINT_COLOR=RED;	  
			
			LCD_ShowString(30,40,210,24,24,"GEEHY ^_^"); 
			LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
			LCD_ShowString(30,90,200,16,16,"APM32E103");
			LCD_ShowString(30,110,200,16,16,lcd_id);		//œ‘ æLCD ID
			LCD_ShowString(30,130,200,12,12,"2022/10/10");	      	
				x++;
			if(x==12)x=0;		   		 
			delay_ms(1000);	

    }
}

/**@} end of group Template_Functions */
/**@} end of group Template */
/**@} end of group Examples */
