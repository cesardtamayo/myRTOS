/*
 * led.c
 *
 *  Created on: Sep 25, 2025
 *      Author: neno
 */

#include "led.h"

#define GPIOAEN (1U<<17)
#define GPIOBEN (1U<<18)
#define LED_PIN5 (1U<<5)
#define LED_PIN13 (1U<<13)

void led_init(void){
	// Enabling clock access to led port (PortA):
//	RCC->AHBENR |= GPIOAEN;
	RCC->AHBENR |= GPIOBEN;

	// Set led pin as output pin:
//	GPIOA->MODER |=  (1U<<10);
//	GPIOA->MODER &= ~(1U<<11);

	GPIOB->MODER |=  (1U<<26);
	GPIOB->MODER &= ~(1U<<27);
}

void led_on(void){
	// Set led pin (PA5) HIGH
//	GPIOA->ODR |= LED_PIN5;
	GPIOB->ODR |= LED_PIN13;
}

void led_off(void){
	// Set led pin (PA5) LOW
//	GPIOA->ODR &= ~LED_PIN5;
	GPIOB->ODR &= ~LED_PIN13;
}
