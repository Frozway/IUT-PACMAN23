/**
  ******************************************************************************************************************
  * @file    game.h
  * @author  Colin DEMOUGE & Thibaut LEFRANCOIS
  * @version v1.0
  * @date    2023
  * @modification	
  * @brief
  ******************************************************************************************************************/
	
#ifndef __GAME_H
#define __GAME_H

#include "stdbool.h"
#include "types.h"
	
	
#define HEIGHT_BOARD	(GLCD_HEIGHT-10)

#define nbPointsForWin	3

#define posScorePacman	80
#define posScoreEnemy		(GLCD_WIDTH-70)

#define posScorePacman	80
#define posScoreEnemy		(GLCD_WIDTH-70)

void gameLoop(void); //Si return true : game finit

void update(volatile Character * character) ;
void newFood( volatile Character * aFood);
void newEnemy(void);
void dessinePlateau(void);
void startLevel(void);
void stopLevel(void);
void directionEnemy(volatile Character * anEnemy);
void moveEnemy(volatile Character * anEnemy);
void moveAllEnemy(void);
void displayPacmanScore(unsigned int abscisse, unsigned int nbNourriture);
void displayTimer(void);
void delay( unsigned long duration);

unsigned int collision(volatile Character * character1, volatile Character * character2);
unsigned int collisionEnemy(volatile Character * character);

#endif
