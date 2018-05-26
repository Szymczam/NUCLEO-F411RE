/*
 * OLED.c
 *
 *  Created on: 31 mar 2018
 *      Author: Marek
 */

#include <OLED/OLED.h>


mOLED::~mOLED() {
};

mOLED::mOLED() {
};


mOLED OLED;


void mOLED::OLED_init(void){


#ifdef INTERFACE_4WIRE_SPI

#endif

	ssd1306_init();
	printf("OLED : Ok\r\n");
}


enum {
	IIC_DIR_TX = 0,
	IIC_DIR_RX,
};

enum {
	IIC_SDA_IN = 0,
	IIC_SDA_OUT
};

enum {
	IIC_ACK = 0,
	IIC_NACK
};

#ifdef INTERFACE_OLED_IIC

void mOLED::iic_init(void)
{
	__IIC_SCL_SET();
	__IIC_SDA_SET();
//	Delay_ms(1);
}

void mOLED::iic_start(void)
{
	__IIC_SDA_OUT();
	__IIC_SDA_SET();
	__IIC_SCL_SET();//
//	Delay_us(1);
	__IIC_SDA_CLR();
//	Delay_us(1);
	__IIC_SCL_CLR();
//	Delay_us(1);
}

void mOLED::iic_stop(void)
{
	__IIC_SCL_CLR();
	__IIC_SDA_CLR();
//	Delay_us(1);
	__IIC_SCL_SET();
//	Delay_us(1);
	__IIC_SDA_SET();
//	Delay_us(1);
}

void mOLED::iic_ack(void)
{
	__IIC_SCL_CLR();
	__IIC_SDA_OUT();
	__IIC_SDA_CLR();
//	Delay_us(1);
	__IIC_SCL_SET();
//	Delay_us(1);
	__IIC_SCL_CLR();
}

void mOLED::iic_nack(void)
{
	__IIC_SCL_CLR();
	__IIC_SDA_OUT();
	__IIC_SDA_SET();
//	Delay_us(1);
	__IIC_SCL_SET();
//	Delay_us(1);
	__IIC_SCL_CLR();
}

bool mOLED::iic_wait_for_ack(void)
{
	uint8_t chTimeOut = 0;

	__IIC_SCL_CLR();
	//Delay_us(1);
	__IIC_SCL_SET();

	//__IIC_SDA_IN();
	while ((!(__IIC_SDA_READ())) && (chTimeOut ++)) {
		chTimeOut ++;
		if (chTimeOut > 200) {
			chTimeOut = 0;
			iic_stop();
			return false;
		}
	}
	__IIC_SCL_CLR();
	//Delay_us(1);

	return true;
}

void mOLED::iic_write_byte(uint8_t chData)
{
	uint8_t i;

	__IIC_SDA_OUT();
	for(i = 0; i < 8; i ++) {
		__IIC_SCL_CLR();
		//Delay_us(1);
		if(chData & 0x80) {
			__IIC_SDA_SET();
		} else {
			__IIC_SDA_CLR();
		}
		chData <<= 1;
		//Delay_us(1);
		__IIC_SCL_SET();
//		Delay_us(1);
	}
}

void mOLED::iic_write_one_byte(uint8_t chDevAddr, uint8_t chRegAddr, uint8_t chData)
{
	iic_start();
	iic_write_byte(chDevAddr | IIC_DIR_TX);
	iic_wait_for_ack();
	iic_write_byte(chRegAddr);
	iic_wait_for_ack();
	iic_write_byte(chData);
	iic_wait_for_ack();
	iic_stop();
}

bool mOLED::iic_write_buffer(uint8_t chDevAddr, uint8_t chRegAddr, uint8_t chNum, uint8_t *pchBuffer)
{
	uint8_t i;

	if (0 == chNum || NULL == pchBuffer) {
		return false;
	}

	iic_start();
	iic_write_byte(chDevAddr | IIC_DIR_TX);
	iic_wait_for_ack();
	iic_write_byte(chRegAddr);
	iic_wait_for_ack();

	for (i = 0; i < chNum; i ++) {
		iic_write_byte(*(pchBuffer + i));
		iic_wait_for_ack();
	}
	iic_stop();

	return true;
}

