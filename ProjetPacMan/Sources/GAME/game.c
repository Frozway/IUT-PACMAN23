/**
  ******************************************************************************************************************
  * @file    jeu.c 
  * @author  R.NOHE     IUT Informatique La Rochelle
  * @version v1.0
  * @date    9 mai 2014
  * @modification	rien pour le moment
  * @brief   routines pour la strategie du jeu
  ******************************************************************************************************************/

#include <stdlib.h>
#include "stm32f10x.h"                  /* STM32F10x.h definitions            */
#include "types.h"
#include "ImagesBMP.h"
#include "GLCD.h"
#include "ExtGlobales.h"
#include "game.h"
#include "TIMERS.h"
#include "GPIO.h"
#include "random.h"
#include "menu.h"
#include "globales.h"
#include "bouton.h"


// affichage du plateau de jeu
void drawBoard(void)
	{
	GLCD_DrawBargraph(0, HEIGHT_BOARD, GLCD_WIDTH, 1, 1024); // trace d'un trait
	GLCD_SetFont(&GLCD_Font_6x8);
	GLCD_SetForegroundColor(GLCD_COLOR_YELLOW);
	GLCD_DrawString (29, 232     ,"SCORE : ");
	GLCD_SetForegroundColor(GLCD_COLOR_RED);
	GLCD_DrawString (200, 232,"SCORE : ");
}

void startLevel()
{
	clearScreenGLCD();
	drawBoard();
	initTIMERS();
}

void update(volatile Character * character)
	{
		if ( character->x > character->xOld )
		{
			// on se depeche de cloner xBoule
			character->xClone = character->x; 
	
			//suppression de la trace à gauche
			GLCD_DrawBargraph(character->xOld, character->yClone, character->xClone - character->xOld, character->height, 0);		
	
			//on ne memorise que la partie qui vient d'être effacée au cas où xBoule aurait été modifiee entre temps
			character->xOld = character->xClone; 
	
			character->bDessine=1;
		}
		
		else if (character->x < character->xOld)
		{
			// on se depeche de cloner xBoule
			character->xClone = character->x;
			
			//suppression de la trace à droite
			GLCD_DrawBargraph(character->xClone + character->width, character->yClone, character->xOld - character->xClone, character->height, 0);
			
			//on ne memorise que la partie qui vient d'être effacée au cas où xBoule aurait été modifiee entre temps
			character->xOld = character->xClone; 
			
			character->bDessine=1;
		}
				
		if (character->y > character->yOld)
		{
			// on se depeche de cloner yBoule
			character->yClone = character->y ; 
			
			//suppression de la trace en haut
			GLCD_DrawBargraph(character->xClone, character->yOld, character->width, character->yClone - character->yOld, 0);
			
			//on ne memorise que la partie qui vient d'être effacée au cas où xBoule aurait été modifiee entre temps
			character->yOld = character->yClone ;

			character->bDessine=1;
		} 
		
		else if (character->y < character->yOld)
		{
			// on se depeche de cloner yBoule
			character->yClone = character->y ;
			
			//suppression de la trace en bas
			GLCD_DrawBargraph(character->xClone, character->yClone + character->height, character->width, character->yOld - character->yClone, 0);
			
			//on ne memorise que la partie qui vient d'être effacée au cas où xBoule aurait été modifiee entre temps
			character->yOld = character->yClone ;

			character->bDessine=1;
		}
				
		if (character->bDessine == 1)
		{
			//Repasser à 0 pour dire qu'on l'a dessiné
			character->bDessine=0;
			
			// affichage du personnage
			GLCD_DrawBitmap(character->xClone, character->yClone, character->width, character->height, character->bmpCharacter);
		}
}


bool gameLoop()
{
	
	//Affichage du plateau et lancement des timers
	startLevel();
	
	srand(TIM1->CNT); // decalage de la séquence pseudo aléatoire en fonction du timer1 pour avoir vraiment de l'aléatoire

	//Initialisation de Pacman
	Pacman.width=WIDTH_PACMAN;
	Pacman.height=HEIGHT_PACMAN;
	Pacman.direction=ARRET;
	Pacman.counterTick=0;
	Pacman.moveTick=1; // tous les 1 ticks, il aura le droit de se déplacer
	Pacman.counterMouseTick=0;
	Pacman.mouseTick=20; // tous les 20 ticks => 1/5s ouverture/fermeture bouche
	Pacman.bDessine=1;
	Pacman.bmpCharacterOpen=(unsigned char *)bmpPacManOpenRight;
	Pacman.bmpCharacter=Pacman.bmpCharacterOpen;
	
	// positionnement aléatoire du Pacman
	Pacman.x = rand()% (GLCD_WIDTH - Pacman.width);
	Pacman.y = rand()% (GLCD_HEIGHT - Pacman.height);
	
	Pacman.xClone = Pacman.x;
	Pacman.xOld   = Pacman.x; 
	
	while(1)
	{
		update( &Pacman );
	}
	
	return true ;

}






