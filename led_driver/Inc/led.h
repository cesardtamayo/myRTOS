/*
 * led.h
 *
 *  Created on: Sep 25, 2025
 *      Author: neno
 */

#ifndef LED_H_
#define LED_H_

//#include <ST/STM32F3xx/Include/stm32f302x8.h>
#include <ST/STM32F3xx/Include/stm32f3xx.h>
//#include "stm32f"

void led_init(void);
void led_on(void);
void led_off(void);

#endif /* LED_H_ */