uint8_t mOLED::iic_read_byte(uint8_t chAck)
{
	uint8_t i, chRecData = 0;

	__IIC_SDA_IN();
	for (i = 0; i < 8; i ++) {
		__IIC_SCL_CLR();
		//Delay_us(1);
		__IIC_SCL_SET();
		chRecData <<= 1;
		if(__IIC_SDA_READ())
			chRecData |= 0x01;
		else
			chRecData &= ~0x01;
		//Delay_us(1);
	}

	if (IIC_ACK == chAck) {
		iic_ack();
	} else {
		iic_nack();
	}

	return chRecData;
}

uint8_t mOLED::iic_read_one_byte(uint8_t chDevAddr, uint8_t chRegAddr)
{
	uint8_t chTemp = 0;

	iic_start();
	iic_write_byte(chDevAddr | IIC_DIR_TX);
	iic_wait_for_ack();
	iic_write_byte(chRegAddr);
	iic_wait_for_ack();
	iic_start();
	iic_write_byte(chDevAddr | IIC_DIR_RX);
	iic_wait_for_ack();
	chTemp = iic_read_byte(IIC_NACK);
	iic_stop();

	return chTemp;
}

bool mOLED::iic_read_buffer(uint8_t chDevAddr, uint8_t chRegAddr, uint8_t chNum, uint8_t *pchBuffer)
{
	uint8_t i;

	if (0 == chNum || NULL == pchBuffer) {
		return false;
	}

	iic_start();
	iic_write_byte(chDevAddr | IIC_DIR_TX);
	iic_wait_for_ack();
	iic_write_byte(chRegAddr);
	iic_wait_for_ack();
	iic_start();
	iic_write_byte(chDevAddr | IIC_DIR_RX);
	iic_wait_for_ack();

	for (i = 0; i < chNum; i ++) {
		if ((chNum - 1) == i) {
			*(pchBuffer + i) = iic_read_byte(IIC_NACK);
		} else {
			*(pchBuffer + i) = iic_read_byte(IIC_ACK);
		}
	}

	iic_stop();

	return true;
}
#endif


extern uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  uint8_t bitstatus = 0x00;

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin));

  if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)GPIO_PIN_RESET)
  {
    bitstatus = (uint8_t)GPIO_PIN_SET;
  }
  else
  {
    bitstatus = (uint8_t)GPIO_PIN_SET;
  }
  return bitstatus;
}


#if !defined(SH1106) && !defined(SSD1306)
	#warning Please select first the target OLED device(SH1106 or SSD1306) in your application!
	#define SSD1306  //define SSD1306 by default
#endif

#if defined(SSD1306)
#define __SET_COL_START_ADDR() 	do { \
									ssd1306_write_byte(0x00, SSD1306_CMD); \
									ssd1306_write_byte(0x10, SSD1306_CMD); \
								} while(false)
#elif defined(SH1106)
#define __SET_COL_START_ADDR() 	do { \
									ssd1306_write_byte(0x02, SSD1306_CMD); \
									ssd1306_write_byte(0x10, SSD1306_CMD); \
								} while(false)
#endif



