/*
 * RTC.h
 *
 *  Created on: 31 mar 2018
 *      Author: Marek
 */

#ifndef MSZ_RTC_H_
#define MSZ_RTC_H_

/*
 *
 * \par Pinout for RTC external 32768Hz crystal
 *
\verbatim
 STM32Fxxx  Oscillator   Description

 PC14       OSC1         Oscillator terminal 1
 PC15       OSC2         Oscillator terminal 2
*/


#include "stm32fxxx_hal.h"

/* RTC clock is: f_clk = RTCCLK(LSI or LSE) / ((RTC_SYNC_PREDIV + 1) * (RTC_ASYNC_PREDIV + 1)) */
/* Sync pre division for clock */
#ifndef RTC_SYNC_PREDIV
#define RTC_SYNC_PREDIV                 0x3FF
#endif
/* Async pre division for clock */
#ifndef RTC_ASYNC_PREDIV
#define RTC_ASYNC_PREDIV                0x1F
#endif
/* NVIC global Priority set */
#ifndef RTC_NVIC_PRIORITY
#define RTC_NVIC_PRIORITY               0x04
#endif
/* Sub priority for wakeup trigger */
#ifndef RTC_NVIC_WAKEUP_SUBPRIORITY
#define RTC_NVIC_WAKEUP_SUBPRIORITY     0x00
#endif
/* Sub priority for alarm trigger */
#ifndef RTC_NVIC_ALARM_SUBPRIORITY
#define RTC_NVIC_ALARM_SUBPRIORITY      0x01
#endif

extern uint8_t bcdToDec(uint8_t val);

extern uint8_t decToBcd(uint8_t val);
/**
 * @brief  RTC Struct for date/time
 */
typedef struct {
	uint8_t Seconds;     /*!< Seconds parameter, from 00 to 59 */
	uint16_t Subseconds; /*!< Subsecond downcounter. When it reaches zero, it's reload value is the same as
                                 @ref RTC_SYNC_PREDIV, so in our case 0x3FF = 1023, 1024 steps in one second */
	uint8_t Minutes;     /*!< Minutes parameter, from 00 to 59 */
	uint8_t Hours;       /*!< Hours parameter, 24Hour mode, 00 to 23 */
	uint8_t WeekDay;     /*!< Day in a week, 1 to 7 */
	uint8_t Day;         /*!< Day in a month, 1 to 31 */
	uint8_t Month;       /*!< Month in a year, 1 to 12 */
	uint8_t Year;        /*!< Year parameter, 00 to 99, 00 is 2000 and 99 is 2099 */
	uint32_t Unix;       /*!< Seconds from 01.01.1970 00:00:00 */
} TM_RTC_t;


/*
 * @brief RTC Result enumeration
 */
typedef enum {
	TM_RTC_Result_Ok,   /*!< Everything OK */
	TM_RTC_Result_Error /*!< An error occurred */
} TM_RTC_Result_t;

/*
 * @brief RTC date and time format
 */
typedef enum {
	TM_RTC_Format_BIN = 0x00, /*!< RTC data in binary format */
	TM_RTC_Format_BCD         /*!< RTC data in binary-coded decimal format */
} TM_RTC_Format_t;

/*
 * @brief  RTC Interrupt enumeration
 */
typedef enum {
	TM_RTC_Int_Disable = 0x00, /*!< Disable RTC wakeup interrupts */
	TM_RTC_Int_60s,            /*!< RTC Wakeup interrupt every 60 seconds */
	TM_RTC_Int_30s,            /*!< RTC Wakeup interrupt every 30 seconds */
	TM_RTC_Int_15s,            /*!< RTC Wakeup interrupt every 15 seconds */
	TM_RTC_Int_10s,            /*!< RTC Wakeup interrupt every 10 seconds */
	TM_RTC_Int_5s,             /*!< RTC Wakeup interrupt every 5 seconds */
	TM_RTC_Int_2s,             /*!< RTC Wakeup interrupt every 2 seconds */
	TM_RTC_Int_1s,             /*!< RTC Wakeup interrupt every 1 seconds */
	TM_RTC_Int_500ms,          /*!< RTC Wakeup interrupt every 500 milliseconds */
	TM_RTC_Int_250ms,          /*!< RTC Wakeup interrupt every 250 milliseconds */
	TM_RTC_Int_125ms           /*!< RTC Wakeup interrupt every 125 milliseconds */
} TM_RTC_Int_t;

/*
 * @brief  Select RTC clock source
 * @note   Internal clock is not accurate and should not be used in production
 */

