/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "adc.h"
#include "../led/led.h"
#include "../timer/timer.h"
#include "../GLCD_advanced/GLCD.h" 

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short AD_current;   
unsigned short AD_last = 0xFF;     /* Last converted value               */
uint16_t converted_value;

void ADC_IRQHandler(void) {
  	
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF); //Read Conversion Result             
  if(AD_current != AD_last){
		converted_value = ((AD_current*6)/(0xFFF));
		switch(converted_value){
			case 0:
				LCD_ClearSpeaker1();
				LCD_ClearSpeaker2();
				LCD_ClearSpeaker3();
				volume=0;
				break;
			case 1:
				LCD_Speaker1();
				LCD_ClearSpeaker2();
				LCD_ClearSpeaker3();
				volume=0.5;
				break;
			case 2:
				LCD_Speaker1();
				LCD_ClearSpeaker2();
				LCD_ClearSpeaker3();
				volume=0.5;
				break;
			case 3:
				LCD_Speaker1();
				LCD_Speaker2();
				LCD_ClearSpeaker3();
				volume=1;
				break;
			case 4:
				LCD_Speaker1();
				LCD_Speaker2();
				LCD_ClearSpeaker3();
				volume=1;
				break;
			case 5:
				LCD_Speaker1();
				LCD_Speaker2();
				LCD_Speaker3();
				volume=1.5;
				break;
			case 6:
				LCD_Speaker1();
				LCD_Speaker2();
				LCD_Speaker3();
				volume=1.5;
				break;
			default:
				break;
		}
		AD_last = AD_current;
  }
}