void mOLED::ssd1306_write_byte(uint8_t chData, uint8_t chCmd)
{
#ifdef INTERFACE_OLED_4WIRE_SPI

	__SSD1306_CS_CLR();

	if (chCmd) {
		__SSD1306_DC_SET();
	} else {
		__SSD1306_DC_CLR();
	}
	__SSD1306_WRITE_BYTE(chData);

	__SSD1306_DC_SET();
	__SSD1306_CS_SET();

#elif defined(INTERFACE_OLED_3WIRE_SPI)

	uint8_t i;
	uint16_t hwData = 0;

	if (chCmd) {
		hwData = (uint16_t)chData | 0x0100;
	} else {
		hwData = (uint16_t)chData & ~0x0100;
	}

	__SSD1306_CS_CLR();
	for(i = 0; i < 9; i ++) {
		__SSD1306_CLK_CLR();

		if(hwData & 0x0100) {
			__SSD1306_DIN_SET();
		} else {
			__SSD1306_DIN_CLR();
		}
		__SSD1306_CLK_SET();
		hwData <<= 1;
	}
	__SSD1306_CS_SET();

#elif defined(INTERFACE_OLED_IIC)

/*

	if (chCmd) {
		PC_I2C.I2C_send(0x78,0x40,chData);
	} else {
		PC_I2C.I2C_send(0x78,0x00,chData);
	}

*/
	iic_start();
	iic_write_byte(0x78);
	iic_wait_for_ack();
	if (chCmd) {
		iic_write_byte(0x40);
		iic_wait_for_ack();
	} else {
		iic_write_byte(0x00);
		iic_wait_for_ack();
	}
	iic_write_byte(chData);
	iic_wait_for_ack();

	iic_stop();

#endif
}


void mOLED::ssd1306_display_on(void)
{
	ssd1306_write_byte(0x8D, SSD1306_CMD);
	ssd1306_write_byte(0x14, SSD1306_CMD);
	ssd1306_write_byte(0xAF, SSD1306_CMD);
}


void mOLED::ssd1306_display_off(void)
{
	ssd1306_write_byte(0x8D, SSD1306_CMD);
	ssd1306_write_byte(0x10, SSD1306_CMD);
	ssd1306_write_byte(0xAE, SSD1306_CMD);
}



void mOLED::ssd1306_refresh_gram(void)
{
	uint8_t i, j;

	for (i = 0; i < 8; i ++) {
		ssd1306_write_byte(0xB0 + i, SSD1306_CMD);
		__SET_COL_START_ADDR();
		for (j = 0; j < 128; j ++) {
			ssd1306_write_byte(s_chDispalyBuffer[j][i], SSD1306_DAT);
		}
	}
}




void mOLED::ssd1306_clear_screen(uint8_t chFill)
{
	uint8_t i, j;

	for (i = 0; i < 8; i ++) {
		ssd1306_write_byte(0xB0 + i, SSD1306_CMD);
		__SET_COL_START_ADDR();
		for (j = 0; j < 128; j ++) {
			s_chDispalyBuffer[j][i] = chFill;
		}
	}

	ssd1306_refresh_gram();
}


void mOLED::ssd1306_draw_point(uint8_t chXpos, uint8_t chYpos, uint8_t chPoint)
{
	uint8_t chPos, chBx, chTemp = 0;

	if (chXpos > 127 || chYpos > 63) {
		return;
	}
	chPos = 7 - chYpos / 8; //
	chBx = chYpos % 8;
	chTemp = 1 << (7 - chBx);

	if (chPoint) {
		s_chDispalyBuffer[chXpos][chPos] |= chTemp;

	} else {
		s_chDispalyBuffer[chXpos][chPos] &= ~chTemp;
	}
}



void mOLED::ssd1306_fill_screen(uint8_t chXpos1, uint8_t chYpos1, uint8_t chXpos2, uint8_t chYpos2, uint8_t chDot)
{
	uint8_t chXpos, chYpos;

	for (chXpos = chXpos1; chXpos <= chXpos2; chXpos ++) {
		for (chYpos = chYpos1; chYpos <= chYpos2; chYpos ++) {
			ssd1306_draw_point(chXpos, chYpos, chDot);
		}
	}

	ssd1306_refresh_gram();
}


