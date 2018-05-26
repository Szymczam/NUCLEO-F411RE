/*
 * IWDG.cpp
 *
 *  Created on: 24 kwi 2018
 *      Author: Marek
 */

#include "MSZ/IWDG.h"


void mIWDG::IWDG_Init(IWDG_Timeout_t timeout) {
	IWDG_HandleTypeDef IWDGx;
	uint16_t reload = 0;
	uint16_t prescaler = 0;

	/* Set proper clock depending on timeout user select */
	if (timeout >= IWDG_Timeout_8s) {
		/* IWDG counter clock: LSI/256 = 128Hz */
		prescaler = 0x07;
	} else {
		/* IWDG counter clock: LSI/32 = 1024Hz */
		prescaler = 0x03;
	}

	// Set counter reload value
	if (timeout == IWDG_Timeout_5ms) {
		reload = 5; /* 1024 Hz IWDG ticking */
	} else if (timeout == IWDG_Timeout_10ms) {
		reload = 10; /* 1024 Hz IWDG ticking */
	} else if (timeout == IWDG_Timeout_15ms) {
		reload = 15; /* 1024 Hz IWDG ticking */
	} else if (timeout == IWDG_Timeout_30ms) {
		reload = 31; /* 1024 Hz IWDG ticking */
	} else if (timeout == IWDG_Timeout_60ms) {
		reload = 61; /* 1024 Hz IWDG ticking */
	} else if (timeout == IWDG_Timeout_120ms) {
		reload = 123; /* 1024 Hz IWDG ticking */
	} else if (timeout == IWDG_Timeout_250ms) {
		reload = 255; /* 1024 Hz IWDG ticking */
	} else if (timeout == IWDG_Timeout_500ms) {
		reload = 511; /* 1024 Hz IWDG ticking */
	} else if (timeout == IWDG_Timeout_1s) {
		reload = 1023; /* 1024 Hz IWDG ticking */
	} else if (timeout == IWDG_Timeout_2s) {
		reload = 2047; /* 1024 Hz IWDG ticking */
	} else if (timeout == IWDG_Timeout_4s) {
		reload = 4095; /* 1024 Hz IWDG ticking */
	} else if (timeout == IWDG_Timeout_8s) {
		reload = 1023; /* 128 Hz IWDG ticking */
	} else if (timeout == IWDG_Timeout_16s) {
		reload = 2047; /* 128 Hz IWDG ticking */
	} else if (timeout == IWDG_Timeout_32s) {
		reload = 4095; /* 128 Hz IWDG ticking */
	}

	IWDGx.Instance 			= IWDG;
	IWDGx.Init.Prescaler 	= prescaler;
	IWDGx.Init.Reload    	= reload;
	HAL_IWDG_Init(&IWDGx);
}

void mIWDG::Reset(void){
//	HAL_IWDG_Refresh(&IWDG);
	IWDG->KR = 0xAAAA;
}


