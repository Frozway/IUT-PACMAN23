/**
  ******************************************************************************************************************
  * @file    ImagesBMP.h
  * @author  Colin DEMOUGE & Thibaut LEFRANCOIS
  * @version v1.0
  * @date    2023
  * @modification	
  * @brief
  ******************************************************************************************************************/


#ifndef __IMAGES_BMP_H
#define __IMAGES_BMP_H
#include "GLCD_Config.h"

#define HEIGHT_PACMAN 8

#define WIDTH_PACMAN 8



#define HEIGHT_ENNEMI 8

#define WIDTH_ENNEMI 8



#define HEIGHT_FOOD 4

#define WIDTH_FOOD 4

//d?finition des matrices d'affichage des ennemis/*

extern const unsigned short bmpPacManClose[HEIGHT_PACMAN*WIDTH_PACMAN];
extern const unsigned short bmpPacManOpenRight[HEIGHT_PACMAN*WIDTH_PACMAN];
extern const unsigned short bmpPacManOpenLeft[HEIGHT_PACMAN*WIDTH_PACMAN];
extern const unsigned short bmpPacManOpenDown[HEIGHT_PACMAN*WIDTH_PACMAN];
extern const unsigned short bmpPacManOpenUp[HEIGHT_PACMAN*WIDTH_PACMAN];
extern const unsigned short bmpEnnemiClose[HEIGHT_ENNEMI*WIDTH_ENNEMI];
extern const unsigned short bmpEnnemiOpenRight[HEIGHT_ENNEMI*WIDTH_ENNEMI];
extern const unsigned short bmpEnnemiOpenLeft[HEIGHT_ENNEMI*WIDTH_ENNEMI];
extern const unsigned short bmpEnnemiOpenDown[HEIGHT_ENNEMI*WIDTH_ENNEMI];
extern const unsigned short bmpEnnemiOpenUp[HEIGHT_ENNEMI*WIDTH_ENNEMI];
extern const unsigned short bmpFood[HEIGHT_FOOD*WIDTH_FOOD];

#define HEIGHT_ROCKET	10
#define WIDTH_ROCKET 2

#endif
