/*
 * IWDG.h
 *
 *  Created on: 24 kwi 2018
 *      Author: Marek
 */

#ifndef MSZ_IWDG_H_
#define MSZ_IWDG_H_

#include "stm32f4xx.h"

typedef enum {
	IWDG_Timeout_5ms = 0x00,   /*!< System reset called every 5ms */
	IWDG_Timeout_10ms = 0x01,  /*!< System reset called every 10ms */
	IWDG_Timeout_15ms = 0x02,  /*!< System reset called every 15ms */
	IWDG_Timeout_30ms = 0x03,  /*!< System reset called every 30ms */
	IWDG_Timeout_60ms = 0x04,  /*!< System reset called every 60ms */
	IWDG_Timeout_120ms = 0x05, /*!< System reset called every 120ms */
	IWDG_Timeout_250ms = 0x06, /*!< System reset called every 250ms */
	IWDG_Timeout_500ms = 0x07, /*!< System reset called every 500ms */
	IWDG_Timeout_1s = 0x08,    /*!< System reset called every 1s */
	IWDG_Timeout_2s = 0x09,    /*!< System reset called every 2s */
	IWDG_Timeout_4s = 0x0A,    /*!< System reset called every 4s */
	IWDG_Timeout_8s = 0x0B,    /*!< System reset called every 8s */
	IWDG_Timeout_16s = 0x0C,   /*!< System reset called every 16s */
	IWDG_Timeout_32s = 0x0D    /*!< System reset called every 32s. This is maximum value allowed with IWDG timer */
} IWDG_Timeout_t;


class mIWDG {
public:
	mIWDG();
    ~mIWDG();

    static void IWDG_Init(IWDG_Timeout_t timeout);
    static void Reset(void);
private:
};


#endif /* MSZ_IWDG_H_ */