void mOLED::ssd1306_display_char(uint8_t chXpos, uint8_t chYpos, uint8_t chChr, uint8_t chSize, uint8_t chMode)
{
	uint8_t i, j;
	uint8_t chTemp, chYpos0 = chYpos;

	chChr = chChr - ' ';
    for (i = 0; i < chSize; i ++) {
		if (chSize == 12) {
			if (chMode) {
				chTemp = c_chFont1206[chChr][i];
			} else {
				chTemp = ~c_chFont1206[chChr][i];
			}
		} else {
			if (chMode) {
				chTemp = c_chFont1608[chChr][i];
			} else {
				chTemp = ~c_chFont1608[chChr][i];
			}
		}

        for (j = 0; j < 8; j ++) {
			if (chTemp & 0x80) {
				ssd1306_draw_point(chXpos, chYpos, 1);
			} else {
				ssd1306_draw_point(chXpos, chYpos, 0);
			}
			chTemp <<= 1;
			chYpos ++;

			if ((chYpos - chYpos0) == chSize) {
				chYpos = chYpos0;
				chXpos ++;
				break;
			}
		}
    }
}
static uint32_t pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	while(n --) result *= m;
	return result;
}


void mOLED::ssd1306_display_num(uint8_t chXpos, uint8_t chYpos, uint32_t chNum, uint8_t chLen, uint8_t chSize)
{
	uint8_t i;
	uint8_t chTemp, chShow = 0;

	for(i = 0; i < chLen; i ++) {
		chTemp = (chNum / pow(10, chLen - i - 1)) % 10;
		if(chShow == 0 && i < (chLen - 1)) {
			if(chTemp == 0) {
				ssd1306_display_char(chXpos + (chSize / 2) * i, chYpos, ' ', chSize, 1);
				continue;
			} else {
				chShow = 1;
			}
		}
	 	ssd1306_display_char(chXpos + (chSize / 2) * i, chYpos, chTemp + '0', chSize, 1);
	}
}


void mOLED::ssd1306_display_string(uint8_t chXpos, uint8_t chYpos,  const char *pchString, uint8_t chSize, uint8_t chMode)
{
    while (*pchString != '\0') {
        if (chXpos > (SSD1306_WIDTH - chSize / 2)) {
			chXpos = 0;
			chYpos += chSize;
			if (chYpos > (SSD1306_HEIGHT - chSize)) {
				chYpos = chXpos = 0;
				ssd1306_clear_screen(0x00);
			}
		}

        ssd1306_display_char(chXpos, chYpos, *pchString, chSize, chMode);
        chXpos += chSize / 2;
        pchString ++;
    }
}

void mOLED::ssd1306_draw_1616char(uint8_t chXpos, uint8_t chYpos, uint8_t chChar)
{
	uint8_t i, j;
	uint8_t chTemp = 0, chYpos0 = chYpos, chMode = 0;

	for (i = 0; i < 32; i ++) {
		chTemp = c_chFont1612[chChar - 0x30][i];
		for (j = 0; j < 8; j ++) {
			chMode = chTemp & 0x80? 1 : 0;
			ssd1306_draw_point(chXpos, chYpos, chMode);
			chTemp <<= 1;
			chYpos ++;
			if ((chYpos - chYpos0) == 16) {
				chYpos = chYpos0;
				chXpos ++;
				break;
			}
		}
	}
}

void mOLED::ssd1306_draw_3216char(uint8_t chXpos, uint8_t chYpos, uint8_t chChar)
{
	uint8_t i, j;
	uint8_t chTemp = 0, chYpos0 = chYpos, chMode = 0;

	for (i = 0; i < 64; i ++) {
		chTemp = c_chFont3216[chChar - 0x30][i];
		for (j = 0; j < 8; j ++) {
			chMode = chTemp & 0x80? 1 : 0;
			ssd1306_draw_point(chXpos, chYpos, chMode);
			chTemp <<= 1;
			chYpos ++;
			if ((chYpos - chYpos0) == 32) {
				chYpos = chYpos0;
				chXpos ++;
				break;
			}
		}
	}
}

