#include "main.h"


// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void TIM2_IRQHandler(void)
{
	// What happened?
	if ((TIM2->SR & TIM_SR_UIF) != 0x00)
	{
		// Timer underrun
		TIM2->SR &= ~TIM_SR_UIF;

		// Re-launch
		TIM2->CR1 |= TIM_CR1_CEN;

		ToggleLed();
	}
}

void ToggleLed(void)
{
	GPIOC->ODR ^= BV(13);
}

int main(int argc, char* argv[])
{
	// Hardware initialization
	SwitchClockToHSE();
	SwitchClockToPLL();
//	ActivateLSEClock();

	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

	GPIOC->CRH &= ~(GPIO_CRH_MODE13_1 | GPIO_CRH_MODE13_0 | GPIO_CRH_CNF13_1 | GPIO_CRH_CNF13_0);
	GPIOC->CRH |= (GPIO_CRH_MODE13_1 | GPIO_CRH_MODE13_0); // PC13 output

	// Clocking-up TIM2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	TIM2->PSC = 0xFFFF; // Prescaler, as slow as possible

	TIM2->ARR = 500; // Downcount from it to 0

	TIM2->CR1 |= TIM_CR1_ARPE; // Auto-preload
	TIM2->CR1 &= ~TIM_CR1_CMS; // Edge-aligned mode
	TIM2->CR1 |= TIM_CR1_DIR; // Downcounting
	TIM2->CR1 |= TIM_CR1_OPM; // One pulse mode
	TIM2->CR1 |= TIM_CR1_URS; // Interrupts only from underflow/overflow
	TIM2->CR1 &= ~TIM_CR1_UDIS; // Update enabled

	TIM2->DIER |= TIM_DIER_UIE; // Interrupts on underflow/overflow

	// Enabling TIM2 interrupts
	uint32_t prioritygroup = NVIC_GetPriorityGrouping();

	// Highest user IRQ priority (0), 1 sub-pri
	uint32_t priority = NVIC_EncodePriority(prioritygroup, 0, 1 );
	NVIC_SetPriority(TIM2_IRQn, priority);
	NVIC_EnableIRQ(TIM2_IRQn);

	// Starting timer
	TIM2->CR1 |= TIM_CR1_CEN;

	while(1) {}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
