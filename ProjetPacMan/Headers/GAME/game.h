/**
  ******************************************************************************************************************
  * @file    game.h 
  * @author  IUT Informatique La Rochelle
  * @version v1.0
  * @date    2023
  * @modification	
  * @brief   Gestion du jeu
  ******************************************************************************************************************/
	
#ifndef __GAME_H
#define __GAME_H

#include "stdbool.h"
#include "types.h"
	
#define HEIGHT_BOARD	(GLCD_HEIGHT-10)

bool gameLoop(void); //Si return true : game finit
void update(volatile Character * character) ;

void dessinePlateau(void);
void startLevel(void);
void stopLevel(void);

#endif