void mOLED::ssd1306_draw_bitmap(uint8_t chXpos, uint8_t chYpos, const uint8_t *pchBmp, uint8_t chWidth, uint8_t chHeight)
{
	uint16_t i, j, byteWidth = (chWidth + 7) / 8;

    for(j = 0; j < chHeight; j ++){
        for(i = 0; i < chWidth; i ++ ) {
            if(*(pchBmp + j * byteWidth + i / 8) & (128 >> (i & 7))) {
                ssd1306_draw_point(chXpos + i, chYpos + j, 1);
            }
        }
    }
}


void mOLED::ssd1306_init(void){

#ifdef INTERFACE_OLED_4WIRE_SPI
	__SSD1306_CS_SET();   //CS set
	__SSD1306_DC_CLR();   //D/C reset
	__SSD1306_RES_SET();  //RES set

#elif defined(INTERFACE_OLED_3WIRE_SPI)
	__SSD1306_CS_CLR();   //CS reset
	__SSD1306_DC_CLR();   //D/C reset
	__SSD1306_RES_SET();  //RES set

#elif defined(INTERFACE_OLED_IIC)
	__SSD1306_CS_CLR();   //CS reset
	__SSD1306_DC_CLR();   //D/C reset
	__SSD1306_RES_SET();  //RES set

#endif

	ssd1306_write_byte(0xAE, SSD1306_CMD);//--turn off oled panel
	ssd1306_write_byte(0x00, SSD1306_CMD);//---set low column address
	ssd1306_write_byte(0x10, SSD1306_CMD);//---set high column address
	ssd1306_write_byte(0x40, SSD1306_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	ssd1306_write_byte(0x81, SSD1306_CMD);//--set contrast control register
	ssd1306_write_byte(0xCF, SSD1306_CMD);// Set SEG Output Current Brightness
	ssd1306_write_byte(0xA1, SSD1306_CMD);//--Set SEG/Column Mapping
	ssd1306_write_byte(0xC0, SSD1306_CMD);//Set COM/Row Scan Direction
	ssd1306_write_byte(0xA6, SSD1306_CMD);//--set normal display
	ssd1306_write_byte(0xA8, SSD1306_CMD);//--set multiplex ratio(1 to 64)
	ssd1306_write_byte(0x3f, SSD1306_CMD);//--1/64 duty
	ssd1306_write_byte(0xD3, SSD1306_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	ssd1306_write_byte(0x00, SSD1306_CMD);//-not offset
	ssd1306_write_byte(0xd5, SSD1306_CMD);//--set display clock divide ratio/oscillator frequency
	ssd1306_write_byte(0x80, SSD1306_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	ssd1306_write_byte(0xD9, SSD1306_CMD);//--set pre-charge period
	ssd1306_write_byte(0xF1, SSD1306_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	ssd1306_write_byte(0xDA, SSD1306_CMD);//--set com pins hardware configuration
	ssd1306_write_byte(0x12, SSD1306_CMD);
	ssd1306_write_byte(0xDB, SSD1306_CMD);//--set vcomh
	ssd1306_write_byte(0x40, SSD1306_CMD);//Set VCOM Deselect Level
	ssd1306_write_byte(0x20, SSD1306_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	ssd1306_write_byte(0x02, SSD1306_CMD);//
	ssd1306_write_byte(0x8D, SSD1306_CMD);//--set Charge Pump enable/disable
	ssd1306_write_byte(0x14, SSD1306_CMD);//--set(0x10) disable
	ssd1306_write_byte(0xA4, SSD1306_CMD);// Disable Entire Display On (0xa4/0xa5)
	ssd1306_write_byte(0xA6, SSD1306_CMD);// Disable Inverse Display On (0xa6/a7)
	ssd1306_write_byte(0xAF, SSD1306_CMD);//--turn on oled panel

	ssd1306_clear_screen(0x00);
}

/*-------------------------------END OF FILE-------------------------------*/

