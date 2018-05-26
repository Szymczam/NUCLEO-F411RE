/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

//
// Include
//
#include <main.h>
#include <iostream>
#include <stdio.h>
#include <float.h>
//
// Structures
//
MS_ADC_Value_t	  	ADC_value;
TM_RTC_t RTCD;

//
// Globals
//
char cpu[20];
char str[50];
uint8_t Transmit[15], Receive[15];
uint8_t value = 0xC;
unsigned char* frame_buffer = (unsigned char*)malloc(ePaper.width / 8 * ePaper.height);
//
// Functions
//

extern void Error_Handler(void);



//--------------------------f-------------------------------------------------
//                            Main
//---------------------------------------------------------------------------
//	SystemInit()
//__libc_init_array
int main(void){
	MRCC::SystemClock_Config();	// system clock frequency 100Mhz
	HAL_Init();					// initialize the HAL Library
	MRCC::Delay_init();
	GPIO::Init_all();
	PC_UART.Init_IT(USART2,115200),

	OLED_SPI.SPI_init(SPI3, SPI_BAUDRATEPRESCALER_8);
	ePAPER_SPI.SPI_init(SPI2, SPI_BAUDRATEPRESCALER_2);


	TIMER1.Period		=	100 - 1,	//	10Hz
	TIMER1.Prescaler	=	100000 - 1,	// 1kHz
	TIMER1.TIM_ADC_init(TIM3);
	PWM_LED.Period		=	100 - 1,
	PWM_LED.Pulse		=	50-1,
	PWM_LED.Prescaler	= 	10-1,	// clk = 10MHz
	PWM_LED.Channel		=	TIM_CHANNEL_1;
	//PWM_LED.PWM_init(TIM3);



//	PC_ADC.ADC_Init();
	PC_ADC.ADC_Init_IT(ADC_CHANNEL_0);


	OLED.OLED_init();
	OLED.ssd1306_display_string(0, 0, "Hello Mark", 16, 1);
	OLED.ssd1306_refresh_gram();

	ePaper.Init();
    Paint paint(frame_buffer, ePaper.width, ePaper.height);
    ePaper.DisplayFrame(gImage_ee);



//	SWTIM1 = TM_DELAY_TimerCreate(250, 1, 1, __interrupt_timer, NULL);// 100ms with auto-reload enabled
	if (TM_RTC_Init(TM_RTC_ClockSource_External)) {
		TM_RTC_Interrupts(TM_RTC_Int_1s);
		printf("RTC : Ok\r\n");
	} else {
		printf("RTC : Error\r\n");
	}

	SYS.CPU_LOAD_Init();

	printf("System core clock: %d MHz; HSE clock: %d MHz\r\n",
			(int) MRCC::GetClockSpeedinMHz(Clock_SYSCLK),
			(int) MRCC::GetClockSpeedinMHz(Clock_HSE));
	printf("PCLK1 (APB1) clock: %d MHz; PCLK2 (APB2) clock: %d MHz\r\n",
			(int) MRCC::GetClockSpeedinMHz(Clock_PCLK1),
			(int) MRCC::GetClockSpeedinMHz(Clock_PCLK2));
	mIWDG::IWDG_Init(IWDG_Timeout_4s);

//---------------------------------------------------------------------------
//                            Main program loop
//---------------------------------------------------------------------------
	for(;;){
		if (SYS.CPU.Updated) {
			sprintf(cpu,"CPU : %2.3f%%", SYS.CPU.Load);
			printf("CPU: %2.3f%%\r\n", SYS.CPU.Load);
		}
		asm ("  NOP");




 		SYS.CPU_LOAD_GoToSleepMode();
	}
}


//---------------------------------------------------------------------------
//                            Interrupt
//---------------------------------------------------------------------------

//void __interrupt_timer(TM_DELAY_Timer_t* SWTIM, void* UserParameters) {

//}

