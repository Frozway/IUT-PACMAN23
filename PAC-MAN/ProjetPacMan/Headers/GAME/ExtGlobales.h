/**
  ******************************************************************************************************************
  * @file    ExtGlobales.h
  * @author  Colin DEMOUGE & Thibaut LEFRANCOIS
  * @version v1.0
  * @date    2023
  * @modification	
  * @brief
  ******************************************************************************************************************/

#ifndef __EXT_GLOBALES_H
#define __EXT_GLOBALES_H

#include "Board_GLCD.h"
#include "types.h"

extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;


extern volatile Character Pacman;
extern volatile Character nourriture;

extern int nbEnemy;
extern volatile Character Enemy[3];
#endif

