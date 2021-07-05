#include "constantes.h" // fichier contenant toutes les constantes du projet
#include <stdint.h>
#include <stdbool.h>


// mettez ici toutes les "extern" correspondant aux d�clarations contenues dans globadec.h

extern char chaine[30]; // buffer pour l'affichage sur le LCD
extern uint16_t touch_x, touch_y ;

extern int playerLeft_score;
extern int playerLeft_x;
extern int playerLeft_y;
extern int playerLeft_width;
extern int playerLeft_height;
extern int playerRight_score;
extern int playerRight_x;
extern int playerRight_y;
extern int playerRight_width;
extern int playerRight_height;
extern int playerRight_score;
extern uint8_t scoreLeftString[1];
extern uint8_t scoreRightString[1];

extern int flag_new_score;
extern int flag_move_ball;
extern int flag_movement;

extern int xBallPosition;
extern int yBallPosition;
extern int trajectoire;
extern int angle;
extern int waitChangeXBallPosition;
extern bool goToLeft;

extern uint8_t motTest[12];
