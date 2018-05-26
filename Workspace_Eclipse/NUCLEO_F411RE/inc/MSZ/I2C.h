/*
 * I2C.h
 *
 *  Created on: 11 kwi 2018
 *      Author: Marek
 */

#ifndef MSZ_I2C_H_
#define MSZ_I2C_H_

#include "stm32f4xx.h"

#define I2C_MEMADD_SIZE_8BIT            0x00000001U
#define I2C_MEMADD_SIZE_16BIT           0x00000010U

class mI2C {
public:
	mI2C();
    ~mI2C();

    I2C_HandleTypeDef		I2Cx;

    void I2C_init(I2C_TypeDef* port, uint32_t clockSpeed);
    void I2C_send(uint8_t device_address, uint8_t register_address, uint8_t data);
    void I2C_sendMulti(uint8_t device_address, uint8_t register_address, uint8_t* data);

private:
	void Clock_enable();
};

extern mI2C PC_I2C;

#endif /* MSZ_I2C_H_ */