typedef enum {
	TM_RTC_ClockSource_Internal = 0x00, /*!< Use internal clock source for RTC (LSI oscillator) */
	TM_RTC_ClockSource_External         /*!< Use external clock source for RTC (LSE oscillator) */
} TM_RTC_ClockSource_t;

/*
 * @brief  RTC Alarm type
 */
typedef enum {
	TM_RTC_AlarmType_DayInWeek, /*!< Trigger alarm every day in a week, days from 1 to 7 (Monday to Sunday) */
	TM_RTC_AlarmType_DayInMonth /*!< Trigger alarm every month */
} TM_RTC_AlarmType_t;

/*
 * @brief  Alarm identifier you will use for Alarm functions
 */
typedef enum {
	TM_RTC_Alarm_A = 0x00, /*!< Work with alarm A */
	TM_RTC_Alarm_B         /*!< Work with alarm B */
} TM_RTC_Alarm_t;

/*
 * @brief  RTC structure for alarm time
 */
typedef struct {
	TM_RTC_AlarmType_t Type; /*!< Alarm type setting. @ref TM_RTC_AlarmType_t for more info */
	uint8_t Seconds;         /*!< Alarm seconds value */
	uint8_t Minutes;         /*!< Alarm minutes value */
	uint8_t Hours;           /*!< Alarm hours value */
	uint8_t Day;             /*!< Alarm day value. If you select trigger for alarm every week, then this parameter has value between
                                     1 and 7, representing days in a week, Monday to Sunday
                                     If you select trigger for alarm every month, then this parameter has value between
                                     1 - 31, representing days in a month. */
} TM_RTC_AlarmTime_t;



/*
 * @brief  Initializes RTC and starts counting
 * @param  source. RTC Clock source @ref TM_RTC_ClockSource_t to be used for RTC
 * @note   Internal clock source is not so accurate
 * @note   If you reset your MCU and RTC still has power, it will count independent of MCU status
 * @retval Returns RTC status.
 *            - 1: RTC has already been initialized and time is set
 *            - 0: RTC was now initialized first time. Now you can set your first clock
 */
uint32_t TM_RTC_Init(TM_RTC_ClockSource_t source);

/*
 * @brief  Gets number of seconds from date and time since 01.01.1970 00:00:00
 * @param  *data: Pointer to @ref TM_RTC_t data structure
 * @retval Calculated seconds from date and time since 01.01.1970 00:00:00
 */
uint32_t TM_RTC_GetUnixTimeStamp(TM_RTC_t* data);

/*
 * @brief  Gets formatted time from seconds till 01.01.1970 00:00:00
 *         It fills struct with valid data
 * @note   Valid if year is greater or equal (>=) than 2000
 * @param  *data: Pointer to @ref TM_RTC_Time_t struct to store formatted data in
 * @param  unix: Seconds from 01.01.1970 00:00:00 to calculate user friendly time
 * @retval Member of @ref TM_RTC_Result_t enumeration
 */
TM_RTC_Result_t TM_RTC_GetDateTimeFromUnix(TM_RTC_t* data, uint32_t unix);

/*
 * @brief  Selects RTC wakeup interrupts interval
 * @note   This function can also be used to disable interrupt
 * @param  int_value: Look for @ref TM_RTC_Int_t for valid inputs
 * @retval Member of @ref TM_RTC_Result_t enumeration
 */
TM_RTC_Result_t TM_RTC_Interrupts(TM_RTC_Int_t int_value);

/*
 * @brief  Sets date and time to internal RTC registers
 * @param  *data: Pointer to filled @ref TM_RTC_t structure with date and time
 * @param  format: Format of your structure. This parameter can be a value of @ref TM_RTC_Format_t enumeration
 * @retval Member of @ref TM_RTC_Result_t enumeration
 *            - @ref TM_RTC_Result_Ok: Date and Time set OK
 *            - @ref TM_RTC_Result_Error: Date and time is wrong
 */
TM_RTC_Result_t TM_RTC_SetDateTime(TM_RTC_t* data, TM_RTC_Format_t format);

/*
 * @brief  Sets date and time using string formatted date time
 * @note   Valid string format is: <b>dd.mm.YY.x;HH:ii:ss</b>
 *            - <b>dd</b>: date, 2 digits, decimal
 *            - <b>mm</b>: month, 2 digits, decimal
 *            - <b>YY</b>: year, last 2 digits, decimal
 *            - <b>x</b>: day in a week: 1 digit, 1 = Monday, 7 = Sunday
 *            - <b>HH</b>: hours, 24-hour mode, 2 digits, decimal
 *            - <b>ii</b>: minutes, 2 digits, decimal
 *            - <b>ss</b>: seconds, 2 digits, decimal
 * @param  *str: Pointer to string with datetime format
 * @retval RTC datetime status @ref TM_RTC_Result_t:
 *            - @ref TM_RTC_Result_Ok: Date and Time set OK
 *            - @ref TM_RTC_Result_Error: Date and time is wrong
 */
