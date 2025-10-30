/*
 * LEDS_cfg.c
 *
 *  Created on: Oct 22, 2025
 *      Author: sunbeam
 */


#include "LEDS.h"

const LED_CfgType LED_CfgParam[LED_UNITS] =
{
	// LED 1 Configurations
    {
    	GPIOA,
		GPIO_PIN_5
	},
	// LED 2 Configurations
	{
	    GPIOB,
		GPIO_PIN_13
    }
};
