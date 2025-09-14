#include "main.h"

void EOS_init(uint32_t delay_ms){
	// Use SystemCoreClock for accurate timing
	uint32_t delay_ticks = (delay_ms * SystemCoreClock) / 1000;
	SysTick->LOAD = delay_ticks - 1;  // Load value should be n-1
	SysTick->VAL = 0;
	SysTick->CTRL = 7;  // Enable SysTick with processor clock and interrupt
}
