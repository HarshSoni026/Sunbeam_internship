/*
 * LEDS.h
 *
 *  Created on: Oct 22, 2025
 *      Author: sunbeam
 */

#ifndef INC_LEDS_H_
#define INC_LEDS_H_

#include "stm32f4xx_hal.h"


// The Number OF LED Units To Be Used In The Project
#define LED_UNITS  2


typedef struct
{
	GPIO_TypeDef * LED_GPIO;
	/*
	 * GPIOA
	 * GPIOB
	 * ...
	 * */
	uint16_t       LED_PIN;
	/*
	 * GPIO_PIN_0
	 * GPIO_PIN_1
	 * ...
	 * */
}LED_CfgType;


/*-----[ Prototypes For All Functions ]-----*/

void LEDs_Init(void);
void LED_ON(uint16_t au16_Instance);
void LED_OFF(uint16_t au16_Instance);


#endif /* INC_LEDS_H_ */
