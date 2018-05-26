/*
 * TIM.h
 *
 *  Created on: 3 kwi 2018
 *      Author: Marek
 */

#ifndef MSZ_TIM_H_
#define MSZ_TIM_H_

/*
@verbatim
TIMER   |CHANNEL 1            |CHANNEL 2            |CHANNEL 3            |CHANNEL 4
        |PP1    PP2    PP3    |PP1    PP2    PP3    |PP1    PP2    PP3    |PP1    PP2    PP3

TIM 1   |PA8    PE9    -      |PA9    PE10   -      |PA10   PE13   -      |PA11   PE14   -
TIM 2   |PA0    PA5    PA15   |PA1    PB3    -      |PA2    PB10   -      |PA3    PB11   -
TIM 3   |PA6    PB4    PC6    |PA7    PB5    PC7    |PB0    PC8    -      |PB1    PC9    -
TIM 4   |PB6    PD12   -      |PB7    PD13   -      |PB8    PD14   -      |PB9    PD15    -
TIM 5   |PA0    PH10   -      |PA1    PH11   -      |PA2    PH12   -      |PA3    PI0    -
TIM 8   |PC6    PI5    -      |PC7    PI6    -      |PC8    PI7    -      |PC9    PI2    -
TIM 9   |PA2    PE5    -      |PA3    PE6    -      |-      -      -      |-      -      -
TIM 10  |PB8    PF6    -      |-      -      -      |-      -      -      |-      -      -
TIM 11  |PB9    PF7    -      |-      -      -      |-      -      -      |-      -      -
TIM 12  |PB14   PH6    -      |PB15   PH9    -      |-      -      -      |-      -      -
TIM 13  |PA6    PF8    -      |-      -      -      |-      -      -      |-      -      -
TIM 14  |PA7    PF9    -      |-      -      -      |-      -      -      |-      -      -
@endverbatim
 */


#include "stm32f4xx.h"


class TIM {
public:
	TIM();
    ~TIM();
    TIM_HandleTypeDef 	TIMx;
	uint32_t Period;
	uint32_t Pulse;
	uint32_t Prescaler;
	uint32_t Channel;

    void TIM_init(TIM_TypeDef* port);
    void TIM_ADC_init(TIM_TypeDef* port);
    void PWM_init(TIM_TypeDef* port);

private:
	void Clock_enable();
};

void TIM4_IRQHandler(void);


/*
 * @brief  Result enumeration
 */
typedef enum {
	TIM_Result_Ok, 		// Everything ok
	TIM_Result_Error  	// An error has occured
} TIM_Result_t;


extern TIM PWM_LED;
extern TIM TIMER1;



#endif /* MSZ_TIM_H_ */