void RTC_WakeupHandler(void) {
	mIWDG::Reset();
	TM_RTC_GetDateTime(&RTCD, TM_RTC_Format_BCD);
	OLED.ssd1306_display_string(0, 0, cpu, 16, 1);
	OLED.ssd1306_draw_3216char(0,32, ((RTCD.Hours)>>4) + 0x30);
	OLED.ssd1306_draw_3216char(16,32, ((RTCD.Hours) & 0x0F ) + 0x30);
	OLED.ssd1306_draw_3216char(32,32, ':');
	OLED.ssd1306_draw_3216char(48,32, ((RTCD.Minutes)>>4) + 0x30);
	OLED.ssd1306_draw_3216char(64,32, ((RTCD.Minutes) & 0x0F ) + 0x30);
	OLED.ssd1306_draw_3216char(80,32, ':');
	OLED.ssd1306_draw_3216char(96,32, ((RTCD.Seconds)>>4) + 0x30);
	OLED.ssd1306_draw_3216char(112,32, ((RTCD.Seconds) & 0x0F) + 0x30);
	OLED.ssd1306_refresh_gram();
	if (RTCD.Seconds == 0){
		sprintf(str, "Time: %02d:%02d", bcdToDec(RTCD.Hours), bcdToDec(RTCD.Minutes));
		Paint paint(frame_buffer, ePaper.width, ePaper.height);
		paint.Clear(COLORED);
		paint.DrawStringAt(5, 5, "Hello Marek!", &Font24, UNCOLORED);
	    paint.DrawStringAt(5, 300-24, str, &Font24, UNCOLORED);
	    ePaper.DisplayFrame(frame_buffer);
	}
//	PC_UART.Puts(cpu);

}

void __EXTI_Interrupt(uint16_t GPIO_Pin){
	if(GPIO_Pin == BUTTON.Pin){
		printf("Button click:\r\n");
/*		ADC_value.ADC_A0 = PC_ADC.ADC_Read(ADC_CHANNEL_0);
		ADC_value.ADC_A1 = PC_ADC.ADC_Read(ADC_CHANNEL_1);
		ADC_value.ADC_A2 = PC_ADC.ADC_Read(ADC_CHANNEL_4);
		ADC_value.ADC_A3 = PC_ADC.ADC_Read(ADC_CHANNEL_8);
		ADC_value.ADC_A4 = PC_ADC.ADC_Read(ADC_CHANNEL_11);
		ADC_value.ADC_A5 = PC_ADC.ADC_Read(ADC_CHANNEL_10);

		printf("AdcA1 = %ld (%.3fV), ", ADC_value.ADC_A1, ADC_value.ADC_A1 * 3.3f / 4095.0f);
		printf("AdcA2 = %ld (%.3fV);\r\n", ADC_value.ADC_A2, ADC_value.ADC_A2 * 3.3f / 4095.0f);
		printf("AdcA3 = %ld (%.3fV), ", ADC_value.ADC_A3, ADC_value.ADC_A3 * 3.3f / 4095.0f);
		printf("AdcA4 = %ld (%.3fV), ", ADC_value.ADC_A4, ADC_value.ADC_A4 * 3.3f / 4095.0f);
		printf("AdcA5 = %ld (%.3fV);\r\n", ADC_value.ADC_A5, ADC_value.ADC_A5 * 3.3f / 4095.0f);
*/		//printf("AdcA0 = %ld (%.3fV), ", ADC_value.ADC_A0, ADC_value.ADC_A0 * 3.3f / 4095.0f);
		OLED.ssd1306_draw_bitmap(0, 0, &Rysunek12[0], SSD1306_WIDTH, SSD1306_HEIGHT);
		OLED.ssd1306_refresh_gram();
		ePaper.DisplayFrame(gImage_ee);
		while(1);
	}
}




/*
* @brief This function handles USART2 global interrupt.
*/
void USART2_IRQHandler(void){
	//Check if interrupt was because data is received
//	if (USART2->SR & USART_SR_RXNE) {
		// Call user function
		HAL_UART_IRQHandler(&PC_UART.USART_handle);
//	}

}

void __TIM_Interrupt(void) {
//	LED.TogglePinValue();
//	printf("TIM:\r\n");
	//printf("AdcA0 = %ld (%.3fV), ", ADC_value.ADC_A0, ADC_value.ADC_A0 * 3.3f / 4095.0f);
}

void TIM4_IRQHandler(void){
	__TIM_Interrupt();
	HAL_TIM_IRQHandler(&TIMER1.TIMx);

}

void USARTx_DMA_RX_IRQHandler(void){
	HAL_UART_Receive_DMA(&PC_UART.USART_handle, PC_UART.Received, 1);
	HAL_DMA_IRQHandler(PC_UART.USART_handle.hdmarx);
}


void USARTx_DMA_TX_IRQHandler(void){
	HAL_UART_Transmit_DMA(&PC_UART.USART_handle, PC_UART.Received, 1);
	HAL_DMA_IRQHandler(PC_UART.USART_handle.hdmatx);
}

void ADC_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&PC_ADC.ADCx);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle){
	ADC_value.ADC_A0 = HAL_ADC_GetValue(&PC_ADC.ADCx);
}


void WWDG_IRQHandler(void){
  HAL_WWDG_IRQHandler(&My_WWDG.WWDGx);

}

void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef* hwwdg){
	  printf("ERROR:\r\n");
}

//---------------------------------------------------------------------------
//                            Functions
//---------------------------------------------------------------------------
void Error_Handler(void){
  while(1);
}
