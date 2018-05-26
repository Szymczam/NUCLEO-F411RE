/*
 * RCC.cpp
 *
 *  Created on: 12 kwi 2018
 *      Author: Marek
 */


#include "RCC.h"


MRCC::~MRCC() {
};

MRCC::MRCC() {

};


RCC_Result_t MRCC::SystemClock_Config(void) {

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	// Enable Power Control clock
	__HAL_RCC_PWR_CLK_ENABLE();
	// Set voltage scaling
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	// Enable HSE Oscillator and activate PLL with HSE as source
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE;
	if (RCC_OSCILLATORTYPE == RCC_OSCILLATORTYPE_HSE) {
		RCC_OscInitStruct.HSEState = RCC_HSE_ON;
		RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
		RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	} else {
		RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
		RCC_OscInitStruct.HSIState = RCC_HSI_ON;
		RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	}
	// Set PLL parameters
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLL_M;
	RCC_OscInitStruct.PLL.PLLN = RCC_PLL_N;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLL_P;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLL_Q;

	// Try to init
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		return RCC_Result_Error;
	}

	// Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	// Try to init
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK) {
		return RCC_Result_Error;
	}
	return RCC_Result_Ok;
}


uint32_t MRCC::GetClockSpeedinMHz(Clock_t clock) {
	// Return clock speed
	switch (clock) {
		case Clock_HSI:
			return HSI_VALUE/1000000;
			break;
		case Clock_HSE:
			return HSE_VALUE/1000000;
			break;
		case Clock_HCLK:
			return HAL_RCC_GetHCLKFreq()/1000000;
			break;
		case Clock_PCLK1:
			return HAL_RCC_GetPCLK1Freq()/1000000;
			break;
		case Clock_PCLK2:
			return HAL_RCC_GetPCLK2Freq()/1000000;
			break;
		case Clock_SYSCLK:
			return HAL_RCC_GetSysClockFreq()/1000000;
			break;
		default:
			return 0;
			break;
	}
}


uint32_t MRCC::Delay_init(void) {
	uint32_t c;

    /* Enable TRC */
    CoreDebug->DEMCR &= ~0x01000000;
    CoreDebug->DEMCR |=  0x01000000;

    /* Enable counter */
    DWT->CTRL &= ~0x00000001;
    DWT->CTRL |=  0x00000001;

    /* Reset counter */
    DWT->CYCCNT = 0;

	/* Check if DWT has started */
	c = DWT->CYCCNT;

	/* 2 dummys */
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");

	/* Return difference, if result is zero, DWT has not started */
	return (DWT->CYCCNT - c);
	/* Return OK */
	return 1;
}


void MRCC::Delay_us(__IO uint32_t micros) {
	uint32_t start = DWT->CYCCNT;

	/* Go to number of cycles for system */
	micros *= (HAL_RCC_GetHCLKFreq() / 1000000);

	/* Delay till end */
	while ((DWT->CYCCNT - start) < micros);
}

void MRCC::Delay_ms(uint32_t millis) {
	/* Use HAL library for delay ms purpose */
	HAL_Delay(millis);
}
