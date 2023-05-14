/**
  ******************************************************************************************************************
  * @file    GLCD.c
  * @author  IUT Informatique La Rochelle
  * @version v1.0
  * @date    2023
  * @modification	
  * @brief   Gestion de l'écran LCD
  ******************************************************************************************************************/
	
#include "stm32f10x.h"                  /* STM32F10x.h definitions            */
	
#include "Board_GLCD.h"
#include "GLCD_Config.h"

#include "GLCD.h"
	
void initGLCD()
{
 /* Initialize graphical LCD display   */
  	GLCD_Initialize();                          
		clearScreenGLCD();
}	

void clearScreenGLCD()
{
	GLCD_SetBackgroundColor(GLCD_COLOR_BLACK);
	GLCD_ClearScreen();
}		

void clearZone(uint32_t x, uint32_t y, uint32_t width, uint32_t height) //permet d'effacer une zone rectangulaire
{
	GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
	GLCD_DrawBargraph(x, y, width, height, 1024);
}
