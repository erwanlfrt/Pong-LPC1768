#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "global.h"
#include "affichagelcd.h"
#include "constantes.h"
#include "touch\ili_lcd_general.h"
#include "touch\lcd_api.h"


/**
* draw the middle line when the ball flies over.
*/
void drawMiddleLine(void) {
	int x;
	for(x = 19; x < 225; x = x +13) {
			dessiner_rect(x,159,8,2,0,1, White, White);
	}
}

/**
* @return 1 if player 1 has won, 2 if player 2 has won or 0 if no one has won.
*/
int gameOver(void) {
	if(playerLeft_score == 5){
		return 1;
	}
	else if(playerRight_score == 5) {
		return 2;
	}
	else {
		return 0;
	}
}

/**
* write score on the screen.
*/
void writeScore(void) {
	char scoreLeft[16];
	char scoreRight[16];
	snprintf(scoreLeft, sizeof(scoreLeft), "%d", playerLeft_score); //store the playerLeft_score into scoreLeft in string format
  snprintf(scoreRight, sizeof(scoreRight), "%d", playerRight_score); //store the playerRight_score into scoreRight in string format
	LCD_write_english_string(2,0,scoreLeft,Black, White); //write scoreLeft on screen
	LCD_write_english_string(228,303, scoreRight, Black, White); //write scoreRight on screen
}

/**
* read score from memory.
*/
void readScore(void) {
  //read scores from memory
	i2c_eeprom_read(1020,&scoreLeftString[0], sizeof(scoreLeftString));
	i2c_eeprom_read(1040,scoreRightString, sizeof(scoreRightString));
	
  //if one of the scores is equal to 5 then we reset the scores.
	if(scoreLeftString[0] == '5' || scoreRightString[0] == '5') {
		playerLeft_score = 0;
		playerRight_score = 0;
	}
	else{
		if(scoreLeftString[0] == 0) {
			playerLeft_score = 0; // if nothing is stored in memory then the default score is 0
		}
		else {
			sscanf((char*) scoreLeftString, "%d", &playerLeft_score); //if a score is stored in memory then we save it into playerLeft_score
		}
    //same as above.
		if(scoreRightString[0] == 0) { //pas de score
			playerRight_score = 0;
		}
		else {
			sscanf((char*) scoreRightString, "%d", &playerRight_score);
		}
	}
}

/**
* Compute tilt of the trajectory. Because of the lack of pixels (only 240px for height), The biggest angle (45� deg) or (135� deg) already implies a vertical movement of 1px. 
* computeTilt allows to represent angles between 45� deg and 135� deg by computing how many pixels it would takes to represent the Tilt.
* The tilt value is calculated like this : tilt = 1 / cos(angle*(PI/180)) with angle in degrees.
* So an angle of 45� would returns 1.41 pixel for the tilt but we only keep the rounded value.
* representation of an angle of 45�:
* .
*  .
*   .
*    . (movement of 1px each 1 update)
* But for an angle of 80�, the tilt would be approximately of 5 px.
* representation of an angle of 80�:
* .....
*      .....
*           ..... (movement of 1 px each 5 updates)
*/
void computeTilt() {
	waitChangeXBallPosition = 1 / cos(angle*(PI/180));
	waitChangeXBallPosition = abs(waitChangeXBallPosition);
}

/**
* compute the new angle after collision.
*/
void compute_angle() {
	angle = 90 - (angle-90);
}
/**
* reininitialize the position of the ball in the middle of the screen with a random position, a random direction and a random angle.
*/
void reinitBall() {
	angle = 45 + rand()%90;
	xBallPosition = 16 + rand()%207;
	goToLeft = rand() & 1;
	if(goToLeft) {
		yBallPosition = 160 + rand()%10;
	}
	else {
		yBallPosition = 160 - rand()%10;
	}
	goToLeft = false;
}
/**
* move a player.
* @param player (int) - 1 for left player, 2 for right player.
* @param x (int) - new x position of the player
*/
void movePlayer(int player, int x){
	if(x > 15 && x + playerLeft_height < 224 ) {	
		if(player == 1) {
			dessiner_rect(playerLeft_x,playerLeft_y,playerLeft_height,playerLeft_width,0,1,Black,Black); //player 1 (left)
			playerLeft_x = x;
			dessiner_rect(playerLeft_x,playerLeft_y,playerLeft_height,playerLeft_width,0,1,White,White);
		}
		else {
			dessiner_rect(playerRight_x,playerRight_y,playerRight_height,playerRight_width,0,1,Black,Black); //player 2 (right) 
			playerRight_x = x;
			dessiner_rect(playerRight_x,playerRight_y,playerRight_height,playerRight_width,0,1,White,White);
		}
	}
}

/**
* compute the movement of a player in order to line up with the trajectory of the ball.
*/
void computeMovement() {
	if(goToLeft) {
		if(trajectoire > playerLeft_x+(playerLeft_height/2)) {
			movePlayer(1, playerLeft_x+1); //move the player upward if the predicted trajectory is above the player 
		}
		else if( trajectoire < playerRight_x) {
			movePlayer(1, playerLeft_x-1); // move the player downward if the predicted trajectory is under the player
		}
	}
	else {
    //same as above but for right player
		if(trajectoire > playerRight_x+(playerRight_height/2)) {
			movePlayer(2, playerRight_x+1);
		}
		else if( trajectoire < playerRight_x) {
			movePlayer(2, playerRight_x-1);
		}
	}
}

