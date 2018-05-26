/*
 * WWDG.h
 *
 *  Created on: 26 kwi 2018
 *      Author: Marek
 */

#ifndef MSZ_WWDG_H_
#define MSZ_WWDG_H_

#include "stm32f4xx.h"

class mWWDG {
public:
	mWWDG();
    ~mWWDG();
	WWDG_HandleTypeDef WWDGx;

    void WWDG_Init(uint32_t timeout);
    void Refresh(void);
private:
};

extern mWWDG My_WWDG;


#endif /* MSZ_WWDG_H_ */
