/*
 * uart.c
 *
 *  Created on: Sep 27, 2025
 *      Author: neno
 */

#include <stdio.h>
#include <stdint.h>
#include "uart.h"

#define GPIOAEN 		(1U<<17)	// GPIOA enable
#define USART2EN 		(1U<<17)	// USART2 enable
#define SYS_FREQ 		8000000
#define APB1_CLK		SYS_FREQ
#define USART_BAUDRATE	115200
#define CR1_TE 			(1U<<3)	 	// Tx enable
#define CR1_UE			(1U<<0)		// USART enable
#define ISR_TXE 		(1U<<7)     // TXE: Transmit Data Register Empty


static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate){
	return ((periph_clk + (baudrate/2U))/baudrate);
}

static void set_uart_bd(uint32_t periph_clk, uint32_t baudrate){
	USART2->BRR = compute_uart_bd(periph_clk, baudrate);
}

static void uart_write(int ch){
	/**
	 * 1. Checking transmit data register is empty first.
	 * 2. Write data to register
	 */

	while (!(USART2->ISR & ISR_TXE)){}
	USART2->TDR = (ch & 0xFF);
}


// Retargettig printf()
int __io_putchar(int ch){
	uart_write(ch);
	return ch;
}


void uart_tx_init(void){
	/**
	 * Using USART2 because it's connected to the ST-Link Virtual COM Port
	 * Setting Alternate Function 7(AF7) modes in PA14/PA15 so they act as USART2_TX/USART2_RX
	 *
	 */

	// Enable Clock access to GPIOA
	RCC->AHBENR |= GPIOAEN;
	// 1. Configure PA2 mode to alternate function mode
	// PA2 MODER: Bits 5:4. (2 * 2) = 4, (2 * 2) + 1 = 5
	GPIOA->MODER &= ~(1U<<4); // Clear bit 4 (PA2 is 0b10)
	GPIOA->MODER |= (1U<<5);  // Set bit 5

	// 2. Set alternate function type for PA2 to AF7 (USART2_TX)
	// PA2 is in AFR[0] (low register). AF7 is 0111.
	// PA2 bits are 8, 9, 10, 11
	GPIOA->AFR[0] |= (1U<<8);   // AFR2[0] = 1
	GPIOA->AFR[0] |= (1U<<9);   // AFR2[1] = 1
	GPIOA->AFR[0] |= (1U<<10);  // AFR2[2] = 1
	GPIOA->AFR[0] &= ~(1U<<11); // AFR2[3] = 0. AF7 = 0111


// Enable clock access to USART
	RCC->APB1ENR |= USART2EN;

//	Configure baud rate
	set_uart_bd(SYS_FREQ, USART_BAUDRATE);

//	Configure transfer direction
	USART2->CR1 = CR1_TE; // not using friendly assignment because only that bit should be 1.
//	Enable USART module
	USART2->CR1 |= CR1_UE;
}





