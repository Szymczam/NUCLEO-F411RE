/*
 * GPIO.h
 *
 *  Created on: 7 kwi 2018
 *      Author: Marek
 */

#ifndef MSZ_GPIO_H_
#define MSZ_GPIO_H_

#include "stm32f4xx.h"


/*
 * @brief  Default EXTI preemption priority for EXTI lines used in NVIC
 */
#define EXTI_NVIC_PRIORITY     0x03

/*
	Pin out NUCLEO F411RE

	A0	-	ADC A0			B0	-	ADC A3			C0	-	ADC A5			H0	-	RCC_OSC_IN
	A1	-	ADC A1			B1	-	ePAPER CS		C1	-	ADC A4			H1	-	RCC_OSC_OUT
	A2	-	UART2 TX		B2	-	ePAPER RST		C2	-	ePAPER DIN
	A3	-	UART2 RX		B3	-	ePAPER DC		C3	-
	A4	-	ADC A2			B4	-	ePAPER BUSY		C4	-
	A5	-	LED	/ PWM		B4	-					C5	-
	A6	-					B6	-					C6	-
	A7	-					B7	-					C7	-
	A8	-					B8	-					C8	-
	A9	-					B9	-					C9	-
	A10	-					B10	-	ePAPER CLK		C10	-
	A11	-					B11	-					C11	-
	A12	-					B12	-					C12	-
	A13	-					B13	-					C13	-	BUTTON
	A14	-					B14	-					C14	-	OSC32_IN
	A15	-					B15	-					C15	-	OSC32_OUT

*/

#define ADC1_A0_PIN 			GPIO_PIN_0
#define ADC1_A0_PORT 			GPIOA
#define ADC1_A1_PIN 			GPIO_PIN_1
#define ADC1_A1_PORT 			GPIOA
#define ADC1_A2_PIN 			GPIO_PIN_4
#define ADC1_A2_PORT 			GPIOA
#define ADC1_A3_PIN 			GPIO_PIN_0
#define ADC1_A3_PORT 			GPIOB
#define ADC1_A4_PIN 			GPIO_PIN_1
#define ADC1_A4_PORT 			GPIOC
#define ADC1_A5_PIN 			GPIO_PIN_0
#define ADC1_A5_PORT 			GPIOC


class GPIO {
public:
	GPIO();
    ~GPIO();
    GPIO_InitTypeDef 	init;
    GPIO_TypeDef*		port;
    uint32_t Pin;


    static void Init_all(void);
    void GPIO_init(GPIO_TypeDef* port, uint32_t pin, uint32_t mode, uint32_t pull, uint32_t speed, uint32_t alternate);
    void EXTI_init(GPIO_TypeDef* port, uint32_t pin, uint32_t mode, uint32_t pull, uint32_t speed, uint32_t alternate);
    void SetPinHigh(void);
    void SetPinLow(void);
    void SetPinValue(uint16_t value);
    void TogglePinValue(void);
    uint8_t ReadPinValue(void);
    void __EXTI_Interrupt(uint16_t GPIO_Pin);


private:
	void Clock_enable();
};

extern GPIO LED;
extern GPIO LED_PWM;
extern GPIO BUTTON;
extern GPIO OLED_DC;
extern GPIO OLED_RST;
extern GPIO OLED_CS;
extern GPIO OLED_SCL;
extern GPIO OLED_SDA;
extern GPIO ePAPIER_DC;
extern GPIO ePAPIER_RST;
extern GPIO ePAPIER_CS;
extern GPIO ePAPIER_BUSY;
extern GPIO ePAPIER_CLK;
extern GPIO ePAPIER_DIN;
extern GPIO PC_UART2;
extern GPIO OLED_SPI3;
extern GPIO OLED_I2C1;
extern GPIO OLED_DC;
extern GPIO OLED_RST;
extern GPIO OLED_CS;
extern GPIO ADCA;
extern GPIO ADCB;
extern GPIO ADCC;


#endif /* MSZ_GPIO_H_ */
