/*
 * GPIO.cpp
 *
 *  Created on: 7 kwi 2018
 *      Author: Marek
 */


#include "GPIO.h"



GPIO LED;
GPIO LED_PWM;
GPIO BUTTON;
GPIO ePAPIER_DC;
GPIO ePAPIER_RST;
GPIO ePAPIER_CS;
GPIO ePAPIER_BUSY;
GPIO ePAPIER_CLK;
GPIO ePAPIER_DIN;
GPIO PC_UART2;
GPIO OLED_SPI3;
GPIO OLED_I2C1;
GPIO OLED_DC;
GPIO OLED_RST;
GPIO OLED_CS;
GPIO ADCA;
GPIO ADCB;
GPIO ADCC;

GPIO::~GPIO() {
};

GPIO::GPIO() {
     Pin		=	0;
     port		=	GPIOA;
};

void GPIO::Init_all(void){
	// LED		PA5
//	LED.GPIO_inite(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, 0x00);
	LED_PWM.GPIO_init(GPIOA, GPIO_PIN_5, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF1_TIM2);
	//BUTTON	PC13
	BUTTON.EXTI_init(GPIOC, GPIO_PIN_13, GPIO_MODE_IT_FALLING, GPIO_PULLUP, GPIO_SPEED_FREQ_LOW, 0x00);
	// e-Paper
	ePAPIER_DC.GPIO_init(GPIOB, GPIO_PIN_3, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, 0x00);
	ePAPIER_RST.GPIO_init(GPIOB, GPIO_PIN_2, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, 0x00);
	ePAPIER_CS.GPIO_init(GPIOB, GPIO_PIN_1, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, 0x00);
	ePAPIER_BUSY.GPIO_init(GPIOB, GPIO_PIN_4, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, 0x00);
	ePAPIER_CLK.GPIO_init(GPIOB, GPIO_PIN_10, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF5_SPI2);
	ePAPIER_DIN.GPIO_init(GPIOC, GPIO_PIN_3, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF5_SPI2);
	// USART2 PC	TX PA2, RX PA3
	PC_UART2.GPIO_init(GPIOA, GPIO_PIN_2 | GPIO_PIN_3, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF7_USART2);
	// OLED				SPI3	CLK PC10, MISO PC11, MOSI PC12
	OLED_SPI3.GPIO_init(GPIOC, GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF6_SPI3);
	OLED_DC.GPIO_init(GPIOC, GPIO_PIN_11, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, 0x00);
	OLED_RST.GPIO_init(GPIOD, GPIO_PIN_2, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, 0x00);
	OLED_CS.GPIO_init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, 0x00);
	// ADC
	ADCA.GPIO_init(GPIOA, ADC1_A0_PIN | ADC1_A1_PIN | ADC1_A2_PIN, GPIO_MODE_ANALOG, GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM, 0x00);
	ADCB.GPIO_init(GPIOB, GPIO_PIN_0, GPIO_MODE_ANALOG, GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM, 0x00);
	ADCC.GPIO_init(GPIOC, ADC1_A5_PIN | GPIO_PIN_1, GPIO_MODE_ANALOG, GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM, 0x00);

}


void GPIO::Clock_enable(){
	if 		(port == GPIOA)	__HAL_RCC_GPIOA_CLK_ENABLE();
	else if (port == GPIOB)	__HAL_RCC_GPIOB_CLK_ENABLE();
	else if (port == GPIOC)	__HAL_RCC_GPIOC_CLK_ENABLE();
	else if (port == GPIOD)	__HAL_RCC_GPIOD_CLK_ENABLE();
	else if (port == GPIOH)	__HAL_RCC_GPIOH_CLK_ENABLE();
}

void GPIO::GPIO_init(GPIO_TypeDef* gpiox, uint32_t pin, uint32_t mode, uint32_t pull, uint32_t speed, uint32_t alternate){
	port	=	gpiox;
    Pin		=	pin;

	// Enable clock for GPIO
	Clock_enable();

	// Do initialization
	init.Pin = Pin;
	init.Mode = mode;
	init.Pull = pull;
	init.Speed = speed;
	init.Alternate = alternate;
	HAL_GPIO_Init(port, &init);
}


void GPIO::EXTI_init(GPIO_TypeDef* port, uint32_t pin, uint32_t mode, uint32_t pull, uint32_t speed, uint32_t alternate){
	IRQn_Type IRQ_channel;

	GPIO_init(port, pin, mode,pull,speed, alternate);

	// Select interrupt mode
//	EXTI->IMR |= GPIO->init.Pin;

	// Select edge
	if (init.Mode == GPIO_MODE_IT_FALLING) {
		// Write to falling edge register
		EXTI->FTSR |= init.Pin;
	} else if (init.Mode == GPIO_MODE_IT_RISING) {
		// Write to rising edge register
		EXTI->RTSR |= init.Pin;
	} else {
		// Write to rising and falling edge registers
		EXTI->FTSR |= init.Pin;
		EXTI->RTSR |= init.Pin;
	}

	switch(init.Pin){
		case GPIO_PIN_0:
			IRQ_channel	=	EXTI0_IRQn;
			break;
		case GPIO_PIN_1:
			IRQ_channel	=	EXTI1_IRQn;
			break;
		case GPIO_PIN_2:
			IRQ_channel	=	EXTI2_IRQn;
			break;
		case GPIO_PIN_3:
			IRQ_channel	=	EXTI3_IRQn;
			break;
		case GPIO_PIN_4:
			IRQ_channel	=	EXTI4_IRQn;
			break;
		case GPIO_PIN_5:
		case GPIO_PIN_6:
		case GPIO_PIN_7:
		case GPIO_PIN_8:
		case GPIO_PIN_9:
			IRQ_channel	=	EXTI9_5_IRQn;
			break;
		case GPIO_PIN_10:
		case GPIO_PIN_11:
		case GPIO_PIN_12:
		case GPIO_PIN_13:
		case GPIO_PIN_14:
		case GPIO_PIN_15:
			IRQ_channel	=	EXTI15_10_IRQn;
			break;
	}

	HAL_NVIC_SetPriority(IRQ_channel, EXTI_NVIC_PRIORITY, 0);
	HAL_NVIC_EnableIRQ(IRQ_channel);
}

