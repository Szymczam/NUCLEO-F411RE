/*
 * WWDG.cpp
 *
 *  Created on: 26 kwi 2018
 *      Author: Marek
 */

#include "MSZ/WWDG.h"

mWWDG::~mWWDG() {
};

mWWDG::mWWDG() {
};


mWWDG My_WWDG;


/* 	Refresh WWDG: update counter value to 127, the refresh window is:
	~655 * (127-80) = 30.7ms < refresh window < ~655 * 64 = 41.9ms
*/
//	    My_WWDG.Refresh();


/*	Configure the WWDG peripheral
 	WWDG clock counter = (PCLK1 (50MHz)/4096)/1) = 1525 Hz (~655 us)
 	WWDG Window value = 80 means that the WWDG counter should be refreshed only
 	when the counter is below 80 (and greater than 64) otherwise a reset will
 	be generated.
 	WWDG Counter value = 127, WWDG timeout = ~655 us * 64 = 41.9 ms
*/
void mWWDG::WWDG_Init(uint32_t timeout) {
	// WWDG Peripheral clock enable
	__HAL_RCC_WWDG_CLK_ENABLE();

	WWDGx.Instance = WWDG;
	WWDGx.Init.Prescaler = WWDG_PRESCALER_8;
	WWDGx.Init.Window    = 80;
	WWDGx.Init.Counter   = 127;
	WWDGx.Init.EWIMode	 = WWDG_EWI_ENABLE;
	HAL_WWDG_Init(&WWDGx);

	HAL_NVIC_SetPriority(WWDG_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(WWDG_IRQn);
}

void mWWDG::Refresh(void){
	HAL_WWDG_Refresh(&WWDGx);
}


