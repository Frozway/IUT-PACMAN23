/**
  ******************************************************************************************************************
  * @file    TIMERS.c 
  * @author  IUT Informatique La Rochelle
  * @version v1.0
  * @date    2023
  * @modification	
  * @brief   configuration des timers
  ******************************************************************************************************************/
	
#include "stm32f10x.h"                  /* STM32F10x.h definitions            */
#include "GLCD_Config.h"
#include "ImagesBMP.h"
#include "ExtGlobales.h"
#include "TIMERS.h"
#include "registres.h"
#include "bouton.h"
#include "game.h"

/**
  * @brief  configuration du timer1 pour générer des interruptions
  * @param  None
  * @retval None
  */
void cfgTimer1(void){
		RCC->APB2ENR |= (1<<11); // fourniture de l'horloge sur le périphérique TMR1
    
		TIM1->PSC = 71; // prescaler de 72
    TIM1->ARR = 10000; // autoReload pour 10ms

    TIM1->DIER |= UIE; // autorisation des interruptions sur Timer1
    TIM1->CR1  |= CEN; // autorisation du comptage 
    
    SETENA0 = (1<<25); // bit 25 pour activation IRQ Timer1 Up
}

void TIM1_UP_TIM10_IRQHandler (void){
//    LEDS_PORTB ^= (1<<8);
	//static int cnt=0;
    if ( TIM1->SR & UIF ){  // apparemment inevitable de faire ce test car sinon, le CPU est capable de se dérouter
                            // dans la routine d'interruption alors que le drapeau n'est pas levé
        TIM1->SR &= ~UIF;
				
    
							if ( Pacman.direction == DROITE ){
					Pacman.x++;
					if ( Pacman.x + Pacman.width >= GLCD_SIZE_Y-1 ){
						Pacman.x=GLCD_SIZE_Y-1-Pacman.width;
						Pacman.direction = ARRET;
						Pacman.bmpCharacter = Pacman.bmpCharacterOpen;
						Pacman.bDessine=1;  // redessiner le Pacman
					}
				}else if ( Pacman.direction == GAUCHE ){
					Pacman.x--;
					if ( Pacman.x <= 0 ){
						Pacman.x=0;
						Pacman.direction = ARRET;
						Pacman.bmpCharacter = Pacman.bmpCharacterOpen;
						Pacman.bDessine=1;  // redessiner le Pacman
					}
				}else if ( Pacman.direction == HAUT ){
					Pacman.y--;
					if ( Pacman.y <= 0 ){
						Pacman.y=0;
						Pacman.direction = ARRET;
						Pacman.bmpCharacter = Pacman.bmpCharacterOpen;
						Pacman.bDessine=1;  // redessiner le Pacman
					}
				}else if ( Pacman.direction == BAS ){
					Pacman.y++;
					if ( Pacman.y + Pacman.height >= HEIGHT_BOARD-1 ){
						Pacman.y=HEIGHT_BOARD-1-Pacman.height;
						Pacman.direction = ARRET;
						Pacman.bmpCharacter = Pacman.bmpCharacterOpen;
						Pacman.bDessine=1;  // redessiner le Pacman
					}
				}
				
				if ( Pacman.direction != ARRET ){
					Pacman.counterMouseTick = (Pacman.counterMouseTick + 1)%Pacman.mouseTick ; // modulo 1/5ème de seconde
					if ( Pacman.counterMouseTick == 0 ){
						if (Pacman.bmpCharacter == Pacman.bmpCharacterOpen){
							Pacman.bmpCharacter = (unsigned char *) bmpPacManClose;
						}else{
							Pacman.bmpCharacter = Pacman.bmpCharacterOpen;
						}
					}
				}
			}
}
void cfgTimer2(void){ // tempo de debounce
		RCC->APB1ENR |= (1<<0); // fourniture de l'horloge sur le périphérique TMR2
    
		TIM2->PSC = 71; // prescaler de 72
    TIM2->ARR = 1000; // autoReload pour 1ms
	
    TIM2->DIER |= UIE; // autorisation des interruptions sur Timer1

  	SETENA0 = (1<<28); // bit 28 pour activation IRQ Timer2
}


void TIM2_IRQHandler (void){
    if ( TIM2->SR & UIF ){  // apparemment inevitable de faire ce test car sinon, le CPU est capable de se dérouter
                            // dans la routine d'interruption alors que le drapeau n'est pas levé
        TIM2->SR  &= ~UIF; // abaissement drapeau
				TIM2->CR1 &= ~CEN; // arret du timer (arret du comptage)
				
				EXTI->PR  |= (1<<15)|(1<<14)|(1<<13)|(1<<3); // abaissement de tous les drapeaux
				EXTI->IMR |= (1<<15)|(1<<14)|(1<<13)|(1<<3); //demasquage de toutes les interruptions sur GPIO
			
			
    }
}

void initTIMERS()
{
		cfgIO();
		cfgTimer1(); // pour la gestion du deplacement à 1pixel tous les 10ms
		cfgTimer2(); // tempo de debounce des BPs
}
