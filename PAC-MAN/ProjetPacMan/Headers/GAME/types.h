/**
  ******************************************************************************************************************
  * @file    types.h
  * @author  Colin DEMOUGE & Thibaut LEFRANCOIS
  * @version v1.0
  * @date    2023
  * @modification	
  * @brief
  ******************************************************************************************************************/


#ifndef __TYPES_H
#define __TYPES_H

typedef enum { ARRET, GAUCHE, HAUT, DROITE, BAS } Direction;

typedef struct {
	
	int x; //Position sur l'axe X d'un personnage
	
	int xClone; //Position sur l'axe X du clone d'un personnage
	
	int xOld; //Ancienne position sur l'axe X d'un personnage
	
	int y; //Position sur l'axe Y d'un personnage
	
	int yClone; //Position sur l'axe Y du clone d'un personnage
	
	int yOld; //Ancienne position sur l'axe Y d'un personnage
	
	int width; //Largeur du personnage
	
	int height; //Hauteur du personnage
	
	Direction direction ; //Direction du personnage de type Direction
	
	int counterTick; //Compteur du nombre de tick effectué
	
	int moveTick; //Nombre de tick auquel le personnage peut se déplacer
	
	int counterMouseTick; //Compteur du nombre de tick effectué pour l'ouverture/fermeture de la bouche

	int mouseTick; //Nombre de tick avant ouverture/fermeture de la bouche du personnage
	
	int bDessine; // booleen pour forcer le dessin (1 représente true et 0 false)
	
	unsigned char * bmpCharacterOpen;  //Stocke l'image du personnage selon sa direction si il est ouvert
	
	unsigned char * bmpCharacter; //Stocke l'image du personnage (ouvert ou fermé)
} Character;

#endif
