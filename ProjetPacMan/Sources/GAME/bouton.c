/**
  ******************************************************************************************************************
  * @file    boutons.c 
  * @author  IUT Informatique La Rochelle
  * @version v1.0
  * @date    2021
  * @modification	rien pour le moment
  * @brief   configurations des GPIO pour les boutons
  ******************************************************************************************************************/
	
#include "stm32f10x.h"                  /* STM32F10x.h definitions            */
#include "ExtGlobales.h"
#include "TIMERS.h"
#include "bouton.h"
#include "registres.h"
#include "types.h"
#include "ImagesBMP.h"

void cfgIO(void){
	unsigned long tmp;
	/*---- Mise en service des registres AFIO pour la configuration d'interruptions externes ----*/
  RCC->APB2ENR |= (1 << 0); // Enable AFIO clock
	RCC->APB2ENR |= (1<<1); // AFIOEN  pour l'utilisation en EXTI des GPIO
	RCC->APB2ENR |= (1<<2); // GPIOAEN
	RCC->APB2ENR |= (1<<3); // GPIOBEN
	RCC->APB2ENR |= (1<<5); // GPIODEN
	RCC->APB2ENR |= (1<<8); // GPIOGEN
	
	// PG15, PG14 et PG13 en entrees flottantes
	tmp = GPIOG->CRH & 0x000FFFFF;
	GPIOG->CRH = tmp | 0x44400000;
	
	// PD3 en entree flottante
	tmp = GPIOD->CRL & 0xFFFF0FFF;
	GPIOD->CRL = tmp | 0x00004000;
	
	// configuration des interruptions
	EXTI->FTSR |= (1<<15)|(1<<14)|(1<<13)|(1<<3); // detections sur fronts descendants
	EXTI->IMR  |= (1<<15)|(1<<14)|(1<<13)|(1<<3); // demasquage
	
	tmp = AFIO_EXTICR4 & 0x000F;
	AFIO_EXTICR4 = tmp | 0x6660; // PG15 sur EXTI15, PG14 sur EXTI14, PG13 sur EXTI13
	
	tmp = AFIO_EXTICR1 & 0x0FFF;
	AFIO_EXTICR1 = tmp | 0x3000; // PD3 sur EXTI3
	
	SETENA0 |= (1<<9); // activation canal 9 du NVIC pour EXTI3_IRQHandler
	SETENA1 |= (1<<(40-32)); // activation canal 40 du NVIC pour EXTI15_10_IRQHandler
}
/*---- Interruption sur PD3 ----------------------------------------------------------------*/
void EXTI3_IRQHandler(void){
	if ( EXTI->PR & (1<<3) ){ // Joystick Down
				EXTI->IMR  &= ~(1<<3); // masquage
        EXTI->PR |= (1<<3);   //Attention ! Il faut mettre un 1 pour "baisser" le drapeau !
				// tempo de debounce
				TIM2->CNT=0; // au cas où un autre debounce serait en cours
				TIM2->CR1 |= CEN;
			  
				if ( Pacman.direction == HAUT ){
					Pacman.direction = ARRET;
					Pacman.bmpCharacter = Pacman.bmpCharacterOpen ;
					Pacman.bDessine=1; // redessiner le mangeur
				}else{
					Pacman.direction = BAS;
					Pacman.bmpCharacterOpen = (unsigned char *)bmpPacManOpenDown;
				}
  }
}

/*---- Interruption sur PG15, PG14 et PG13 ----------------------------------------------------------------*/
void EXTI15_10_IRQHandler(void){ 
    if ( EXTI->PR & (1<<13) ) {// Joystick Right
				EXTI->IMR  &= ~(1<<13); // masquage
        EXTI->PR |= (1<<13);   //Attention ! Il faut mettre un 1 pour "baisser" le drapeau !
				// tempo de debounce
				TIM2->CNT=0; // au cas où un autre debounce serait en cours
				TIM2->CR1 |= CEN;
			  
				if ( Pacman.direction == GAUCHE ){
					Pacman.direction = ARRET;
					Pacman.bmpCharacter = Pacman.bmpCharacterOpen ;
					Pacman.bDessine=1; // redessiner le mangeur
				}else{
					Pacman.direction = DROITE;
					Pacman.bmpCharacterOpen = (unsigned char *)bmpPacManOpenRight;
				}
    }
		if ( EXTI->PR & (1<<14) ){ // Joystick Left    
				EXTI->IMR  &= ~(1<<14); // masquage
        EXTI->PR |= (1<<14);   //Attention ! Il faut mettre un 1 pour "baisser" le drapeau !
			  // tempo de debounce
			  TIM2->CNT=0; // au cas où un autre debounce serait en cours
				TIM2->CR1 |= CEN;
				if ( Pacman.direction == DROITE ){
					Pacman.direction = ARRET;
					Pacman.bmpCharacter = Pacman.bmpCharacterOpen ;
					Pacman.bDessine=1; // redessiner le mangeur
				}else{
					Pacman.direction = GAUCHE;
					Pacman.bmpCharacterOpen = (unsigned char *)bmpPacManOpenLeft;
				}
    }
		if ( EXTI->PR & (1<<15) ) {// Joystick Up
				EXTI->IMR  &= ~(1<<15); // masquage
        EXTI->PR |= (1<<15);   //Attention ! Il faut mettre un 1 pour "baisser" le drapeau !
			  // tempo de debounce
			  TIM2->CNT=0; // au cas où un autre debounce serait en cours
				TIM2->CR1 |= CEN;
				if ( Pacman.direction == BAS ){
					Pacman.direction = ARRET;
					Pacman.bmpCharacter = Pacman.bmpCharacterOpen ;
					Pacman.bDessine=1; // redessiner le mangeur
				}else{
					Pacman.direction = HAUT;
					Pacman.bmpCharacterOpen = (unsigned char *)bmpPacManOpenUp;
				}
    }
} 
