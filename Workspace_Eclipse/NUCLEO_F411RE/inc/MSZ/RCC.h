/*
 * RCC.h
 *
 *  Created on: 12 kwi 2018
 *      Author: Marek
 */

#ifndef MSZ_RCC_H_
#define MSZ_RCC_H_

#include "stm32f4xx.h"

#define RCC_OSCILLATORTYPE    RCC_OSCILLATORTYPE_HSE /*!< Used to select system oscillator type */
#define RCC_PLL_M             8                      /*!< Used for PLL M parameter */
#define RCC_PLL_N             400                    /*!< Used for PLL N parameter */
#define RCC_PLL_P             4                      /*!< Used for PLL P parameter */
#define RCC_PLL_Q             7                      /*!< Used for PLL Q parameter */

typedef enum {
	RCC_Result_Ok = 0x00,
	RCC_Result_Error
} RCC_Result_t;

typedef enum {
	Clock_HSI,    /*!< High speed internal clock */
	Clock_HSE,    /*!< High speed external clock */
	Clock_SYSCLK, /*!< System core clock */
	Clock_PCLK1,  /*!< PCLK1 (APB1) peripheral clock */
	Clock_PCLK2,  /*!< PCLK2 (APB2) peripheral clock */
	Clock_HCLK    /*!< HCLK (AHB1) high speed clock */
} Clock_t;

class MRCC {
public:
	MRCC();
    ~MRCC();


    static	RCC_Result_t SystemClock_Config(void);
    static	uint32_t Delay_init(void);
    static	uint32_t GetClockSpeedinMHz(Clock_t clock);

    void Delay_us(__IO uint32_t micros);
    void Delay_ms(uint32_t millis);

private:

};




#endif /* MSZ_RCC_H_ */
