/**
  ******************************************************************************************************************
  * @file    game.c 
  * @author  Colin DEMOUGE & Thibaut LEFRANCOIS
  * @version v1.0
  * @date    2023
  * @modification	
  * @brief   
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
	// V�rifier si la position en x a �t� modifi�e
	if (character->x > character->xOld)
	{
		// Cloner la nouvelle position en x
		character->xClone = character->x; 
		
		// Supprimer la trace � gauche
		GLCD_DrawBargraph(character->xOld, character->yClone, character->xClone - character->xOld, character->height, 0);		
		
		// M�moriser uniquement la partie qui vient d'�tre effac�e, au cas o� character->x aurait �t� modifi� entre-temps
		character->xOld = character->xClone; 
		
		// Indiquer que le personnage doit �tre dessin�
		character->bDessine = 1;
	}
	else if (character->x < character->xOld)
	{
		// Cloner la nouvelle position en x
		character->xClone = character->x;
		
		// Supprimer la trace � droite
		GLCD_DrawBargraph(character->xClone + character->width, character->yClone, character->xOld - character->xClone, character->height, 0);
		
		// M�moriser uniquement la partie qui vient d'�tre effac�e, au cas o� character->x aurait �t� modifi� entre-temps
		character->xOld = character->xClone; 
		
		// Indiquer que le personnage doit �tre dessin�
		character->bDessine = 1;
	}
	
	// V�rifier si la position en y a �t� modifi�e
	if (character->y > character->yOld)
	{
		// Cloner la nouvelle position en y
		character->yClone = character->y ; 
		
		// Supprimer la trace en haut
		GLCD_DrawBargraph(character->xClone, character->yOld, character->width, character->yClone - character->yOld, 0);
		
		// M�moriser uniquement la partie qui vient d'�tre effac�e, au cas o� character->y aurait �t� modifi� entre-temps
		character->yOld = character->yClone ;
		
		// Indiquer que le personnage doit �tre dessin�
		character->bDessine = 1;
	} 
	else if (character->y < character->yOld)
	{
		// Cloner la nouvelle position en y
		character->yClone = character->y ;
		
		// Supprimer la trace en bas
		GLCD_DrawBargraph(character->xClone, character->yClone + character->height, character->width, character->yOld - character->yClone, 0);
		
		// M�moriser uniquement la partie qui vient d'�tre effac�e, au cas o� character->y aurait �t� modifi� entre-temps
		character->yOld = character->yClone ;
		
		// Indiquer que le personnage doit �tre dessin�
		character->bDessine = 1;
	}
	
	// V�rifier si le personnage doit �tre dessin�
	if (character->bDessine == 1)
	{
		// Remettre la valeur � 0 pour indiquer qu'il a �t� dessin�
		character->bDessine = 0;
		
		// Afficher le personnage
		GLCD_DrawBitmap(character->xClone, character->yClone, character->width, character->height, character->bmpCharacter);
	}
}

	
// D�termine si deux figures se touchent
unsigned int collision(volatile Character * character1, volatile Character * character2)
{
	// V�rifier si les coordonn�es x des deux figures se chevauchent
	if (character1->x + character1->width <= character2->x)
		return 0; // Pas de collision
	
	if (character1->x >= character2->x + character2->width)
		return 0; // Pas de collision

	// V�rifier si les coordonn�es y des deux figures se chevauchent
	if (character1->y + character1->height <= character2->y)
		return 0; // Pas de collision

	if (character1->y >= character2->y + character2->height)
		return 0; // Pas de collision

	// Si les coordonn�es x et y se chevauchent, il y a collision
	return 1;
}

// D�termine s'il y a collision entre le personnage et un ennemi
unsigned int collisionEnemy(volatile Character * character)
{
	int i;
	for (i = 0; i < nbEnemy; i++)
	{
		if (collision(character, &Enemy[i]))
			return 1; // Collision d�tect�e avec un ennemi
	}
	
	return 0; // Pas de collision avec les ennemis
}


void directionEnemy(volatile Character *anEnemy)
{
    int deltaX;
    int deltaY;

    // Calcul des diff�rences en X et en Y entre l'ennemi et la position de la nourriture
    deltaX = Food.x - anEnemy->x;
    deltaY = Food.y - anEnemy->y;

    if (abs(deltaX) >= abs(deltaY))
    {
        // Si la diff�rence en X est plus grande ou �gale � la diff�rence en Y,
        // on privil�gie le mouvement horizontal

        if (deltaX > 0)
        {
            // La nourriture se trouve � droite de l'ennemi
            anEnemy->direction = DROITE;
            anEnemy->bmpCharacterOpen = (unsigned char *)bmpEnnemiOpenRight;
        }
        else
        {
            // La nourriture se trouve � gauche de l'ennemi
            anEnemy->direction = GAUCHE;
            anEnemy->bmpCharacterOpen = (unsigned char *)bmpEnnemiOpenLeft;
        }
    }
    else
    {
        // Si la diff�rence en Y est plus grande que la diff�rence en X,
        // on privil�gie le mouvement vertical

        if (deltaY > 0)
        {
            // La nourriture se trouve en bas de l'ennemi
            anEnemy->direction = BAS;
            anEnemy->bmpCharacterOpen = (unsigned char *)bmpEnnemiOpenDown;
        }
        else
        {
            // La nourriture se trouve en haut de l'ennemi
            anEnemy->direction = HAUT;
            anEnemy->bmpCharacterOpen = (unsigned char *)bmpEnnemiOpenUp;
        }
    }
}
	
void newFood(volatile Character* aFood)
{
    // D�finition des caract�ristiques de la nourriture
    aFood->bmpCharacter = (unsigned char*)bmpFood;  // Image de la nourriture
    aFood->width = WIDTH_FOOD;                      // Largeur de la nourriture
    aFood->height = HEIGHT_FOOD;                    // Hauteur de la nourriture

    // Placement de la nourriture de mani�re al�atoire sans collision avec Pacman
    do
    {
        // G�n�ration de coordonn�es al�atoires pour la nourriture
        aFood->x = rand() % (GLCD_WIDTH - WIDTH_FOOD);      // Coordonn�e x dans les limites de l'�cran
        aFood->y = rand() % (HEIGHT_BOARD - HEIGHT_FOOD);   // Coordonn�e y dans les limites du plateau de jeu
    } while (collision(aFood, &Pacman));  // V�rification de collision avec Pacman, si collision, g�n�rer de nouvelles coordonn�es

    // Affichage de la nourriture sur l'�cran
    GLCD_DrawBitmap(aFood->x, aFood->y, aFood->width, aFood->height, aFood->bmpCharacter);
}


void newEnemy(void)
{
	// Cr�ation d'un nouvel ennemi

	// Initialisation des propri�t�s de l'ennemi
	Enemy[nbEnemy].width = WIDTH_ENNEMI;               // Largeur de l'ennemi
	Enemy[nbEnemy].height = HEIGHT_PACMAN;            // Hauteur de l'ennemi
	Enemy[nbEnemy].direction = ARRET;               // Direction initiale de l'ennemi (arr�t�)
	Enemy[nbEnemy].counterTick = 0;                   // Compteur de ticks pour l'ennemi
	Enemy[nbEnemy].moveTick = 2;                      // Nombre de ticks avant le prochain mouvement de l'ennemi
	Enemy[nbEnemy].counterMouseTick = 0;              // Compteur de ticks pour la bouche de l'ennemi
	Enemy[nbEnemy].mouseTick = 20;                    // Nombre de ticks avant le prochain mouvement de la bouche de l'ennemi
	Enemy[nbEnemy].bDessine = 1;                      // Indicateur si l'ennemi doit �tre dessin� ou non

	// G�n�ration al�atoire des coordonn�es de d�part de l'ennemi
	do
	{
		Enemy[nbEnemy].x = (rand() % 2) * (GLCD_WIDTH - WIDTH_PACMAN - 1);     // Coordonn�e X (abscisse) de l'ennemi
		Enemy[nbEnemy].y = (rand() % 2) * (HEIGHT_BOARD - HEIGHT_PACMAN - 1);  // Coordonn�e Y (ordonn�e) de l'ennemi
	}
	while(collisionEnemy(&Enemy[nbEnemy]) || collision(&Food, &Enemy[nbEnemy]));  // V�rifie s'il y a une collision avec d'autres objets

	directionEnemy(&Enemy[nbEnemy]);                  // D�termine la direction de l'ennemi
	Enemy[nbEnemy].bmpCharacter = Enemy[nbEnemy].bmpCharacterOpen;  // Image de l'ennemi (graphisme)

	nbEnemy++;                                        // Incr�mente le nombre total d'ennemis dans le tableau
}


void moveEnemy(volatile Character* anEnemy)
{
	// Incr�mente le compteur de ticks pour le mouvement de l'ennemi
	anEnemy->counterTick = (anEnemy->counterTick + 1) % anEnemy->moveTick;

	// V�rifie si le compteur de ticks atteint 0 pour effectuer un d�placement
	if (anEnemy->counterTick == 0)
	{
		// Effectue diff�rentes actions en fonction de la direction de l'ennemi
		switch (anEnemy->direction)
		{
			case DROITE:
				anEnemy->x++; // D�place l'ennemi vers la droite

				// V�rifie si l'ennemi atteint la limite de la largeur de l'�cran
				if (anEnemy->x + anEnemy->width >= GLCD_WIDTH - 1)
				{
					anEnemy->x = GLCD_WIDTH - 1 - anEnemy->width; // R�ajuste la position de l'ennemi � la limite
					directionEnemy(anEnemy); // Change la direction de l'ennemi
				}

				// V�rifie si l'ennemi atteint la position horizontale de la nourriture
				if (anEnemy->x == Food.x)
				{
					directionEnemy(anEnemy); // Change la direction de l'ennemi
				}
				break;

			case GAUCHE:
				anEnemy->x--; // D�place l'ennemi vers la gauche

				// V�rifie si l'ennemi atteint la limite de la position horizontale minimale
				if (anEnemy->x <= 0)
				{
					anEnemy->x = 0; // R�ajuste la position de l'ennemi � la limite
					directionEnemy(anEnemy); // Change la direction de l'ennemi
				}

				// V�rifie si l'ennemi atteint la position horizontale de la nourriture
				if (anEnemy->x == Food.x)
				{
					directionEnemy(anEnemy); // Change la direction de l'ennemi
				}
				break;

			case HAUT:
				anEnemy->y--; // D�place l'ennemi vers le haut

				// V�rifie si l'ennemi atteint la limite de la position verticale minimale
				if (anEnemy->y <= 0)
				{
					anEnemy->y = 0; // R�ajuste la position de l'ennemi � la limite
					directionEnemy(anEnemy); // Change la direction de l'ennemi
				}

				// V�rifie si l'ennemi atteint la position verticale de la nourriture
				if (anEnemy->y == Food.y)
				{
					directionEnemy(anEnemy); // Change la direction de l'ennemi
				}
				break;

			case BAS:
				anEnemy->y++; // D�place l'ennemi vers le bas

				// V�rifie si l'ennemi atteint la limite de la hauteur du plateau
				if (anEnemy->y + anEnemy->height >= HEIGHT_BOARD - 1)
				{
					anEnemy->y = HEIGHT_BOARD - 1 - anEnemy->height; // R�ajuste la position de l'ennemi � la limite
					directionEnemy(anEnemy); // Change la direction de l'ennemi
					//unEnnemi->bDessine=1;
				}

				// V�rifie si l'ennemi atteint la position verticale de la nourriture
				if (anEnemy->y == Food.y)
				{
					directionEnemy(anEnemy); // Change la direction de l'ennemi
				}
				break;

			default: // Cas par d�faut (pour supprimer l'avertissement du compilateur)
				break; // Pour supprimer l'avertissement du compilateur
		}

		// Effectue des actions suppl�mentaires si la direction de l'ennemi n'est pas ARRET
		if (anEnemy->direction != ARRET)
		{
			// Incr�mente le compteur de ticks pour l'animation de la souris (ouverture/fermeture)
			anEnemy->counterMouseTick = (anEnemy->counterMouseTick + 1) % anEnemy->mouseTick; // modulo 1/5�me de seconde

			// V�rifie si le compteur de ticks atteint 0 pour changer l'apparence de l'ennemi
			if (anEnemy->counterMouseTick == 0)
			{
				if (anEnemy->bmpCharacter == anEnemy->bmpCharacterOpen)
				{
					anEnemy->bmpCharacter = (unsigned char*)bmpEnnemiClose; // Change l'apparence de l'ennemi � "ferm�"
				}
				else
				{
					anEnemy->bmpCharacter = anEnemy->bmpCharacterOpen; // Change l'apparence de l'ennemi � "ouvert"
				}
			}
		}
	}
}

void moveAllEnemy(void)
{
	int i;
	for (i=0; i < nbEnemy ; i++)
	{
		moveEnemy( &Enemy[i] );
	}
}

void displayPacmanScore(unsigned int abscisse, unsigned int nbNourriture)
{
	int i;
	for ( i=0 ; i != nbNourriture ; i++)
	{
			GLCD_DrawBitmap ( abscisse+i*(WIDTH_FOOD+2), GLCD_HEIGHT -HEIGHT_FOOD-1 ,
						 WIDTH_FOOD , HEIGHT_FOOD ,
						 (unsigned char *)bmpFood);
	}
}

void gameLoop()
{
	
	//Initialisations des variables n�cessaires aux jeux
	int nbPointsPacMan = 0 ;
	int nbPointsEnemy = 0 ;
	int bCollision = 0;
	
	//Affichage du plateau et lancement des timers
	startLevel();
	
	srand(TIM1->CNT); // decalage de la s�quence pseudo al�atoire en fonction du timer1 pour avoir vraiment de l'al�atoire
	


	//Initialisation de Pacman
	Pacman.width=WIDTH_PACMAN;
	Pacman.height=HEIGHT_PACMAN;
	Pacman.direction=ARRET;
	Pacman.counterTick=0;
	Pacman.moveTick=1; // tous les 1 ticks, il aura le droit de se d�placer
	Pacman.counterMouseTick=0;
	Pacman.mouseTick=20; // tous les 20 ticks => 1/5s ouverture/fermeture bouche
	Pacman.bDessine=1;
	Pacman.bmpCharacterOpen=(unsigned char *)bmpPacManOpenRight;
	Pacman.bmpCharacter=Pacman.bmpCharacterOpen;
	
	// positionnement al�atoire du Pacman
	Pacman.x = rand()% (GLCD_WIDTH - Pacman.width);
	Pacman.y = rand()% (GLCD_HEIGHT - Pacman.height);
	
	Pacman.xClone = Pacman.x;
	Pacman.xOld   = Pacman.x; 
	
	newFood(&Food);
	newEnemy();

	
	while(1)
	{
		int i ;

		update( &Pacman ); // Met � jour la position et l'�tat du personnage Pacman

		for (i=0; i < nbEnemy ; i++)
		{
			update( &Enemy[i] ); // Met � jour la position et l'�tat de chaque ennemi
		}

		if ( collision(&Food, &Pacman) ) // V�rifie s'il y a une collision entre Pacman et la nourriture
		{

			nbPointsPacMan++; // Incr�mente le nombre de points de Pacman
			displayPacmanScore(posScorePacman, nbPointsPacMan); // Affiche le score de Pacman � l'�cran

			if ( nbPointsPacMan == nbPointsForWin ) // V�rifie si Pacman a atteint le nombre de points n�cessaire pour gagner
			{
				GLCD_SetFont(&GLCD_Font_16x24);
				GLCD_SetForegroundColor(GLCD_COLOR_GREEN);
				GLCD_DrawString (30, 82,"GAGNE");
				while(1); // Boucle infinie pour arr�ter le jeu (Pacman a gagn�)
			}
			else
			{
				newEnemy(); // G�n�re un nouvel ennemi
			}

			// Efface la nourriture de l'�cran
			GLCD_DrawBargraph(Food.x, Food.y, Food.width, Food.height, 0);

			newFood (&Food); // G�n�re une nouvelle position pour la nourriture
			moveAllEnemy(); // D�place tous les ennemis

		}

		if (collisionEnemy(&Pacman) ) // V�rifie s'il y a une collision entre Pacman et un ennemi
		{
			GLCD_SetFont(&GLCD_Font_16x24);
			GLCD_SetForegroundColor(GLCD_COLOR_RED);
			GLCD_DrawString (30, 82,"PERDU");
			while(1); // Boucle infinie pour arr�ter le jeu (Pacman a perdu)
		}

		for (i=0, bCollision=0; i < nbEnemy && !bCollision ; i++)
		{
			bCollision=collision(&Enemy[i],&Food); // V�rifie s'il y a une collision entre un ennemi et la nourriture

			if ( bCollision )
			{

				nbPointsEnemy++; // Incr�mente le nombre de points de l'ennemi
				displayPacmanScore(posScoreEnemy,nbPointsEnemy); // Affiche le score de l'ennemi � l'�cran

				if ( nbPointsEnemy == nbPointsForWin ) // V�rifie si l'ennemi a atteint le nombre de points n�cessaire pour gagner
				{
					GLCD_SetFont(&GLCD_Font_16x24);
					GLCD_SetForegroundColor(GLCD_COLOR_RED);
					GLCD_DrawString (30, 82,"PERDU");
					while(1); // Boucle infinie pour arr�ter le jeu (l'ennemi a gagn�)
				}

				// Efface la nourriture de l'�cran
				GLCD_DrawBargraph(Food.x, Food.y, Food.width, Food.height, 0);

				newFood ( &Food ); // G�n�re une nouvelle position pour la nourriture
				moveAllEnemy(); // D�place tous les ennemis

				// L'ennemi devient plus rapide
				if ( Enemy[i].moveTick > 1 )
				{
					Enemy[i].moveTick /= 2; // La vitesse de l'ennemi est multipli�e par 2
				}
			}
		}
	}
}






