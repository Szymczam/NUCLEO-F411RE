/*
 * SPI.cpp
 *
 *  Created on: 8 kwi 2018
 *      Author: Marek
 */
#include <SPI.h>

SPI ePAPER_SPI;
SPI OLED_SPI;

SPI::~SPI() {
};

SPI::SPI() {
};


void SPI::Clock_enable(){
	if 		(SPIx.Instance == SPI1)	__HAL_RCC_SPI1_CLK_ENABLE();
	else if (SPIx.Instance == SPI2)	__HAL_RCC_SPI2_CLK_ENABLE();
	else if (SPIx.Instance == SPI3)	__HAL_RCC_SPI3_CLK_ENABLE();
	else if (SPIx.Instance == SPI4)	__HAL_RCC_SPI4_CLK_ENABLE();
	else if (SPIx.Instance == SPI5)	__HAL_RCC_SPI5_CLK_ENABLE();

}

void SPI::SPI_init(SPI_TypeDef* port, uint32_t BaudRatePrescaler){
	SPIx.Instance = port;

	// Enable clock for GPIO
	Clock_enable();

	// Do initialization
	SPIx.Init.Mode = SPI_MODE_MASTER;
	SPIx.Init.Direction = SPI_DIRECTION_1LINE;
	SPIx.Init.DataSize = SPI_DATASIZE_8BIT;
	SPIx.Init.CLKPolarity = SPI_POLARITY_LOW;
	SPIx.Init.CLKPhase = SPI_PHASE_1EDGE;
	SPIx.Init.NSS = SPI_NSS_SOFT;
	SPIx.Init.BaudRatePrescaler = BaudRatePrescaler;
	SPIx.Init.FirstBit = SPI_FIRSTBIT_MSB;
	SPIx.Init.TIMode = SPI_TIMODE_DISABLE;
	SPIx.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	SPIx.Init.CRCPolynomial = 10;
	HAL_SPI_Init(&SPIx);
}

void SPI::SPI_Send(uint8_t data) {
	HAL_SPI_Transmit(&SPIx, &data, 1, 1000);

}


