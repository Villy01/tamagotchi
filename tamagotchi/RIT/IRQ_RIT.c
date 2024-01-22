/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../timer/timer.h"
#include "../GLCD_advanced/GLCD.h" 
#include "../adc/adc.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


int RIT_menu=-1;
uint16_t duration=0;

void RIT_IRQHandler (void)
{					
	static int select=0;
	static int J_left = 0;
	static int J_right=0;	
	
	ADC_start_conversion();
	LPC_ADC->ADCR |=  (1<<24);
	
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
		
		J_left++;
		switch(J_left){
			case 1:
				if (eat == 0 && end == 0){
					if (cuddle == 0){
						sound=1;
					}
					LCD_MenuRestore(1);
					LCD_MenuSelect(0);
					RIT_menu=0;
				}
				break;
			default:
				break;
		}
	}
	else{
			J_left=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
		
		J_right++;
		switch(J_right){
			case 1:
				if (eat == 0 && end == 0){
					if (cuddle == 0){
						sound=2;
					}
					LCD_MenuRestore(0);
					LCD_MenuSelect(1);
					RIT_menu=1;
				}
				break;
			default:
				break;
		}
	}
	else{
			J_right=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	
		
		select++;
		switch(select){
			case 1:
				if (eat == 0 && RIT_menu != -1 && end == 0 && cuddle == 0){
					eat=1;
					if (RIT_menu == 0){
						if (satiety != MaxBar){
							menuFlag = RIT_menu;
							LCD_EatMeal(Xpos, Ypos, 0);
							enable_timer(2);
							RIT_menu=-1;
						} else{
							eat=0;
						}
					} else
					{
						if (happiness != MaxBar){
							menuFlag = RIT_menu;
							LCD_EatSnack(Xpos, Ypos, 0);
							enable_timer(2);
							RIT_menu=-1;
						} else{
							eat=0;
						}
					}
				}
				if (end == 1 && endAnimation == 0){
					RIT_menu=-1;
					GameReset();
				}
				break;
			default:
				break;
		}
	}
	else{
			select=0;
	}
	
	switch(sound){
		case 0:
			break;
		case 1:
			Sound(14);
			duration++;
			if (duration == 2){
				duration=0;
				sound=0;
				StopSound();
			}
			break;
		case 2:
			Sound(12);
			duration++;
			if (duration == 2){
				duration=0;
				sound=0;
				StopSound();
			}
			break;
		case 3:
			Sound(4);
			duration++;
			if (duration == 5){
				sound++;
				duration=0;
			}
			break;
		case 4:
			Sound(6);
			duration++;
			if (duration == 5){
				sound++;
				duration=0;
			}
			break;
		case 5:
			Sound(10);
			duration++;
			if (duration == 5){
				sound++;
				duration=0;
			}
			break;
		case 6:
			Sound(6);
			duration++;
			if (duration == 5){
				sound++;
				duration=0;
			}
			break;
		case 7:
			Sound(10);
			duration++;
			if (duration == 5){
				sound++;
				duration=0;
			}
			break;
		case 8:
			Sound(12);
			duration++;
			if (duration == 5){
				sound++;
				duration=0;
			}
			break;
		case 9:
			Sound(10);
			duration++;
			if (duration == 5){
				sound++;
				duration=0;
			}
			break;
		case 10:
			Sound(14);
			duration++;
			if (duration == 5){
				duration=0;
				sound=0;
				StopSound();
			}
			break;
		default:
			break;
	}
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