TM_RTC_Result_t TM_RTC_SetDateTimeString(char* str);

/*
 * @brief  Gets date and time from internal RTC registers
 * @param  *data: Pointer to @ref TM_RTC_t structure to save data to
 * @param  format: Format of your structure. This parameter can be a value of @ref TM_RTC_Format_t enumeration
 * @retval Member of @ref TM_RTC_Result_t enumeration
 */
TM_RTC_Result_t TM_RTC_GetDateTime(TM_RTC_t* data, TM_RTC_Format_t format);

/*
 * @brief  Gets number of days in month
 * @note   This function also detects if it is leap year and returns different number for February
 * @param  month: Month number in year, valid numbers are 1 - 12
 * @param  year: Year number where you want to get days in month, last 2 digits
 * @retval Number of days in specific month and year
 */
uint8_t TM_RTC_GetDaysInMonth(uint8_t month, uint8_t year);

/*
 * @brief  Gets number of days in specific year
 * @note   This function also detects if it is leap year
 * @param  year: Year number where you want to get days in month, last 2 digits
 * @retval Number of days in year
 */
uint16_t TM_RTC_GetDaysInYear(uint8_t year);

/*
 * @brief  Writes RTC backup register value.
 *            This method allows you to write 32bit value from backup register 0 - 18 for STM32F4xx and 0 - 30 for STM32F7xx
 * @note   RTC has backup registers where you can store data which will be available all the time RTC is running and has power.
 *
 * @note   My library uses register 19 to store info about RTC settings and is not available for USER to store data there.
 *
 * @note   RTC HAS to be initialized first before you can use this method.
 * @param  location: RTC backup register location 0 - 18 for STM32F4xx and 0 - 30 for STM32F7xx are valid
 * @param  value: 32-bit long value to be stored in RTC backup register
 * @retval Value at specific RTC backup register location
 */
void TM_RTC_WriteBackupRegister(uint8_t location, uint32_t value);

/*
 * @brief  Reads RTC backup register value.
 *            This method allows you to read 32bit value from backup register 0 - 18 for STM32F4xx and 0 - 30 for STM32F7xx
 * @note   RTC has backup registers where you can store data which will be available all the time RTC is running and has power.
 *
 * @note   My library uses register 19 to store info about RTC settings and is not available for USER to store data there.
 *
 * @note   RTC HAS to be initialized first before you can use this method.
 * @param  location: RTC backup register location 0 - 18 for STM32F4xx and 0 - 30 for STM32F7xx are valid
 * @retval Value at specific RTC backup register location
 */
uint32_t TM_RTC_ReadBackupRegister(uint8_t location);

/*
 * @brief  Enables alarm A or alarm B
 * @param  Alarm: Specify which alarm to set. This parameter can be a value of @ref TM_RTC_Alarm_t enumeration
 * @param  *AlarmTime: Pointer to @ref TM_RTC_AlarmTime_t structure to get data from.
 * @param  format: RTC date and time format. This parameter can be a value of @ref TM_RTC_Format_t enumeration.
 * @retval Member of @ref TM_RTC_Result_t enumeration
 */
TM_RTC_Result_t TM_RTC_EnableAlarm(TM_RTC_Alarm_t Alarm, TM_RTC_AlarmTime_t* AlarmTime, TM_RTC_Format_t format);

/*
 * @brief  Disables specific alarm
 * @param  Alarm: Select which alarm you want to disable. This parameter can be a value of @ref TM_RTC_Alarm_t enumeration
 * @retval Member of @ref TM_RTC_Result_t enumeration
 */
TM_RTC_Result_t TM_RTC_DisableAlarm(TM_RTC_Alarm_t Alarm);

/*
 * @brief  RTC Wakeup handler function. Called when wakeup interrupt is triggered
 * @note   Called from my RTC library
 * @param  None
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void RTC_WakeupHandler(void);

/*
 * @brief  RTC Alarm A handler function. Called when interrupt is triggered for alarm A
 * @note   Called from my RTC library
 * @param  None
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void RTC_AlarmAHandler(void);

/*
 * @brief  RTC Alarm B handler function. Called when interrupt is triggered for alarm B.
 * @note   Called from my RTC library
 * @param  None
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void RTC_AlarmBHandler(void);

#endif /* MSZ_RTC_H_ */
