/*
 * SYS.h
 *
 *  Created on: 12 kwi 2018
 *      Author: Marek
 */

#ifndef MSZ_SYS_H_
#define MSZ_SYS_H_


#include "stm32fxxx_hal.h"
#include "RCC.h"

/*
 * @brief  CPU LOAD structure
 */
typedef struct {
	float Load;      /*!< CPU load percentage */
	uint8_t Updated; /*!< Set to 1 when new CPU load is calculated */
	uint32_t WCNT;   /*!< Number of working cycles in one period. Meant for private use */
	uint32_t SCNT;   /*!< Number of sleeping cycles in one period. Meant for private use */
} CPU_LOAD_t;



class mSYS {
public:
	mSYS();
    ~mSYS();
    CPU_LOAD_t	CPU;

    uint8_t CPU_LOAD_Init(void);
    uint8_t CPU_LOAD_GoToSleepMode();


private:
    uint8_t DWTCounterEnable(void);
};

extern mSYS SYS;

#endif /* MSZ_SYS_H_ */
