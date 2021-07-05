#include <math.h>
#include <stdlib.h>
#include "global.h"
#include "lpc17xx.h"
#include "lpc17xx_timer.h"
#include "touch\ili_lcd_general.h"
#include "touch\lcd_api.h"
#include "affichagelcd.h"
#include "touch\touch_panel.h"

/**
* Timer 0 interruption. Timer 0 handle the movement of the ball.
*/
void TIMER0_IRQHandler(void)
{
	flag_move_ball = 1; //trigger a flag to move the ball, the main's while(1) makes all the calculations.
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT); //refresh timer 0
}

/**
* Setup the timer
*/
void timer0_setup(){
	TIM_MATCHCFG_Type configMatch;
	NVIC_EnableIRQ(TIMER0_IRQn); // allows interruption for timer 0
	
	LPC_TIM0->PR = 65000; // 1/25Mhz * 65000 = 2.6 ms
	configMatch.MatchChannel = 0; // match register 0 for timer 0
	configMatch.IntOnMatch = ENABLE; // interruption when there is a match
	configMatch.StopOnMatch = DISABLE; // don't stop the timer when there is a match
	configMatch.ResetOnMatch = ENABLE; // reset the timer to 0 on match
	configMatch.ExtMatchOutputType = TIM_EXTMATCH_TOGGLE; 
	configMatch.MatchValue = 1; // 1*2.6ms = 2.6 ms (very fast in order to get a fluent ball movement) 
	
	TIM_ConfigMatch(LPC_TIM0, &configMatch);
}