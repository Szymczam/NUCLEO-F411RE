/**
  ******************************************************************************
  * @file    LIB_Config.h
  * @author  Waveshare Team
  * @version 
  * @date    13-October-2014
  * @brief     This file provides configurations for low layer hardware libraries.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WAVESHARE SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _USE_LIB_CONFIG_H_
#define _USE_LIB_CONFIG_H_
//Macro Definition

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <OLED/Fonts.h>
#include "MSZ/SPI.h"
#include <MSZ/GPIO.h>
#include "MSZ/I2C.h"
#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"

#define SSD1306_WIDTH    128
#define SSD1306_HEIGHT   64

#define SSD1306_CMD    0
#define SSD1306_DAT    1

#define SH1106
//#define SSD1306

//#define INTERFACE_OLED_3WIRE_SPI   //3-wire SPI
#define INTERFACE_OLED_4WIRE_SPI     //4-wire SPI
//#define INTERFACE_OLED_IIC         //I2C


#define __SSD1306_CS_SET()      OLED_CS.SetPinHigh()
#define __SSD1306_CS_CLR()      OLED_CS.SetPinLow()

#define __SSD1306_RES_SET()     OLED_RST.SetPinHigh()
#define __SSD1306_RES_CLR()     OLED_RST.SetPinLow()

#define __SSD1306_DC_SET()      OLED_DC.SetPinHigh()
#define __SSD1306_DC_CLR()      OLED_DC.SetPinLow()


#define __SSD1306_WRITE_BYTE(DATA) OLED_SPI.SPI_Send(chData);



/*------------------------------------------------------------------------------------------------------*/
//I2C

#ifdef INTERFACE_OLED_IIC

	#define __SSD1306_CLK_SET()     OLED_CS.SetPinHigh()
	#define __SSD1306_CLK_CLR()     OLED_CS.SetPinLow()

	#define __SSD1306_DIN_SET()     OLED_CS.SetPinHigh()
	#define __SSD1306_DIN_CLR()     OLED_CS.SetPinLow()

	#define __IIC_SCL_SET()     OLED_SCL.SetPinHigh()
	#define __IIC_SCL_CLR()     OLED_SCL.SetPinLow()

	#define __IIC_SDA_SET()		OLED_SDA.SetPinHigh()
	#define __IIC_SDA_CLR()     OLED_SDA.SetPinLow()

	#define __IIC_SDA_IN()     	do { \
								GPIO_InitTypeDef tGPIO; \
								tGPIO.Pin = OLED_SDA.Pin; \
								tGPIO.Speed = GPIO_SPEED_FREQ_VERY_HIGH; \
								tGPIO.Mode = GPIO_MODE_INPUT; \
								HAL_GPIO_Init(OLED_SDA.GPIO_port, &tGPIO); \
							}while(0)				

	#define __IIC_SDA_OUT() 	do { \
								GPIO_InitTypeDef tGPIO; \
								tGPIO.Pin = OLED_SDA.Pin; \
								tGPIO.Speed = GPIO_SPEED_FREQ_VERY_HIGH; \
								tGPIO.Mode = GPIO_MODE_OUTPUT_PP; \
								HAL_GPIO_Init(OLED_SDA.GPIO_port, &tGPIO); \
							}while(0)   


	#define __IIC_SDA_READ()    OLED_SDA.ReadPinValue()


#elif defined(INTERFACE_OLED_4WIRE_SPI)



#endif
/*------------------------------------------------------------------------------------------------------*/

class mOLED {
public:
	mOLED();
    ~mOLED();

    I2C_HandleTypeDef		I2Cx;
    uint8_t s_chDispalyBuffer[128][8];

	void OLED_init(void);
	void ssd1306_clear_screen(uint8_t chFill);
	void ssd1306_refresh_gram(void);
	void ssd1306_fill_screen(uint8_t chXpos1, uint8_t chYpos1, uint8_t chXpos2, uint8_t chYpos2, uint8_t chDot);
	void ssd1306_display_char(uint8_t chXpos, uint8_t chYpos, uint8_t chChr, uint8_t chSize, uint8_t chMode);
	void ssd1306_display_num(uint8_t chXpos, uint8_t chYpos, uint32_t chNum, uint8_t chLen,uint8_t chSize);
	void ssd1306_display_string(uint8_t chXpos, uint8_t chYpos, const char *pchString, uint8_t chSize, uint8_t chMode);
	void ssd1306_draw_1616char(uint8_t chXpos, uint8_t chYpos, uint8_t chChar);
	void ssd1306_draw_3216char(uint8_t chXpos, uint8_t chYpos, uint8_t chChar);
	void ssd1306_draw_bitmap(uint8_t chXpos, uint8_t chYpos, const uint8_t *pchBmp, uint8_t chWidth, uint8_t chHeight);
	void ssd1306_init(void);
	void ssd1306_display_on(void);
	void ssd1306_display_off(void);
#ifdef INTERFACE_OLED_IIC
    void iic_init(void);
    void iic_start(void);
    void iic_stop(void);
    bool iic_wait_for_ack(void);
    void iic_ack(void);
    void iic_nack(void);
    void iic_write_byte(uint8_t chData);
    uint8_t iic_read_byte(uint8_t chAck);
    void iic_write_one_byte(uint8_t chDevAddr, uint8_t chRegAddr, uint8_t chData);
    bool iic_write_buffer(uint8_t chDevAddr, uint8_t chRegAddr, uint8_t chNum, uint8_t *pchBuffer);
    uint8_t iic_read_one_byte(uint8_t chDevAddr, uint8_t chRegAddr);
    bool iic_read_buffer(uint8_t chDevAddr, uint8_t chRegAddr, uint8_t chNum, uint8_t *pchBuffer);
    uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
#endif


private:
    void ssd1306_draw_point(uint8_t chXpos, uint8_t chYpos, uint8_t chPoint);
    void ssd1306_write_byte(uint8_t chData, uint8_t chCmd);
};

extern mOLED OLED;




/* Exported functions ------------------------------------------------------- */
#define BIT(__N)         ((uint8_t)(1 << __N))

#define enable_irq()   __enable_irq();
#define disable_irq()  __disable_irq();


//! \brief The safe ATOM code section macro
#define SAFE_ATOM_CODE(...) {\
        disable_irq();\
        __VA_ARGS__;\
        enable_irq();\
    }

typedef struct {
    unsigned Bit0:1;
    unsigned Bit1:1;
    unsigned Bit2:1;
    unsigned Bit3:1;
    unsigned Bit4:1;
    unsigned Bit5:1;
    unsigned Bit6:1;
    unsigned Bit7:1;
}byte_bit_t;

typedef union{
	byte_bit_t tBit;
	uint8_t chByte;
}bit_operate_t;




#endif

/*-------------------------------END OF FILE-------------------------------*/

