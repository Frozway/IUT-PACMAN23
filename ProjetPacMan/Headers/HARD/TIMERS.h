/**
  ******************************************************************************************************************
  * @file    TIMERS.h
  * @author  IUT Informatique La Rochelle
  * @version v1.0
  * @date    2023
  * @modification	
  * @brief   configuration des timers
  ******************************************************************************************************************/
	

#ifndef __TIMERS_H
#define __TIMERS_H

#include "stm32f10x.h"                  /* STM32F10x.h definitions            */

#define UIE (1<<0)
#define UIF (1<<0)
#define CEN (1<<0)


/**
  * @brief  configuration du timer1 pour générer des interruptions
  * @param  None
  * @retval None
  */
void initTIMERS(void);
void cfgTimer1(void);
void cfgTimer2(void);
#endif
