/*
 * SYS.cpp
 *
 *  Created on: 12 kwi 2018
 *      Author: Marek
 */

#include "SYS.h"


mSYS::~mSYS() {
};

mSYS::mSYS() {

};

mSYS SYS;

uint8_t mSYS::CPU_LOAD_Init() {
	// Set values to 0
	CPU.Load = 0;
	CPU.SCNT = 0;
	CPU.WCNT = 0;
	CPU.Updated = 0;

	// Return DWT counter enabled status
	return DWTCounterEnable();
}


uint8_t mSYS::DWTCounterEnable(void) {
	uint32_t c;

    // Enable TRC
    CoreDebug->DEMCR &= ~0x01000000;
    CoreDebug->DEMCR |=  0x01000000;

    // Enable counter
    DWT->CTRL &= ~0x00000001;
    DWT->CTRL |=  0x00000001;

    // Reset counter
    DWT->CYCCNT = 0;

	// Check if DWT has started
	c = DWT->CYCCNT;

	// 2 dummys
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");

	// Return difference, if result is zero, DWT has not started
	return (DWT->CYCCNT - c);
}


uint8_t mSYS::CPU_LOAD_GoToSleepMode() {
	uint32_t t;
	static uint32_t l = 0;
	static uint32_t WorkingTime = 0;
	static uint32_t SleepingTime = 0;
	uint8_t irq_status;

	// Add to working time
	WorkingTime += DWT->CYCCNT - l;

	// Save count cycle time
	t = DWT->CYCCNT;

	// Get interrupt status
	irq_status = __get_PRIMASK();

	// Disable interrupts
	__disable_irq();

	// Go to sleep mode
	// Wait for wake up interrupt, systick can do it too
	__WFI();

	// Increase number of sleeping time in CPU cycles
	SleepingTime += DWT->CYCCNT - t;

	// Save current time to get number of working CPU cycles
	l = DWT->CYCCNT;

	// Enable interrupts, process/execute an interrupt routine which wake up CPU
	if (!irq_status) {
		__enable_irq();
	}

	// Reset flag
	CPU.Updated = 0;

	// Every 1000ms print CPU load via USART
	if ((SleepingTime + WorkingTime) >= HAL_RCC_GetHCLKFreq()) {
		/* Save values */
		CPU.SCNT = SleepingTime;
		CPU.WCNT = WorkingTime;
		CPU.Load = ((float)WorkingTime / (float)(SleepingTime + WorkingTime) * 100);
		CPU.Updated = 1;

		// Reset time
		SleepingTime = 0;
		WorkingTime = 0;
	}

	// Return updated status
	return CPU.Updated;
}

