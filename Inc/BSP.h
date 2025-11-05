/*
 * BSP.h
 *
 *  Created on: Oct 23, 2025
 *      Author: umutcanakyol
 */

#ifndef INC_BSP_H_
#define INC_BSP_H_
#define USER_LED 			(0x5U << 0) // PA5
#define USER_LED_POS		(5U)
#define USER_LED_MODER_POS	(10U)
#define USER_BTN			(0xD << 0) // PC13
#define USER_BTN_MODER_POS	(26U)
#define GPIOA_ENR 			(0x1U << 0)
#define GPIOC_ENR			(0x1 << 2)
#define ADC1_ENR			(0x1 << 8)

#include "stm32f411xe.h"
#include <stdint.h>


void BSP_LED_INIT(void);
void BSP_USER_LED_ON(void);
void BSP_USER_LED_OFF(void);
void BSP_USER_LED_TOGGLE(void);
void BSP_BTN_INIT(void);
uint8_t BSP_BTN_READ(void);
void BSP_ADC1_INIT(void);
uint32_t BSP_ADC1_Read(void);
void BSP_Delay(uint32_t milliseconds);
void BSP_PROBES_INIT();
void BSP_PROBE0_PC0();
void BSP_PROBE0_PC1();
void UART_INIT(void);
void UART_SEND_CHAR(uint8_t chr);
uint8_t UART_READ_CHAR(void);
void uprint(const char *c);
void uget(char* buff, uint8_t size);
uint8_t strcount(const char* c);
uint8_t streq(const char* restrict c1, const char* restrict c2);

#endif /* INC_BSP_H_ */
