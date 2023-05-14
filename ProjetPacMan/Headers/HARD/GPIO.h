/**
  ******************************************************************************************************************
  * @file    GPIO.h
  * @author  IUT Informatique La Rochelle
  * @version v1.0
  * @date    2023
  * @modification	
  * @brief   configuration des ports GPIO
  ******************************************************************************************************************/
	
#ifndef __GPIO_H
#define __GPIO_H

void initGPIO(void);//Initialisation des ports

int joytickUpPressed(void);
int joytickDownPressed(void);
int joytickSelectPressed(void);
int joytickLeftPressed(void);
int joytickRightPressed(void);
int userButtonPressed(void);

void startInterruptBP_USER(void);
void stopInterruptBP_USER(void);



#endif
