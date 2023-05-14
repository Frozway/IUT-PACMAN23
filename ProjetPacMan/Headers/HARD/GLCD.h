/**
  ******************************************************************************************************************
  * @file    GLCD.h 
  * @author  IUT Informatique La Rochelle
  * @version v1.0
  * @date    2023
  * @modification	
  * @brief   Gestion de l'écran LCD
  ******************************************************************************************************************/
	
#ifndef __GLCD_H
#define __GLCD_H

#include "stm32f10x.h"

#define BACK_COLOR GLCD_COLOR_BLACK
#define TEXT_COLOR GLCD_COLOR_WHITE

void initGLCD(void);
void clearScreenGLCD(void);
void clearZone(uint32_t x, uint32_t y, uint32_t width, uint32_t height); //permet d'effacer une zone rectangulaire

#endif
