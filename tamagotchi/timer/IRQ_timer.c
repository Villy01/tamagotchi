/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../led/led.h"
#include "../GLCD_advanced/GLCD.h" 
#include "../RIT/RIT.h"
#include "../TouchPanel/TouchPanel.h"
#include "../adc/adc.h"
/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern unsigned char led_value;					/* defined in funct_led								*/

/*uint16_t SinTable[45] =                                      
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694,
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};*/

uint16_t SinTable[32] = {512, 612, 708, 799, 873, 937, 984, 1013, 1023, 1013, 984, 937, 873, 796, 708, 612, 512,
		412, 316, 228, 151, 87, 40, 11, 1, 11, 40, 87, 151, 228, 316, 412};



uint16_t timer0_flag = 0, eat=0, end=0, menuFlag, endAnimation=0, cuddle=0;
uint16_t Xpos = 120, Ypos = 170, eatCounter=1, sound=0;
uint16_t h=0, m=0, s=0;
float volume;
const int freqs[15]={4240,3779,3367,3175,2834,2525,2249,2120,1890,1684,1592,1417,1263,1125,1062};

void TIMER0_IRQHandler (void)
{
	uint16_t c;
	if ((timer0_flag%2) == 0)
	{
		if (eat == 0 && end == 0 && cuddle == 0){
			LCD_DrawAnimation(Xpos, Ypos);
		}
		if (cuddle > 0){
			cuddle++;
			if (cuddle == 6) {
				cuddle=0;
				LCD_ClearCuddle();
			} else {
			LCD_CuddleAnimation1();
			}
		}
	} else{
		UpdateTime();
		LCD_Age(h, m, s);
		if (eat == 0 && end == 0 && cuddle == 0){
			LCD_EndAnimation(Xpos, Ypos);
		}
		if (cuddle > 0){
			cuddle++;
			if (cuddle == 6) {
				cuddle=0;
				LCD_ClearCuddle();
			} else {
			LCD_CuddleAnimation2();
			}
		}
	}
	timer0_flag++;
	if (timer0_flag == 10)
	{
		timer0_flag=0;
		c=LCD_HappinessDec();
		if (c == 0)
		{
			EndGame();
		}
		if (end == 0){
			c=LCD_SatietyDec();
			if (c == 0)
			{
				EndGame();
			}
		}
	}
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	if(getDisplayPoint(&display, Read_Ads7846(), &matrix)){
		if(display.x > 70 && display.x < 170 && display.y > 120 && display.y < 210){
			if (eat == 0 && end == 0 && cuddle == 0 && happiness < MaxBar){
				cuddle=1;
				sound=3;
				LCD_CuddleAnimation1();
				LCD_HappinessUp();
			}
		}
	}
	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER2_IRQHandler (void)
{
	if (end == 0)
	{
		if (menuFlag == 0)
		{
			LCD_EatMeal(Xpos, Ypos, eatCounter);
		} else
		{
			LCD_EatSnack(Xpos, Ypos, eatCounter);
		}
		eatCounter++;
		if (eatCounter == 21)
		{
			eatCounter=1;
			disable_timer(2);
			eat=0;
		}
	} else
	{
		LCD_TamagotchiLeave(Xpos, Ypos, eatCounter);
		eatCounter++;
		if (eatCounter == 19)
		{
			eatCounter=1;
			disable_timer(2);
			endAnimation=0;
		}
	}
	LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}


void TIMER3_IRQHandler (void)
{
	static int ticks=0;
	/* DAC management */	
	LPC_DAC->DACR = (SinTable[ticks]<<6)*volume;
	ticks++;
	if(ticks==32) ticks=0;

	
  LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}

void UpdateTime(void)
{
	if (s == 59){
		s=0;
		if (m == 59){
			m=0;
			h++;
		} else{
			m++;
		}
	} else{
		s++;
	}
}

void EndGame(void)
{
	if (eat == 1){
		disable_timer(2);
		eatCounter=1;
		eat=0;
		LCD_ExcepClear();
	}
	if (cuddle > 0){
		cuddle=0;
		LCD_ExcepClear();
	}
	endAnimation=1;
	end=1;
	LCD_TamagotchiLeave(Xpos, Ypos, 0);
	enable_timer(2);
	disable_timer(0);
	disable_timer(1);
	LCD_ShowRestart();
}

void GameReset(void)
{
	LCD_StatusReset();
	LCD_ClearRestart();
	timer0_flag = 0;
	eat=0;
	end=0;
	Xpos = 120;
	Ypos = 170;
	h=0;
	m=0;
	s=0;
	LCD_DrawTamagotchi(Xpos, Ypos);
	LCD_ShowMenu();
	LCD_Speaker();
	LCD_Age(h, m, s);
	LCD_ShowStatus();
	LCD_StatusBarInit();
	ADC_init();
	enable_timer(0);
	enable_timer(1);
}

void Sound(uint16_t n)
{
	disable_timer(3);
	reset_timer(3);
	init_timer(3, 0, 0, 3, freqs[n]);
	enable_timer(3);
}

void StopSound(void)
{
	disable_timer(3);
	reset_timer(3);
}

/******************************************************************************
**                            End Of File
******************************************************************************/
