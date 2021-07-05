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
* Timer 1 interruption. Timer 1 handle the movement of players (computing trajectory and moving players)/
*/
void TIMER1_IRQHandler() {
	flag_movement = 1;
	TIM_ClearIntPending(LPC_TIM1, TIM_MR1_INT);
}

/**
* Timer 1 setup.
*/
void timer1_setup() {
	TIM_MATCHCFG_Type configMatch;
	NVIC_EnableIRQ(TIMER1_IRQn); //allow interruption for timer 1

	LPC_TIM1->PR = 175000; // 1/25Mhz * 175000 = 7ms
	configMatch.MatchChannel = 1; //match register 1 for timer 1
	configMatch.IntOnMatch = ENABLE; // interruption on match
	configMatch.StopOnMatch = DISABLE; // don't stop the timer on match
	configMatch.ResetOnMatch = ENABLE; // reset timer to 0 on match
	configMatch.ExtMatchOutputType = TIM_EXTMATCH_TOGGLE; 
	configMatch.MatchValue = 1; // 1*7ms = 7ms, match each 7 seconds (~3x less faster than the ball)
	TIM_ConfigMatch(LPC_TIM1, &configMatch);
}