/*
 * TIM.c
 *
 *  Created on: 3 kwi 2018
 *      Author: Marek
 */

#include "TIM.h"


TIM PWM_LED;
TIM TIMER1;


TIM::~TIM() {
};

TIM::TIM() {
	 Period		=	0;
	 Pulse		=	0;
	 Prescaler	=	0;
	 Channel	=	0;
};

void TIM::Clock_enable(){
	if 		(TIMx.Instance == TIM1)		__HAL_RCC_TIM1_CLK_ENABLE();
	else if (TIMx.Instance == TIM2)		__HAL_RCC_TIM2_CLK_ENABLE();
	else if (TIMx.Instance == TIM3)		__HAL_RCC_TIM3_CLK_ENABLE();
	else if (TIMx.Instance == TIM4)		__HAL_RCC_TIM4_CLK_ENABLE();
	else if (TIMx.Instance == TIM4)		__HAL_RCC_TIM5_CLK_ENABLE();
	else if (TIMx.Instance == TIM5)		__HAL_RCC_TIM5_CLK_ENABLE();
	else if (TIMx.Instance == TIM9)		__HAL_RCC_TIM9_CLK_ENABLE();
	else if (TIMx.Instance == TIM10)	__HAL_RCC_TIM10_CLK_ENABLE();
	else if (TIMx.Instance == TIM11)	__HAL_RCC_TIM11_CLK_ENABLE();
}

void TIM::TIM_init(TIM_TypeDef* port){
	TIMx.Instance 				= port;
	IRQn_Type IRQ_channel;

	// Enable clock for TIM
	Clock_enable();

	TIMx.Init.Period 				= Period;
	TIMx.Init.Prescaler 			= Prescaler;
	TIMx.Init.ClockDivision 		= 0;
	TIMx.Init.CounterMode 			= TIM_COUNTERMODE_UP;
	TIMx.Init.RepetitionCounter 	= 0;
	HAL_TIM_Base_Init(&TIMx);


	if 		(TIMx.Instance == TIM2)		IRQ_channel	=	TIM2_IRQn;
	else if (TIMx.Instance == TIM3)		IRQ_channel	=	TIM3_IRQn;
	else if (TIMx.Instance == TIM4)		IRQ_channel	=	TIM4_IRQn;
	else if (TIMx.Instance == TIM5)		IRQ_channel	=	TIM5_IRQn;

	HAL_NVIC_SetPriority(IRQ_channel, 0, 0);
	HAL_NVIC_EnableIRQ(IRQ_channel);

	HAL_TIM_Base_Start_IT(&TIMx);
}

void TIM::TIM_ADC_init(TIM_TypeDef* port){
	TIM_MasterConfigTypeDef sMasterConfig;
	TIMx.Instance 	= port;

	// Enable clock for TIM
	Clock_enable();

	TIMx.Init.Period 				= Period;
	TIMx.Init.Prescaler 			= Prescaler;
	TIMx.Init.ClockDivision 		= 0;
	TIMx.Init.CounterMode 			= TIM_COUNTERMODE_UP;
	TIMx.Init.RepetitionCounter 	= 0;
	HAL_TIM_Base_Init(&TIMx);


	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&TIMx, &sMasterConfig);

	HAL_TIM_Base_Start_IT(&TIMx);

}

void TIM::PWM_init(TIM_TypeDef* port){
	TIMx.Instance 				= port;
	TIM_OC_InitTypeDef oc;

	// Enable clock for TIM
	Clock_enable();

	TIMx.Init.Period 			= Period;
	TIMx.Init.Prescaler 		= Prescaler;
	TIMx.Init.ClockDivision 	= 0;
	TIMx.Init.CounterMode 		= TIM_COUNTERMODE_UP;
	TIMx.Init.RepetitionCounter	= 0;
	HAL_TIM_PWM_Init(&TIMx);

	oc.OCMode 			= TIM_OCMODE_PWM1;
	oc.Pulse 			= Pulse;
	oc.OCPolarity 		= TIM_OCPOLARITY_HIGH;
	oc.OCNPolarity 		= TIM_OCNPOLARITY_LOW;
	oc.OCFastMode 		= TIM_OCFAST_ENABLE;
	oc.OCIdleState 		= TIM_OCIDLESTATE_SET;
	oc.OCNIdleState 	= TIM_OCNIDLESTATE_RESET;
	HAL_TIM_PWM_ConfigChannel(&TIMx, &oc, Channel);
	HAL_TIM_PWM_Start(&TIMx, Channel);
}


__weak void __TIM_Interrupt(void) {
}





uint8_t sign;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (PWM_LED.Pulse == PWM_LED.Period/2) 	sign = 0;
	else if (PWM_LED.Pulse == 1) 			sign = 1;

	if (sign) 	PWM_LED.Pulse++;
	else 		PWM_LED.Pulse--;
	__HAL_TIM_SET_COMPARE(&PWM_LED.TIMx, PWM_LED.Channel, PWM_LED.Pulse);

}

/*
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim){

}
*/
