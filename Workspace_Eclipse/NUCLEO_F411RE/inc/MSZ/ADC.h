/*
 * ADC.h
 *
 *  Created on: 30 mar 2018
 *      Author: Marek
 */

#ifndef MSZ_ADC_H_
#define MSZ_ADC_H_

/*
CHANNEL   ADC1

0         PA0
1         PA1
2         PA2
3         PA3
4         PA4
5         PA5
6         PA6
7         PA7
8         PB0
9         PB1
10        PC0
11        PC1
12        PC2
13        PC3
14        PC4
15        PC5
*/

#include "stm32f4xx.h"

typedef struct {
	uint32_t	ADC_A0;
	uint32_t	ADC_A1;
	uint32_t	ADC_A2;
	uint32_t	ADC_A3;
	uint32_t	ADC_A4;
	uint32_t	ADC_A5;
} MS_ADC_Value_t;


class mADC {
public:
	mADC();
    ~mADC();

    ADC_HandleTypeDef	ADCx;

    void ADC_Init();
    void ADC_Init_IT(uint8_t channel);
    uint16_t ADC_Read(uint32_t channel);


private:
	inline void Clock_enable();
};

void ADC_IRQHandler(void);

extern mADC PC_ADC;



#endif /* MSZ_ADC_H_ */
