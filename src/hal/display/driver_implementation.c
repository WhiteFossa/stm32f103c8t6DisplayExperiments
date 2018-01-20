/*
 * driver.c
 *
 *  Created on: 12 янв. 2018 г.
 *      Author: fossa
 */

#include "driver_implementation.h"

void InitializeDisplay()
{
	// Clocking up port A
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	// Full speed for needed pins
	GPIOA->CRL &= ~(GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0 | GPIO_CRL_CNF0_1 | GPIO_CRL_CNF0_0
					| GPIO_CRL_MODE1_1 | GPIO_CRL_MODE1_0 | GPIO_CRL_CNF1_1 | GPIO_CRL_CNF1_0
					| GPIO_CRL_MODE2_1 | GPIO_CRL_MODE2_0 | GPIO_CRL_CNF2_1 | GPIO_CRL_CNF2_0
					| GPIO_CRL_MODE3_1 | GPIO_CRL_MODE3_0 | GPIO_CRL_CNF3_1 | GPIO_CRL_CNF3_0
					| GPIO_CRL_MODE4_1 | GPIO_CRL_MODE4_0 | GPIO_CRL_CNF4_1 | GPIO_CRL_CNF4_0
					| GPIO_CRL_MODE5_1 | GPIO_CRL_MODE5_0 | GPIO_CRL_CNF5_1 | GPIO_CRL_CNF5_0
					| GPIO_CRL_MODE6_1 | GPIO_CRL_MODE6_0 | GPIO_CRL_CNF6_1 | GPIO_CRL_CNF6_0
					| GPIO_CRL_MODE7_1 | GPIO_CRL_MODE7_0 | GPIO_CRL_CNF7_1 | GPIO_CRL_CNF7_0);

	// Full speed push-pull
	GPIOA->CRL |= (GPIO_CRL_MODE0_0 | GPIO_CRL_MODE0_1
					| GPIO_CRL_MODE1_0 | GPIO_CRL_MODE1_1
					| GPIO_CRL_MODE2_0 | GPIO_CRL_MODE2_1
					| GPIO_CRL_MODE3_0 | GPIO_CRL_MODE3_1
					| GPIO_CRL_MODE4_0 | GPIO_CRL_MODE4_1
					| GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1
					| GPIO_CRL_MODE6_0 | GPIO_CRL_MODE6_1
					| GPIO_CRL_MODE7_0 | GPIO_CRL_MODE7_1);

	GPIOA->CRH &= ~(GPIO_CRH_MODE8_1 | GPIO_CRH_MODE8_0 | GPIO_CRH_CNF8_1 | GPIO_CRH_CNF8_0
						| GPIO_CRH_MODE9_1 | GPIO_CRH_MODE9_0 | GPIO_CRH_CNF9_1 | GPIO_CRH_CNF9_0
						| GPIO_CRH_MODE10_1 | GPIO_CRH_MODE10_0 | GPIO_CRH_CNF10_1 | GPIO_CRH_CNF10_0
						| GPIO_CRH_MODE11_1 | GPIO_CRH_MODE11_0 | GPIO_CRH_CNF11_1 | GPIO_CRH_CNF11_0);

	GPIOA->CRH |= (GPIO_CRH_MODE8_0 | GPIO_CRH_MODE8_1
						| GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1
						| GPIO_CRH_MODE10_0 | GPIO_CRH_MODE10_1
						| GPIO_CRH_MODE11_0 | GPIO_CRH_MODE11_1);

	// Switching display on
	SendToDisplay(0b00111111 | (1 << STD_COMMAND));
}

void Delay(void)
{
	volatile uint32_t timer = 0x3FFF;
	while (timer--);
}

void SendToDisplay(uint16_t data)
{
	// Both controllers off
	uint16_t toPort = (data & DISP_DATA_MASK) | (1 << DISP_CS1) | (1 << DISP_CS2);

	if ((data & (1 << STD_COMMAND)) != 0)
	{
		// Command, writing to both controllers
		toPort &= ~(BV(DISP_CS1) | BV(DISP_CS2));
	}
	else
	{
		// Data
		toPort |= (1 << DISP_DC);

		// To what controller?
		if ((data & (1 << STD_LEFT_CTRLR)) != 0)
		{
			toPort &= ~BV(DISP_CS1);
		}
		else
		{
			toPort &= ~BV(DISP_CS2);
		}
	}

	GPIOA->ODR = toPort;

	Delay();

	toPort |= BV(DISP_E);
	GPIOA->ODR = toPort;

	Delay();

	toPort &= ~BV(DISP_E);
	toPort |= (1 << DISP_CS1) | (1 << DISP_CS2);
	GPIOA->ODR = toPort;

	Delay();
}


