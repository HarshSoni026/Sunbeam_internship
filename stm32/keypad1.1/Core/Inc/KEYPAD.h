/*
 * KEYPAD.h
 *
 *  Created on: Oct 21, 2025
 *      Author: sunbeam
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#define HAL_GPIO_MODULE_ENABLED

#include "stm32f4xx_hal.h"

#define KEYPAD_UNITS 1
#define ROWS 		 4
#define COLS 		 4
#define KEYS		 16

#define KEY_PRESSED  1
#define KEY_RELEASED 0

typedef struct{
	GPIO_TypeDef * ROW_GPIO[ROWS];
	uint16_t 	   ROW_PIN[ROWS];
	GPIO_TypeDef * COL_GPIO[COLS];
	uint16_t 	   COL_PIN[COLS];
}KEYPAD_CfgType;



void KEYPAD_Init(uint16_t au16_Instance, uint8_t* au8_KeyStates);
void KEYPAD_Scan(uint16_t au16_Instance);


#endif /* INC_KEYPAD_H_ */
