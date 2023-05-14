/**
  ******************************************************************************************************************
  * @file    menu.c
  * @author  IUT Informatique La Rochelle
  * @version v1.0
  * @date    2023
  * @modification	
  * @brief   Gestion du menu
  ******************************************************************************************************************/

#include "GLCD_Config.h"
#include "Board_GLCD.h"
#include "GPIO.h"
#include "GLCD.h"

#include "menu.h"
#include "ImagesBMP.h"
#include "ExtGlobales.h"

#include "stdio.h"

//affichage de l'?cran d'accueil
void displayMenu(MenuSelected menuSelected)
{	
	clearScreenGLCD();	
	displayLogo();
	
	if(menuSelected==JOUER)
	{ 
		GLCD_SetForegroundColor(GLCD_COLOR_RED);
		GLCD_DrawString  (100, 100,"Jouer");
		GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
		GLCD_DrawString (100, 140, "Quitter");
	}
	else if(menuSelected==QUITTER)
	{ 
		GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
		GLCD_DrawString (100, 100,"Jouer");
		GLCD_SetForegroundColor(GLCD_COLOR_RED);
		GLCD_DrawString (100, 140,"Quitter");
	}
}

void displayLogo()
{

	GLCD_SetBackgroundColor(GLCD_COLOR_BLACK);
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_DrawString(100, 10, "PAC-MAN");
	GLCD_DrawBitmap(110, 50, WIDTH_PACMAN, HEIGHT_PACMAN, (unsigned char*)bmpPacManOpenRight);
	GLCD_DrawBitmap(140, 50, WIDTH_ENNEMI, HEIGHT_ENNEMI, (unsigned char*)bmpEnnemiClose);
	GLCD_DrawBitmap(170, 53, WIDTH_FOOD, HEIGHT_FOOD, (unsigned char*)bmpFood);
	
}

int refreshMenu()
{
	static MenuSelected menuSelected = JOUER;
	
	if(joytickUpPressed() && menuSelected == QUITTER)
	{
		menuSelected = JOUER;
		displayMenu(menuSelected);
	}
	if(joytickDownPressed()&& menuSelected == JOUER) 
	{
		menuSelected = QUITTER;
		displayMenu(menuSelected);
	}
	if (joytickSelectPressed() && menuSelected==JOUER)
	{
		return 1 ;
	}
	
	return 0 ;
	
	//return joytickSelectPressed() && menuSelected==JOUER;
}
	




