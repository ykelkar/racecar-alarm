#ifndef _RTC_H_
#define _RTC_H_

//	#include "stdutils.h"

//declare struct to store all time variables
typedef struct
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t weekDay;
	uint8_t date;
	uint8_t month;
	uint8_t year;
}rtc_t;

//Below values are fixed in reference tp  DS3231 DataSheet

#define C_DS3231Read					0xD1u  // DS3231 ID
#define C_DS3231Write					0xD0u  // DS3231 ID

#define C_DS3231SECReg					0x00u   // Address to access DS3231 SEC register
#define C_DS3231DATAReg					0x04u   // Address to access DS3231 DATE register
#define C_DS3231CTRLReg					0x07u   // Address to access DS3231 CONTROL register

//RTC Functions
void RTC_init();
void RTC_Set(rtc_t *rtc);
void RTC_Get(rtc_t *rtc);

#endif
