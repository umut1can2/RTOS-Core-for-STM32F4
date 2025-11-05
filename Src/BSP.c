/*
 * BSP.c
 *
 *  Created on: Oct 23, 2025
 *      Author: umutcanakyol
 */

#include "BSP.h"

/*
 * Led Support
 */
void BSP_LED_INIT(void)
{
	__asm("CPSID I");

	RCC->AHB1ENR |=  GPIOA_ENR;

	GPIOA->MODER &= ~(0x3 << USER_LED_MODER_POS);
	GPIOA->MODER |=  (0x1 << USER_LED_MODER_POS);

	__asm("CPSIE I");
}

void BSP_USER_LED_ON(void)
{
	GPIOA->ODR |= (0x1 << USER_LED_POS);
}


void BSP_USER_LED_OFF(void)
{
	GPIOA->ODR &= ~(0x1 << USER_LED_POS);
}

void BSP_USER_LED_TOGGLE(void)
{
	GPIOA->ODR ^= (0x1 << USER_LED_POS);
}

/*
 * Button Support
 */

void BSP_BTN_INIT(void)
{
	__asm("CPSID I");

	RCC->AHB1ENR |= GPIOC_ENR;

	GPIOC->PUPDR &= ~(0x3 << 26);
	GPIOC->PUPDR |= (0x1 << 26);   // 01b = Pull-UP (değişti!)

	GPIOC->MODER &= ~(0x3 << 26);

	__asm("CPSIE I");
}

uint8_t BSP_BTN_READ(void)
{
	return (GPIOC->IDR & (0x1 << 13)) ? 1 : 0;
}

/*
 * ADC Support
 */

void BSP_ADC1_INIT(void)
{
	__asm("CPSID I");
	RCC->AHB1ENR |= GPIOA_ENR;
	GPIOA->MODER |= (0x3 << 0); // PA0 as analog input

	RCC->APB2ENR |= ADC1_ENR;
	ADC1->CR2 = 0;
	ADC1->SQR3 = 0;
	ADC1->SQR1 = 0;
	ADC1->CR2 |= 1;

	__asm("CPSIE I");
}

uint32_t BSP_ADC1_Read(void)
{
	ADC1->CR2 |= (0x1 << 30);

	while(!(ADC1->SR & (0x1 << 1)));

	return ADC1->DR;
}


/*
 * Delay
 */

void BSP_Delay(uint32_t milliseconds)
{
	RCC->APB1ENR |= (0x1 << 1);
	TIM3->PSC = 160 - 1; // 16 000 000 -> 100 000 // saniyede 100kHz sayacak
	TIM3->ARR = (100-1);	// bIZ SAYACI 100 den baslaticaz 1 Milisaniye
	TIM3->CNT = 0;
	TIM3->CR1 = 1;

	for(int i = 0; i < milliseconds; i++)
	{
		while(!(TIM3->SR & 1)){};
		TIM3->SR &= ~1;
	}
}

/*
 * Probe support for Osilators
 */

void BSP_PROBES_INIT()
{
	RCC->AHB1ENR |= (0x1 << 2);
	GPIOC->MODER &= ~(0xF << 0);
	GPIOC->MODER |= (0x1 << 0) | (0x1 << 2);
}

void BSP_PROBE0_PC0()
{
	GPIOC->ODR ^= (0x1 << 0);
}

void BSP_PROBE0_PC1()
{
	GPIOC->ODR ^= (0x1 << 1);
}

void UART_INIT(void)
{
	RCC->APB1ENR |= (0x1 << 17);
	RCC->AHB1ENR |= (0x1 << 0);

	GPIOA->MODER &= ~(0xF << 4);
	GPIOA->MODER |= (0xA << 4);
	GPIOA->AFR[0] &= ~(0xFF << 8);
	GPIOA->AFR[0] |= (0b0111 << 8) | (0b0111 << 12);

	USART2->BRR = 0X683; // 9600
	USART2->CR1 |= (0x1 << 3); // TransE
	USART2->CR1 |= (0x1 << 2); // RecivE
	USART2->CR1 |= (0x1 << 13); // UE
}

void UART_SEND_CHAR(uint8_t chr)
{
	while(!(USART2->SR & (0x1 << 7))); // wait
	USART2->DR = chr;
}

uint8_t UART_READ_CHAR(void)
{
	while(!(USART2->SR & (0x1 << 5)));
	return USART2->DR;
}



void uprint(const char *c)
{
	while(*c != 0)
	{
		UART_SEND_CHAR((uint8_t)*c);
		c++;
	}
}


void uget(char* buff, uint8_t size)
{
	uint8_t i = 0;
	uint8_t c;
	while(1)
	{
		c = UART_READ_CHAR();
		if(c == '\r' || c == 'n')
		{
			UART_SEND_CHAR('\r');
			UART_SEND_CHAR('\n');
			buff[i] = '\0';
			break;
		}
		else
		{
			if (i >= size - 1) {
				buff[i] = '\0';
				break;
			}
			else
			{
				buff[i++] = c;
				UART_SEND_CHAR(c);
			}
		}
	}
}
uint8_t strcount(const char* c)
{
	uint8_t t = 0;
	while(*c != '\0')
	{
		t++;
		c++;
	}
	return t;
}

uint8_t streq( const  char* restrict c1,  const  char* restrict c2)
{
	uint8_t size = strcount(c1);
	if(size != strcount(c2))
		return 0;

	for(int i = 0; i < size; i++)
		if(c1[i] != c2[i])
			return 0;

	return 1;
}
