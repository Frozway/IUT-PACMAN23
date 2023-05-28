/**
  ******************************************************************************************************************
  * @file    GPIO.c 
  * @author  Colin DEMOUGE & Thibaut LEFRANCOIS
  * @version v1.0
  * @date    2023
  * @modification	
  * @brief   
  ******************************************************************************************************************/
	
	#include "stm32f10x.h"                  /* STM32F10x.h definitions            */
	#include "registres.h"
	#include "GPIO.h"
	
	#include "game.h"

void initGPIO(void)
{
	//variable servant à gagner du temps lors de l'attribution des masques aux registres
	unsigned long temp;
	
	//Init Joystick Gauche
	temp = GPIOG->CRH & ~ 0xF000000;
	GPIOG->CRH = temp | 0x4000000;
	
	//Init Joystick Droite
	temp = GPIOG->CRH & ~ 0xF00000;
	GPIOG->CRH = temp | 0x400000;		
	
	//Init Joystick Haut
	temp = GPIOG->CRH & ~ 0xF0000000;
	GPIOG->CRH = temp | 0x40000000;
	
	//Init Joystick Bas
	temp = GPIOD->CRL & ~ 0xF000;
	GPIOD->CRL = temp | 0x4000;
	
	//Init BP_User
	temp = GPIOG->CRH & 0xFFFFFFF0;
	GPIOG->CRH = temp | 0x00000004;
	
	//Init Joystick Select
	temp = GPIOG->CRL & ~ 0xF0000000;
	GPIOG->CRL = temp | 0x40000000;
		
	//Enable GPIO
	RCC->APB2ENR |= (1<<5); // GPIODEN
	RCC->APB2ENR |= (1<<8); // GPIOGEN	
		
	//---- Mise en service des registres AFIO pour la configuration d'interruptions externes ----
  RCC->APB2ENR |= (1 << 0); // Enable AFIO clock
	RCC->APB2ENR |= (1<<1); // AFIOEN  pour l'utilisation en EXTI des GPIO		
}

int joytickUpPressed(void)
{
	return (GPIOG->IDR & 0x8000) == APPUYE;
}

int joytickDownPressed(void)
{
	return (GPIOD->IDR & 0x0008) == APPUYE;
}

int joytickSelectPressed(void)
{
	return (GPIOG->IDR & 0x0080) == APPUYE;
}

int joytickLeftPressed(void)
{
	return (GPIOG->IDR & 0x4000) == APPUYE;
}

int joytickRightPressed(void)
{
	return (GPIOG->IDR & 0x2000) == APPUYE;
}

int userButtonPressed(void)
{
	return (GPIOG->IDR & 0x0100) == APPUYE;
}

void startInterruptBP_USER()
{
	unsigned long temp;

	SETENA0 |= (1<<23); //choix de la routine ? lancer lors de l'interruption
	temp = AFIO_EXTICR3 & 0xFFF0;
	AFIO_EXTICR3 = temp | 0x0006;
	EXTI->IMR |= (1<<8);
	EXTI->RTSR |= (1<<8); //on d?clenche lors de l'appui sur le bouton (donc front montant)
}

void stopInterruptBP_USER()
{
	EXTI->IMR &= ~(1<<8);
}

//routine d'interruption du bouton BP_USER
void EXTI9_5_IRQHandler(void)
{
	if(EXTI->PR & (1<<8)) //on rentre que si il n'y a pas d?ja un missile
	{
		//shoot();
		EXTI->PR |= (1<<8); //on remet ? z?ro pour la prochaine interruption
	}	
}
