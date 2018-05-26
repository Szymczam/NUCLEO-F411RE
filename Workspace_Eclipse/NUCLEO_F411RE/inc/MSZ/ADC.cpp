/*
 * ADC.c
 *
 *  Created on: 30 mar 2018
 *      Author: Marek
 */

#include "ADC.h"

mADC PC_ADC;

mADC::~mADC() {
};

mADC::mADC() {
};


void mADC::Clock_enable(){
	__HAL_RCC_ADC1_CLK_ENABLE();
}

void mADC::ADC_Init(){
	ADCx.Instance = ADC1;

	// Enable clock for ADC
	Clock_enable();

	// Do initialization
	ADCx.Init.Resolution = ADC_RESOLUTION_12B;
	ADCx.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADCx.Init.ScanConvMode = DISABLE;
	ADCx.Init.ContinuousConvMode = DISABLE;
	ADCx.Init.DiscontinuousConvMode = DISABLE;
	ADCx.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	ADCx.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	ADCx.Init.DMAContinuousRequests = DISABLE;
	ADCx.Init.NbrOfDiscConversion = 0;
	ADCx.Init.NbrOfConversion = 1;
	ADCx.Init.EOCSelection = DISABLE;
	ADCx.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
	HAL_ADC_Init(&ADCx);
}

void mADC::ADC_Init_IT(uint8_t channel){
	ADC_ChannelConfTypeDef ADCChannel;
	ADCx.Instance = ADC1;

	// Enable clock for ADC
	Clock_enable();

	// Do initialization
	ADCx.Init.ClockPrescaler 		= ADC_CLOCKPRESCALER_PCLK_DIV8;
	ADCx.Init.Resolution 			= ADC_RESOLUTION_12B;
	ADCx.Init.DataAlign 			= ADC_DATAALIGN_RIGHT;
	ADCx.Init.ScanConvMode 			= DISABLE;
	ADCx.Init.ContinuousConvMode 	= DISABLE;
	ADCx.Init.DiscontinuousConvMode = DISABLE;
	ADCx.Init.ExternalTrigConvEdge 	= ADC_EXTERNALTRIGCONVEDGE_RISING;
	ADCx.Init.ExternalTrigConv 		= ADC_EXTERNALTRIGCONV_T3_TRGO;
	ADCx.Init.DMAContinuousRequests = ENABLE;
	ADCx.Init.NbrOfDiscConversion 	= 0;
	ADCx.Init.NbrOfConversion		= 1;
	ADCx.Init.EOCSelection 			= ADC_EOC_SEQ_CONV;
	HAL_ADC_Init(&ADCx);

	HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADC_IRQn);


	// Configure ADC regular channel
	ADCChannel.Channel 		= channel;
	ADCChannel.Rank 		= 1;
	ADCChannel.SamplingTime = ADC_SAMPLETIME_15CYCLES;


	HAL_ADC_ConfigChannel(&ADCx, &ADCChannel);


	HAL_ADC_Start_IT(&ADCx);
}


uint16_t mADC::ADC_Read(uint32_t channel){
	ADC_ChannelConfTypeDef ADCChannel;

	// Configure ADC regular channel
	ADCChannel.Channel = (uint8_t) channel;
	ADCChannel.Rank = 1;
	ADCChannel.SamplingTime = ADC_SAMPLETIME_15CYCLES;
//	ADCChannel.Offset = 0;

	// Return zero
	if (HAL_ADC_ConfigChannel(&ADCx, &ADCChannel) != HAL_OK) {
		return 0;
	}

	// Start conversion
	if (HAL_ADC_Start(&ADCx) != HAL_OK) {
		return 0;
	}

	// Poll for end
	HAL_ADC_PollForConversion(&ADCx, 10);

	return HAL_ADC_GetValue(&ADCx);
}

