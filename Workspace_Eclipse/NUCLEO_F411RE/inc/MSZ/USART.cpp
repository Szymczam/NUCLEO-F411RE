/*
 * USART.c
 *
 *  Created on: 30 mar 2018
 *      Author: Marek
 */

#include "USART.h"

 UART_HandleTypeDef huart1;
USART PC_UART;

USART::~USART() {
};

USART::USART() {
//     Received		=	0;
};

void USART::Clock_enable(){
	if 		(USART_handle.Instance == USART1)	__HAL_RCC_USART1_CLK_ENABLE();
	else if (USART_handle.Instance == USART2)	__HAL_RCC_USART2_CLK_ENABLE();
#ifdef USART_MODE_DMA
	if 		(USART_handle.Instance == USART1)	__HAL_RCC_DMA2_CLK_ENABLE();
	else if (USART_handle.Instance == USART2)	__HAL_RCC_DMA1_CLK_ENABLE();
#endif
}

void USART::Init(USART_TypeDef* port, uint32_t BaudRate){
	USART_handle.Instance = port;

	// Enable clock for UART
	Clock_enable();

	USART_handle.Init.BaudRate 		= BaudRate;
	USART_handle.Init.WordLength 	= UART_WORDLENGTH_8B;
	USART_handle.Init.Parity 		= UART_PARITY_NONE;
	USART_handle.Init.StopBits 		= UART_STOPBITS_1;
	USART_handle.Init.HwFlowCtl 	= UART_HWCONTROL_NONE;
	USART_handle.Init.OverSampling 	= UART_OVERSAMPLING_16;
	USART_handle.Init.Mode 			= UART_MODE_TX_RX;
	// Initialize USART
	HAL_UART_Init(&USART_handle);
	// Enable RX interrupt
	USART_handle.Instance->CR1 |= USART_CR1_RXNEIE;


	HAL_UART_Receive(&USART_handle, Received, 1, 100);

	printf("USART OK!\r\n");
}

void USART::Init_IT(USART_TypeDef* port, uint32_t BaudRate){
	USART_handle.Instance = port;

	// Enable clock for UART
	Clock_enable();

	USART_handle.Init.BaudRate 		= BaudRate;
	USART_handle.Init.WordLength 	= UART_WORDLENGTH_8B;
	USART_handle.Init.Parity 		= UART_PARITY_NONE;
	USART_handle.Init.StopBits 		= UART_STOPBITS_1;
	USART_handle.Init.HwFlowCtl 	= UART_HWCONTROL_NONE;
	USART_handle.Init.OverSampling 	= UART_OVERSAMPLING_16;
	USART_handle.Init.Mode 			= UART_MODE_TX_RX;
	// Initialize USART
	HAL_UART_Init(&USART_handle);
	// Enable RX interrupt
	USART_handle.Instance->CR1 |= USART_CR1_RXNEIE;

	// Enable IRQ
	if (USART_handle.Instance == USART1){
		HAL_NVIC_DisableIRQ(USART1_IRQn);
		HAL_NVIC_SetPriority(USART1_IRQn, USART_NVIC_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
		HAL_NVIC_ClearPendingIRQ(USART1_IRQn);
	}
	else if (USART_handle.Instance == USART2){
		HAL_NVIC_DisableIRQ(USART2_IRQn);
		HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
		HAL_NVIC_ClearPendingIRQ(USART2_IRQn);
	}

	HAL_UART_Receive_IT(&USART_handle, Received, 1);

	printf("USART OK!\r\n");
}

void USART::Init_DMA(USART_TypeDef* port, uint32_t BaudRate){
	static DMA_HandleTypeDef hdma_tx;
	static DMA_HandleTypeDef hdma_rx;

	USART_handle.Instance = port;

	// Enable clock for UART
	Clock_enable();

	USART_handle.Init.BaudRate = BaudRate;
	USART_handle.Init.WordLength = UART_WORDLENGTH_8B;
	USART_handle.Init.Parity = UART_PARITY_NONE;
	USART_handle.Init.StopBits = UART_STOPBITS_1;
	USART_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	USART_handle.Init.OverSampling = UART_OVERSAMPLING_16;
	USART_handle.Init.Mode = UART_MODE_TX_RX;
	// Init USART
	HAL_UART_Init(&USART_handle);

	hdma_tx.Instance                 = USARTx_TX_DMA_STREAM;
	hdma_tx.Init.Channel             = USARTx_TX_DMA_CHANNEL;
	hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
	hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_tx.Init.Mode                = DMA_NORMAL;
	hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
	hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
	hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;
	HAL_DMA_Init(&hdma_tx);
	__HAL_LINKDMA(&USART_handle, hdmatx, hdma_tx);

	hdma_rx.Instance                 = USARTx_RX_DMA_STREAM;
	hdma_rx.Init.Channel             = USARTx_RX_DMA_CHANNEL;
	hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
	hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_rx.Init.Mode                = DMA_NORMAL;
	hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
	hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
	hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;
	HAL_DMA_Init(&hdma_rx);
	__HAL_LINKDMA(&USART_handle, hdmarx, hdma_rx);

	HAL_NVIC_SetPriority(USARTx_DMA_TX_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USARTx_DMA_TX_IRQn);

	// NVIC configuration for DMA transfer complete interrupt (USARTx_RX)
	HAL_NVIC_SetPriority(USARTx_DMA_RX_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USARTx_DMA_RX_IRQn);

	// NVIC configuration for USART, to catch the TX complete
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART2_IRQn);

	char* kk = "DMA OK\n\r";
	HAL_UART_Transmit_DMA(&USART_handle, (uint8_t*)kk, strlen((char*)kk));
	HAL_UART_Receive_DMA(&USART_handle, (uint8_t*)kk, strlen((char*)kk));

	printf("Hello Mark!\r\n");
}


