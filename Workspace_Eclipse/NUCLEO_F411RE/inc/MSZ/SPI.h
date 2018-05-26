/*
 * SPI.h
 *
 *  Created on: 8 kwi 2018
 *      Author: Marek
 */

#ifndef MSZ_SPI_H_
#define MSZ_SPI_H_

#include "stm32f4xx.h"


#define SPI_WAIT_TXe(SPIx)                   while ((SPIx->SR & SPI_FLAG_TXE) == 0 || (SPIx->SR & SPI_FLAG_BSY))
#define SPI_WAIT_RXe(SPIx)                   while ((SPIx->SR & SPI_FLAG_RXNE) == 0 || (SPIx->SR & SPI_FLAG_BSY))
#define SPI_CHECK_ENABLED_RESP(SPIx, val)   if (!((SPIx)->CR1 & SPI_CR1_SPE)) {return (val);}

class SPI {
public:
	SPI();
    ~SPI();

    SPI_HandleTypeDef	SPIx;

    void SPI_init(SPI_TypeDef* port, uint32_t BaudRatePrescaler);
    void SPI_Send(uint8_t data);


private:
	void Clock_enable();
};

extern SPI ePAPER_SPI;
extern SPI OLED_SPI;


#endif /* MSZ_SPI_H_ */
