/**
  ******************************************************************************************************************
  * @file    main.c 
  * @author  IUT Informatique La Rochelle
  * @version v1.0
  * @date    2023
  * @modification	
  * @brief   
  ******************************************************************************************************************/

#include "GLCD.h"
#include "GPIO.h"
#include <stdlib.h>
#include "stm32f10x.h"                  /* STM32F10x.h definitions            */
#include "menu.h"
#include "game.h"

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
 
int main(void)
{
	//initalisation du HARDWARE
	initGLCD();
	initGPIO();
	
	while(1)
	{		
		//Affichage du menu tant que le mode jouer n'a pas été cliqué 
		displayMenu(JOUER);
		while(refreshMenu() != 1);
		
		//Lancement de la boucle de jeu et vérification constante de si la partie est finit
		if (gameLoop() == true) 
		{
			break ;
		}
		
	}
	
}








