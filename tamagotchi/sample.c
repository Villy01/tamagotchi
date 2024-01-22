/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control led through EINT buttons and manage the bouncing effect
 *        	- key1 switches on the led at the left of the current led on, 
 *					- it implements a circular led effect. 	
  * Note(s): this version supports the LANDTIGER Emulator
 * Author: 	Paolo BERNARDI - PoliTO - last modified 15/12/2020
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <stdio.h>
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "led/led.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "GLCD_advanced/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "adc/adc.h"

/* Led external variables from funct_led */
extern unsigned char led_value;					/* defined in funct_led								*/
#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
	
	SystemInit();  												/* System Initialization (i.e., PLL)  */
	LCD_Initialization();
	TP_Init();
	TouchPanel_Calibrate();
	LCD_Clear(White);
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       */
	ADC_init();
	
	
	init_timer(0, 0, 0, 3, 0x00BEBC20);				/* 0,5 secondi			*/
	init_timer(1, 0, 0, 3, 0x000000C8);				/*	8 microsecondi			*/
	init_timer(2, 0, 0, 3, 0x0016E360);				/*	0,06 secondi		*/	
	disable_timer(2);
	
	enable_RIT();												/* enable RIT to count 50ms				 */
	
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	
	GameReset();
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);	
	
	
  while (1) {                           /* Loop forever                       */	
//		__ASM("wfi");
  }

}
