#include "rtc.h"
#include "i2c.c"

/***************************************************************************************************
 *	Function: void RTC_init()
 *	Inputs: none
 *	Outputs: none
 *	Description: This function is used to initialize the DS3231 RTC module.
 *************************************************************************************************/
void RTC_init()
{
    I2C_Init();                             // Initialize the I2C module.
    I2C_Start();                            // Start I2C communication

    I2C_Write(C_DS3231Write);				// Connect to DS3231 by sending its ID on I2C Bus
    I2C_Write(C_DS3231SECReg);				// Select the DS3231 ControlRegister to configure DS3231

    I2C_Write(0x00);                        // Write 0x00 to Control register to disable SQW-Out

    I2C_Stop();                             // Stop I2C communication after initializing DS3231
}

/***************************************************************************************************
 *	Function: void RTC_Set(rtc_t *rtc)
 *	Inputs: Pointer to struct variable rtc which contains sec, min, hour, weekday, date, month, year
 *	Outputs: none
 *	Description: This function is used to update the Date and time of DS3231 RTC module. The new
 *				date and time is updated in the non-volatile memory of the DS3231
 *  Format of input: Binary-Coded Decimal (BCD) formatting. Ex: To set hour to 12, use 0x12.
 *************************************************************************************************/

void RTC_Set(rtc_t *rtc)
{
    I2C_Start();							// Start I2C communication

    I2C_Write(C_DS3231Write);				// connect to DS1307 by sending its ID on I2c Bus
    I2C_Write(C_DS3231SECReg);				// Request SEC RAM address at 00h

    I2C_Write(rtc->sec);                    // Write sec from RAM address 00h
    I2C_Write(rtc->min);                    // .. min .. 01h
    I2C_Write(rtc->hour);                   // .. hour .. 02h
    I2C_Write(rtc->weekDay);                // .. weekDay .. 03h
    I2C_Write(rtc->date);                   // .. date .. 04h
    I2C_Write(rtc->month);                  // .. month .. 05h
    I2C_Write(rtc->year);                   // .. year .. 06h

    I2C_Stop();                             // Stop I2C communication after Setting the Date
}

/***************************************************************************************************
 *	Function: void RTC_Get(rtc_t *rtc)
 *	Inputs: Pointer to struct variable rtc which contains sec, min, hour, weekday, date, month, year
 *	Outputs: none
 *	Description: This function is used to get the data and time from the DS3231 RTC module.
 *  Format of input: Binary-Coded Decimal (BCD) formatting. Ex: To set hour to 12, use 0x12.
 *************************************************************************************************/

void RTC_Get(rtc_t *rtc)
{
    I2C_Start();                            // Start I2C communication

    I2C_Write(C_DS3231Write);				// connect to DS3231 by sending its ID on I2c Bus
    I2C_Write(C_DS3231SECReg);				// Request SEC RAM address at 00H

    I2C_Stop();                             // Stop I2C communication after selecting Sec Register

    I2C_Start();                            // Start I2C communication
    I2C_Write(C_DS3231Read);				// connect to DS3231(Read mode) by sending its ID

    rtc->sec = I2C_Read(1);					// read second and return Positive ACK
    rtc->min = I2C_Read(1);                 // .. minute .. Positive ACK
    rtc->hour = I2C_Read(1);					// .. hour .. Negative/No ACK
    rtc->weekDay = I2C_Read(1);				// .. weekDay .. Positive ACK
    rtc->date = I2C_Read(1);					// .. date .. Positive ACK
    rtc->month = I2C_Read(1);					// .. month .. Positive ACK
    rtc->year = I2C_Read(0);					// .. year .. Negative/No ACK

    I2C_Stop();                              // Stop I2C communication after reading the Date
}