/**
* initialize the game (players positions, draw the area, init ball, ...).
*/
void initGame() {
	flag_new_score = 0;
	readScore();
	writeScoreInMemory();
	
	dessiner_rect(0,0,240,320,0,1,Black,Black); //background
	dessiner_rect(0,0,15,320,0,1,White,White); //lower border
	dessiner_rect(224,0,15,320,0,1,White, White); //upper border

	//init players
	playerLeft_width = 8;
	playerLeft_height = 30;
	playerLeft_x = 105;
	playerLeft_y = 5;
	
	playerRight_width = 8;
	playerRight_height = 30;
	playerRight_x = 105;
	playerRight_y = 307;

	dessiner_rect(playerLeft_x,playerLeft_y,playerLeft_height,playerLeft_width,0,1,White,White); //player 1 (left)
	dessiner_rect(playerRight_x,playerRight_y,playerRight_height,playerRight_width,0,1,White,White); //player 2 (right) 
	
	writeScore();
	drawMiddleLine();
	reinitBall(); 
	computeTilt();
}

/**
* Move the ball according to its trajectory.
*/
void moveBall() {
	dessiner_rect(xBallPosition,yBallPosition,5,5,0,1,Black,Black); //ball

	if(goToLeft) {
		if(yBallPosition - ceil(sin(angle*(PI/180))) <= playerLeft_y + playerLeft_width && xBallPosition < playerLeft_x+playerLeft_height && xBallPosition + 5 > playerLeft_x ) {
			//collision if predicted trajectory touch the player
			goToLeft = false; //reverse the direction of the ball
			yBallPosition = yBallPosition + ceil(sin(angle*(PI/180))); //new y position of the ball
			movePlayer(1,playerLeft_x); //redraw player
		}
		else if(yBallPosition - ceil(sin(angle*(PI/180))) > 0) {
			yBallPosition = yBallPosition - ceil(sin(angle*(PI/180))); //if no collision, keep the same direction and move the ball
		}
		else {
      //player missed the ball
			playerRight_score++; 
			flag_new_score = 1; //trigger the flag "new score" to write the new score in memory
			reinitBall();
			writeScore();
		}
	}
	else {
    //same as above but for right player
		if(yBallPosition + ceil(sin(angle*(PI/180))) >= playerRight_y - 5 && xBallPosition < playerRight_x+playerRight_height && xBallPosition + 5 > playerRight_x ) {
			goToLeft = true;
			yBallPosition = yBallPosition - sin(angle*(PI/180));
			movePlayer(2,playerRight_x); //redraw player
		}
		else if(yBallPosition + sin(angle*(PI/180)) <315) {
			yBallPosition = yBallPosition + ceil(sin(angle*(PI/180)));
		}
		else {
			goToLeft = true;
			playerLeft_score++;
			flag_new_score = 1;
			reinitBall();
			writeScore();
		}
	}
	if(waitChangeXBallPosition == 0) {
    //honor the tilt
		if( cos( angle*(PI/180)) > 0) { //if angle < 90deg
			if(xBallPosition < 218) {
				xBallPosition++; //move the ball if no collision with the border
			}
			else { //collision with the upper border
				xBallPosition--;
				compute_angle();
			}
		}
		else { //if angle > 90deg
			if(xBallPosition > 16) {
				xBallPosition--;
			}
			else { //collision with the lower border
				xBallPosition++;
				compute_angle();
			}
		}
		computeTilt();
	}
	waitChangeXBallPosition--;

	//if the ball go over the middle line, we have to redraw it 
	if(yBallPosition == 153 || yBallPosition == 162) {
		drawMiddleLine();
	}
	dessiner_rect(xBallPosition,yBallPosition,5,5,0,1,White, White); //ball
}

/**
* Compute the trajectory of the ball according to his y position and angle.
* Trajectory is basically predicted thanks to the angle and the position of the ball.
*                                             _________________________       a = angle of the ball (in deg)
*                                            / ( (y - yb) )²           \      y = y inner position of the player
*    Predicted Trajectory =   xb (+/-)    \ /  ( -------- )  - (y-yb)²       xb = x positon of the ball
*                                          v   (  sin(a)  )                  yb = y position of the ball
*/
void computeTrajectory(void) {
	if(!goToLeft) {
		if(angle < 90) {
			trajectoire = xBallPosition + sqrt(pow(((307-yBallPosition)/sin(angle*(PI/180))),2) - pow((307-yBallPosition),2));
		}
		else {
			trajectoire =  xBallPosition - sqrt(pow(((307-yBallPosition)/sin(angle*(PI/180))),2) - pow((307-yBallPosition),2));
		}
	}
	else {
		if(angle < 90) {
			trajectoire = xBallPosition + sqrt(pow(((14-yBallPosition)/sin(angle*(PI/180))),2) - pow((14-yBallPosition),2));
		}
		else {
			trajectoire =  xBallPosition - sqrt(pow(((14-yBallPosition)/sin(angle*(PI/180))),2) - pow((14-yBallPosition),2));
		}
	}
}

/**
* run the game.
*/
void runGame() {
	if(gameOver() == 1 || gameOver() == 2) {
		if(gameOver() == 1) {
			LCD_write_english_string(80,152, "left wins", White,Black);
		}
		else {
			LCD_write_english_string(80,152, "right wins", White,Black);
		}
		
		TIM_Cmd(LPC_TIM0, DISABLE); //disable timer 0 (no ball movement)
		TIM_Cmd(LPC_TIM1, DISABLE); //disable timer 1 (no players movement)
	}
}

/**
* write the score in memory.
*/
writeScoreInMemory(void) {
  //string format of both scores.
	sprintf(scoreLeftString, "%d", playerLeft_score); 
	sprintf(scoreRightString, "%d", playerRight_score);

  //write scores at the addresses : 1020 and 1040
	i2c_eeprom_write(1040, &scoreRightString[0], sizeof(scoreRightString)+1);
	i2c_eeprom_write(1020, &scoreLeftString[0], sizeof(scoreLeftString)+1);
}