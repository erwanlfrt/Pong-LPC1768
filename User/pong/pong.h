#include <stdint.h>
#include <stdbool.h>

#ifndef PONG_H
#define PONG_H

  int xBallPosition;
  int yBallPosition;
  int trajectoire;
  int angle;
  int waitChangeXBallPosition;
  bool goToLeft = false;

	int playerLeft_score;
  int playerLeft_x;
  int playerLeft_y;
  int playerLeft_width;
  int playerLeft_height;
  int playerRight_score;
  int playerRight_x;
  int playerRight_y;
  int playerRight_width;
  int playerRight_height;
  uint8_t scoreLeftString[2];
  uint8_t scoreRightString[2];
	
	int flag_new_score;
	int flag_move_ball;
	int flag_movement;

	
	void drawMiddleLine(void);
  int gameOver(void);
  void writeScore(void);
  void computeTilt(void);
  void compute_angle(void);
  void reinitBall(void);
  void movePlayer(void);
  void computeMovement(void);
	void initGame();
	void moveBall(void);
	void computeTrajectory(void);
	void runGame(void);
	void writeScoreInMemory(void);
  
#endif