void USART::Puts(char* str) {

#ifdef USART_MODE_CS
	HAL_UART_Transmit(&USART_handle, (uint8_t*)(str),strlen(str), 100);
//	while (*str) {
//			USART_WAITe(USART_handle.Instance);								// Wait to be ready, buffer empty
//			USART_WRITE_DATA(USART_handle.Instance, (uint16_t)(*str++));	// Send data
//			USART_WAITe(USART_handle.Instance);								// Wait to be ready, buffer empty
//	}
#elif defined (USART_MODE_IT)
	HAL_UART_Transmit_IT(&USART_handle, (uint8_t*)(str),strlen(str));
#elif defined (USART_MODE_DMA)
	HAL_UART_Transmit_DMA(&USART_handle, (uint8_t*)(str),strlen(str));
#endif
}

void USART::Putc(volatile char c) {
	if ((USART_handle.Instance->CR1 & USART_CR1_UE)) {
		USART_WAITe(USART_handle.Instance);// Wait to be ready, buffer empty
		USART_WRITE_DATA(USART_handle.Instance, (uint16_t)(c & 0x01FF));// Send data
		USART_WAITe(USART_handle.Instance);// Wait to be ready, buffer empty
	}

}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle){

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
#ifdef USART_MODE_IT
	HAL_UART_Receive_IT(&PC_UART.USART_handle, PC_UART.Received, 1);
	HAL_UART_Transmit_IT(&PC_UART.USART_handle, PC_UART.Received, strlen((char*)PC_UART.Received));
#elif defined (USART_MODE_DMA)
	HAL_UART_Transmit_DMA(&PC_UART.USART_handle, PC_UART.Received, 1);
	HAL_UART_Receive_DMA(&PC_UART.USART_handle, PC_UART.Received, 1);
#endif
}


void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle){
	printf("USART ERROR\r\n");
	HAL_NVIC_ClearPendingIRQ(DMA1_Stream6_IRQn);
	HAL_NVIC_ClearPendingIRQ(DMA1_Stream4_IRQn);
	HAL_NVIC_ClearPendingIRQ(USART2_IRQn);
}
