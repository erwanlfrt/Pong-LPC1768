//===========================================================//
//                          PONG GAME             					 //
//===========================================================//

#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_libcfg_default.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_i2c.h"
#include "touch\ili_lcd_general.h"
#include "touch\lcd_api.h"
#include "touch\touch_panel.h"
#include "affichagelcd.h"
#include "globaldec.h" 
#include "pong/pong.h"
#include "timer0.h"
#include "timer1.h"
#include "memory.h"


/**
* Configure pin connect block.
*/
void pin_Configuration(void) {
	PINSEL_CFG_Type type;
	type.Portnum = PINSEL_PORT_0;
	type.Pinnum = PINSEL_PIN_27;
	type.Funcnum = PINSEL_FUNC_1;
	type.Pinmode = PINSEL_PINMODE_PULLUP;
	type.OpenDrain = PINSEL_PINMODE_NORMAL;
	PINSEL_ConfigPin(&type);
	type.Pinnum = PINSEL_PIN_28;
	PINSEL_ConfigPin(&type);
	type.Portnum = PINSEL_PORT_2;
	type.Pinnum = PINSEL_PIN_10;
	PINSEL_ConfigPin(&type);
	type.Pinnum = PINSEL_PIN_11;
	PINSEL_ConfigPin(&type);
}

/**
* Main function
*/
int main(void)
{	  
		touch_init(); // init touch screen
		pin_Configuration(); //configure pin select block
	  lcd_Initialization(); // init pinsel screen
		
		init_i2c_eeprom(); // init memory
	
		
		initGame(); // init the game

		timer0_setup(); // Init timer 0 (movement of the ball)
		timer1_setup(); // Init timer 1 (movement of players)
	
		TIM_Cmd(LPC_TIM0, ENABLE); // start timer 0
		TIM_Cmd(LPC_TIM1, ENABLE); // start timer 1

		
		
    while(1) {
			runGame(); //run game until a player reaches 5 points.
      //operations with tempo (triggered by a flag)
			if(flag_new_score == 1) {
				writeScoreInMemory();
				flag_new_score = 0;
			}
			if(flag_move_ball == 1) {
				moveBall();
				flag_move_ball = 0;
			}
			if(flag_movement == 1) {
				computeTrajectory();
				computeMovement();
				flag_movement = 0;
			} 
		}
} 

//---------------------------------------------------------------------------------------------	
#ifdef  DEBUG
void check_failed(uint8_t *file, uint32_t line) {while(1);}
#endif
