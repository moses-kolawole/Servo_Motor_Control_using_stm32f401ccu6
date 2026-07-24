#include "main.h"

void Delay_ms(uint32_t time);

int main(void)
{
	// Enable GPIOA Clock
	RCC->AHB1ENR |= (1 << 0);

	// Configure PA6 as Alternate Function
	GPIOA->MODER &= ~(3 << (6 * 2));
	GPIOA->MODER |=  (2 << (6 * 2));

	// Select Alternate Function AF2 (TIM3)
	GPIOA->AFR[0] &= ~(0xF << (6 * 4));
	GPIOA->AFR[0] |=  (2 << (6 * 4));

	// Enable TIM3 Clock
	RCC->APB1ENR |= (1 << 1);

	// Configure Timer Speed
	TIM3->PSC = 15;

	// Configure PWM Period (20 ms = 50 Hz)
	TIM3->ARR = 19999;

	// Initial Pulse Width
	TIM3->CCR1 = 1000;

	// Configure Channel 1 as PWM Mode 1
	TIM3->CCMR1 &= ~(7 << 4);
	TIM3->CCMR1 |=  (6 << 4);

	// Enable Output Compare Preload
	TIM3->CCMR1 |= (1 << 3);

	// Enable Channel 1 Output
	TIM3->CCER |= (1 << 0);

	// Enable Auto Reload Preload
	TIM3->CR1 |= (1 << 7);

	// Generate Update Event
	TIM3->EGR |= (1 << 0);

	// Clear Update Flag
	TIM3->SR &= ~(1 << 0);

	// Start Timer
	TIM3->CR1 |= (1 << 0);

	while(1)
	{
		// Move from 0° to 180°
		for(uint16_t pulse = 1000; pulse <= 2000; pulse += 10)
		{
			TIM3->CCR1 = pulse;
			Delay_ms(20);
		}

		// Move from 180° back to 0°
		for(uint16_t pulse = 2000; pulse >= 1000; pulse -= 10)
		{
			TIM3->CCR1 = pulse;
			Delay_ms(20);
		}
	}
}

void Delay_ms(uint32_t time)
{
	for(uint32_t i = 0; i < time; i++)
	{
		for(volatile uint32_t j = 0; j < 16000; j++);
	}
}
