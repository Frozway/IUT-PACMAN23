/**
  ******************************************************************************************************************
  * @file    menu.h
  * @author  Colin DEMOUGE & Thibaut LEFRANCOIS
  * @version v1.0
  * @date    2023
  * @modification	
  * @brief
  ******************************************************************************************************************/
	
#ifndef __MENU_H
#define __MENU_H

typedef enum {JOUER, QUITTER} MenuSelected;

void displayMenu(MenuSelected menuSelected);
int refreshMenu(void);
void displayGameLosed(void);
void displayGameWon(void);
void displayLogo(void);

#endif
