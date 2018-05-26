/*
 * USART.h
 *
 *  Created on: 30 mar 2018
 *      Author: Marek
 */

#ifndef MSZ_USART_H_
#define MSZ_USART_H_


#include "stm32f4xx.h"
#include "MSZ/GPIO.h"
#include <string.h>

//#define USART_MODE_CS
#define USART_MODE_IT
//#define USART_MODE_DMA


#define USART_NVIC_PRIORITY					0x06
#define USART_WRITE_DATA(USARTx, data)      ((USARTx)->DR = (data))

/* Definition for USARTx's NVIC */
#define USARTx_DMA_TX_IRQn                DMA1_Stream6_IRQn
#define USARTx_DMA_RX_IRQn                DMA1_Stream4_IRQn
#define USARTx_DMA_TX_IRQHandler          DMA1_Stream6_IRQHandler
#define USARTx_DMA_RX_IRQHandler          DMA1_Stream4_IRQHandler

/* Definition for USARTx's DMA */
#define USARTx_TX_DMA_STREAM              DMA1_Stream6
#define USARTx_RX_DMA_STREAM              DMA1_Stream4

#define USARTx_TX_DMA_CHANNEL             DMA_CHANNEL_4
#define USARTx_RX_DMA_CHANNEL             DMA_CHANNEL_4

// Wait for TX empty
#define USART_TXEMPTYe(USARTx)               ((USARTx)->SR & USART_FLAG_TXE)
#define USART_WAITe(USARTx)                  while (!USART_TXEMPTYe(USARTx))
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))


class USART {
public:
	USART();
    ~USART();
    uint8_t Received[50];
    UART_HandleTypeDef 	USART_handle;
    void Init(USART_TypeDef* port, uint32_t BaudRate);
    void Init_IT(USART_TypeDef* port, uint32_t BaudRate);
    void Init_DMA(USART_TypeDef* port, uint32_t BaudRate);
    void Puts(char* str);
    void Putc(volatile char c);
private:
    void Clock_enable();

};

extern USART PC_UART;


void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART6_IRQHandler(void);
void USARTx_DMA_RX_IRQHandler(void);
void USARTx_DMA_TX_IRQHandler(void);

int __io_putchar(int c){
	PC_UART.Putc(c);
	return c;
}


#endif /* MSZ_USART_H_ */

