/*
 * I2C.cpp
 *
 *  Created on: 11 kwi 2018
 *      Author: Marek
 */


#include <I2C.h>

mI2C PC_I2C;

mI2C::~mI2C() {
};

mI2C::mI2C() {
};


void mI2C::Clock_enable(){
	if 		(I2Cx.Instance == I2C1)	__HAL_RCC_I2C1_CLK_ENABLE();
	else if (I2Cx.Instance == I2C2)	__HAL_RCC_I2C2_CLK_ENABLE();
	else if (I2Cx.Instance == I2C3)	__HAL_RCC_I2C3_CLK_ENABLE();
}

void mI2C::I2C_init(I2C_TypeDef* port, uint32_t clockSpeed){
	I2Cx.Instance = port;

	// Enable clock for GPIO
	Clock_enable();

	// Do initialization
	I2Cx.Init.ClockSpeed = clockSpeed;
	I2Cx.Init.OwnAddress2 = 0x00;
	I2Cx.Init.OwnAddress1 = 0x00;
	I2Cx.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	I2Cx.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	I2Cx.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	I2Cx.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	I2Cx.Init.DutyCycle = I2C_DUTYCYCLE_2;
	HAL_I2C_Init(&I2Cx);
}

void mI2C::I2C_send(uint8_t device_address, uint8_t register_address, uint8_t data) {
	uint8_t d[2];

	/* Format array to send */
	d[0] = register_address;
	d[1] = data;
	HAL_I2C_Master_Transmit(&I2Cx, (uint16_t)device_address, (uint8_t *)d, 3, 1000);

}

void mI2C::I2C_sendMulti(uint8_t device_address, uint8_t register_address, uint8_t* data) {
	HAL_I2C_Master_Transmit(&I2Cx, (uint16_t)device_address, (uint8_t *)data, 2, 1000);

}

