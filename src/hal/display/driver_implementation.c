/*
 * driver.c
 *
 *  Created on: 12 янв. 2018 г.
 *      Author: fossa
 */

#include "driver_implementation.h"

void InitializeDisplay(void)
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

	// Clocking-up TIM2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	TIM2->PSC = DISP_TIMER_PRESCALER; // Prescaler, with 36 MHz timer we will have near 100ns per tick

	// Auto-preload, downcounting, one pulse mode, interrupts only from underflow/overflow
	TIM2->CR1 |= TIM_CR1_ARPE | TIM_CR1_DIR | TIM_CR1_OPM | TIM_CR1_URS;

	// Edge-aligned mode, update enabled
	TIM2->CR1 &= ~(TIM_CR1_CMS | TIM_CR1_UDIS);

	TIM2->DIER |= TIM_DIER_UIE; // Interrupts on underflow/overflow

	// Enabling TIM2 interrupts
	uint32_t prioritygroup = NVIC_GetPriorityGrouping();

	// Highest user IRQ priority (0), 1 sub-pri
	uint32_t priority = NVIC_EncodePriority(prioritygroup, 0, 1 );
	NVIC_SetPriority(TIM2_IRQn, priority);
	NVIC_EnableIRQ(TIM2_IRQn);

	// Clear framebuffer
	ClearFramebuffer();

	// Now we are ready to send commands.
	SendToDisplayState = Idle;
}

void TIM2_IRQHandler(void)
{
	// What happened?
	if ((TIM2->SR & TIM_SR_UIF) != 0x00)
	{
		// Timer underrun
		TIM2->SR &= ~TIM_SR_UIF;

		// What state we have?
		switch (SendToDisplayState)
		{
			case DataSent:
				// Data sent, time to set strobe
				GPIOA->ODR |= BV(DISP_E);

				SendToDisplayState = StrobeSet;

				// Re-launch
				TIM2->ARR = DISP_STROBE_LENGTH;
				TIM2->CR1 |= TIM_CR1_CEN;

				break;
			case StrobeSet:
				// Strobe set, time to clear it
				GPIOA->ODR = (GPIOA->ODR & ~BV(DISP_E)) | (1 << DISP_CS1) | (1 << DISP_CS2);

				SendToDisplayState = StrobeCleared;

				// Re-launch
				TIM2->ARR = DISP_STROBE_END_TO_IDLE_TIME;
				TIM2->CR1 |= TIM_CR1_CEN;

				break;
			case StrobeCleared:
				// Transmission completed
				SendToDisplayState = Idle;
				break;
		}
	}
}

uint8_t SendToDisplay(uint16_t data)
{
	// Checking state
	if (SendToDisplayState != Idle)
	{
		return 0x00U;
	}

	// Setting port data
	SendToDisplayState = DataSent;

	// Both controllers off
	uint16_t toPort = (data & DISP_DATA_MASK) | (1 << DISP_CS1) | (1 << DISP_CS2);

	if ((data & (1 << DISP_STD_COMMAND)) != 0)
	{
		// Command, writing to both controllers
		toPort &= ~(BV(DISP_CS1) | BV(DISP_CS2));
	}
	else
	{
		// Data
		toPort |= (1 << DISP_DC);

		// To what controller?
		if ((data & (1 << DISP_STD_LEFT_CTRLR)) != 0)
		{
			toPort &= ~BV(DISP_CS1);
		}
		else
		{
			toPort &= ~BV(DISP_CS2);
		}
	}

	GPIOA->ODR = toPort;

	// Starting timecount until strobe
	TIM2->ARR = DISP_DATA_TO_STROBE_TIME;
	TIM2->CR1 |= TIM_CR1_CEN;

	return 0xFFU;
}

void SendToDisplayWithWait(uint16_t data)
{
	while (0x00 == SendToDisplay(data)) {};
}

void DisplayOnOff(uint8_t on)
{
	uint16_t command = 0b00111110 | BV(DISP_STD_COMMAND);

	if (on != 0x00)
	{
		command |= BV(0);
	}

	SendToDisplayWithWait(command);
}

void DisplaySetYAddress(uint8_t addr)
{
	uint16_t command = 0b01000000 | BV(DISP_STD_COMMAND);

	command |= (addr & DISP_Y_ADDRESS_MASK);

	SendToDisplayWithWait(command);
}

void DisplaySetXAddress(uint8_t addr)
{
	uint16_t command = 0b10111000 | BV(DISP_STD_COMMAND);

	command |= addr & DISP_X_ADDRESS_MASK;

	SendToDisplayWithWait(command);
}

void DisplaySetStartLine(uint8_t line)
{
	uint16_t command = 0b11000000 | BV(DISP_STD_COMMAND);

	command |= line & DISP_START_LINE_MASK;

	SendToDisplayWithWait(command);
}

void PushFramebuffer(void)
{
	uint16_t stripeBeginAddr;

	for (uint8_t hStripe = 0; hStripe < DISP_FRAMEBUFFER_H_STRIPES; hStripe ++)
	{
		stripeBeginAddr = hStripe * 2 * DISP_FRAMEBUFFER_H_STRIPE_WIDTH;

		DisplaySetXAddress(hStripe);

		// Starting from begin of stripe, left to right
		DisplaySetYAddress(0U);

		for(uint8_t v = 0; v < DISP_FRAMEBUFFER_H_STRIPE_WIDTH; v++)
		{
			// Left
			SendToDisplayWithWait(Framebuffer[stripeBeginAddr + v] | BV(DISP_STD_LEFT_CTRLR));

			// Right
			SendToDisplayWithWait(Framebuffer[stripeBeginAddr + v + DISP_FRAMEBUFFER_H_STRIPE_WIDTH]);
		}
	}
}

void DrawPixel(uint16_t y, uint16_t x)
{
	if ((y >= DISPLAY_Y_SIZE) || (x >= DISPLAY_X_SIZE))
	{
		return;
	}

	uint8_t stripe = y / DISP_FRAMEBUFFER_H_STRIPE_HEIGHT;
	uint8_t posInStripe = y % DISP_FRAMEBUFFER_H_STRIPE_HEIGHT;

	uint16_t fbAddr = stripe * 2 * DISP_FRAMEBUFFER_H_STRIPE_WIDTH + x;

	if (ActiveColor != 0x00)
	{
		Framebuffer[fbAddr] |= BV(posInStripe);
	}
	else
	{
		Framebuffer[fbAddr] &= ~BV(posInStripe);
	}
}

void SetActiveColor(uint32_t color)
{
	ActiveColor = color & DISP_COLOR_MASK;
}

void ClearFramebuffer(void)
{
	memset(Framebuffer, 0x00, DISP_FRAMEBUFFER_SIZE);
}