void GPIO::SetPinLow(void){
	HAL_GPIO_WritePin(port, Pin, GPIO_PIN_RESET);
}

void GPIO::SetPinHigh(void){
	HAL_GPIO_WritePin(port, Pin, GPIO_PIN_SET);
}

void GPIO::SetPinValue(uint16_t value){
	  if(value == 0)		SetPinLow();
	  else					SetPinHigh();
}

void GPIO::TogglePinValue(void){
	HAL_GPIO_TogglePin(port, Pin);
}


uint8_t GPIO::ReadPinValue(void){
//	return HAL_GPIO_ReadPin(GPIO_port, Pin);
	  if(HAL_GPIO_ReadPin(port, Pin)) return 1;
	  else return 0;
}


__weak void __EXTI_Interrupt(uint16_t GPIO_Pin) {
}


void EXTI0_IRQHandler(void){
	// Check status
	if 		(EXTI->PR & (EXTI_PR_PR0)) {
		EXTI->PR = EXTI_PR_PR10;		// Clear bit
		__EXTI_Interrupt(GPIO_PIN_0);	// Call global function
	}
}

void EXTI1_IRQHandler(void){
	// Check status
	if 		(EXTI->PR & (EXTI_PR_PR1)) {
		EXTI->PR = EXTI_PR_PR1;			// Clear bit
		__EXTI_Interrupt(GPIO_PIN_1);	// Call global function
	}
}

void EXTI2_IRQHandler(void){
	// Check status
	if 		(EXTI->PR & (EXTI_PR_PR2)) {
		EXTI->PR = EXTI_PR_PR2;			// Clear bit
		__EXTI_Interrupt(GPIO_PIN_2);	// Call global function
	}
}

void EXTI3_IRQHandler(void){
	// Check status
	if 		(EXTI->PR & (EXTI_PR_PR3)) {
		EXTI->PR = EXTI_PR_PR3;			// Clear bit
		__EXTI_Interrupt(GPIO_PIN_3);	// Call global function
	}
}

void EXTI4_IRQHandler(void){
	// Check status
	if 		(EXTI->PR & (EXTI_PR_PR4)) {
		EXTI->PR = EXTI_PR_PR4;			// Clear bit
		__EXTI_Interrupt(GPIO_PIN_4);	// Call global function
	}
}

void EXTI9_5_IRQHandler(void){
	// Check status
	if 		(EXTI->PR & (EXTI_PR_PR5)) {
		EXTI->PR = EXTI_PR_PR5;			// Clear bit
		__EXTI_Interrupt(GPIO_PIN_5);	// Call global function
	}
	else if	(EXTI->PR & (EXTI_PR_PR6)) {
		EXTI->PR = EXTI_PR_PR6;			// Clear bit
		__EXTI_Interrupt(GPIO_PIN_6);	// Call global function
	}
	else if	(EXTI->PR & (EXTI_PR_PR7)) {
		EXTI->PR = EXTI_PR_PR7;			// Clear bit
		__EXTI_Interrupt(GPIO_PIN_7);	// Call global function
	}
	else if	(EXTI->PR & (EXTI_PR_PR8)) {
		EXTI->PR = EXTI_PR_PR8;			// Clear bit
		__EXTI_Interrupt(GPIO_PIN_8);	// Call global function
	}
	else if	(EXTI->PR & (EXTI_PR_PR9)) {
		EXTI->PR = EXTI_PR_PR9;			// Clear bit
		__EXTI_Interrupt(GPIO_PIN_9);	// Call global function
	}

}

void EXTI15_10_IRQHandler(void){
	// Check status
	if 		(EXTI->PR & (EXTI_PR_PR10)) {
		EXTI->PR = EXTI_PR_PR10;		// Clear bit
		__EXTI_Interrupt(GPIO_PIN_10);	// Call global function
	}
	else if	(EXTI->PR & (EXTI_PR_PR11)) {
		EXTI->PR = EXTI_PR_PR11;		// Clear bit
		__EXTI_Interrupt(GPIO_PIN_11);	// Call global function
	}
	else if	(EXTI->PR & (EXTI_PR_PR12)) {
		EXTI->PR = EXTI_PR_PR12;		// Clear bit
		__EXTI_Interrupt(GPIO_PIN_12);	// Call global function
	}
	else if	(EXTI->PR & (EXTI_PR_PR13)) {
		EXTI->PR = EXTI_PR_PR13;		// Clear bit
		__EXTI_Interrupt(GPIO_PIN_13);	// Call global function
	}
	else if	(EXTI->PR & (EXTI_PR_PR14)) {
		EXTI->PR = EXTI_PR_PR14;		// Clear bit
		__EXTI_Interrupt(GPIO_PIN_14);	// Call global function
	}
	else if	(EXTI->PR & (EXTI_PR_PR15)) {
		EXTI->PR = EXTI_PR_PR15;		// Clear bit
		__EXTI_Interrupt(GPIO_PIN_15);	// Call global function
	}
}

