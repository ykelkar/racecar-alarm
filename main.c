#include <stdio.h>
#include <avr/interrupt.h>
#include "C:\Users\Avanti\Documents\Atmel Studio\7.0\GccApplication1\GccApplication1\rtc\rtc.c"
#include "C:\Users\Avanti\Documents\Atmel Studio\7.0\GccApplication1\GccApplication1\rtc\lcd.c"
#include "usart_ATmega1284.h"

rtc_t rtc;
rtc_t alarm;

volatile unsigned char TimerFlag = 0;
unsigned long avr_timer_M = 1;
unsigned long avr_timer_cntcurr = 0;

void TimerOn()
{
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	avr_timer_cntcurr = avr_timer_M;
	SREG = 0x80;
}

void TimerOff()
{
	TCCR1B = 0x00;
}

void TimerISR()
{
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect)
{
	avr_timer_cntcurr--;
	if (avr_timer_cntcurr == 0){
		TimerISR();
		avr_timer_cntcurr = avr_timer_M;
		
	}
}

void TimerSet(unsigned long M)
{
	avr_timer_M = M;
	avr_timer_cntcurr = avr_timer_M;
	
}
uint8_t alarm_hour = 0x00;
uint8_t alarm_min = 0x00;

enum Menu_options {Init, Time_Display, ALARM, Time_Display_Release, Change_Time, Transition, Change_Date, Set_Alarm, Set_Hour, Set_Minute, Set_Sec, Set_Date, Set_Month, Set_Year, Reset, Set_Alarm_Hour, Set_Alarm_Min} option;
void Menu()
{
	unsigned char sec_button = ~PINA & 0x04;		//button that controls seconds and year
	unsigned char minute_button = ~PINA & 0x08;		//controls minutes and date
	unsigned char hour_button = ~PINA & 0x10;		//controls hour and month
	unsigned char next_button = ~PINA & 0x20;		//goes to next page
	unsigned char home_button = ~PINA & 0x40;		//goes back to menu
	switch (option) //Transistions
	{
		case Init:
		option = Time_Display;
		break;
		
		case Time_Display:
		if (next_button == 0x20)
		{
			option = Time_Display_Release;
		}
		// 		else if (home_button == 0x40)
		// 		{
		// 			option = ALARM;
		// 		}
		else if((rtc.hour == alarm_hour) && (rtc.min == alarm_min))
		{
			option = ALARM;
		}
		else
		{
			option = Time_Display;
		}
		break;
		
		case Time_Display_Release:
		option = Change_Time;
		break;
		
		case Change_Time:
		if (hour_button == 0x10)
		{
			option = Set_Hour;
		}
		else if (minute_button == 0x08)
		{
			option = Set_Minute;
		}
		else if (sec_button == 0x04)
		{
			option = Set_Sec;
		}
		else if (next_button == 0x20)
		{
			option = Transition;
		}
		else if (home_button == 0x40)
		{
			option = Init;
		}
		else
		{
			option = Change_Time;
		}
		break;
		
		case Transition:
		option = Change_Date;
		break;
		
		case Set_Hour:
		option = Change_Time;
		break;
		
		case Set_Minute:
		option = Change_Time;
		break;
		
		case Set_Sec:
		option = Change_Time;
		break;

		case Change_Date:
		if (sec_button == 0x04)
		{
			option = Set_Year;
		}
		else if (minute_button == 0x08)
		{
			option = Set_Date;
		}
		else if (hour_button == 0x10)
		{
			option = Set_Month;
		}
		else if (next_button == 0x20)
		{
			option = Reset;
		}
		else if (home_button == 0x40)
		{
			option = Init;
		}
		else
		{
			option = Change_Date;
		}
		break;
		
		case Set_Date:
		option = Change_Date;
		break;
		
		case Set_Month:
		option = Change_Date;
		break;
		
		case Set_Year:
		option = Change_Date;
		break;
		
		case Reset:
		option = Set_Alarm;
		
		case Set_Alarm:
		if (hour_button == 0x10)
		{
			option = Set_Alarm_Hour;
		}
		else if (minute_button == 0x08)
		{
			option = Set_Alarm_Min;
		}
		else if (home_button == 0x40)
		{
			option = Init;
		}
		else
		{
			option = Set_Alarm;
		}
		break;
		
		case Set_Alarm_Hour:
		option = Set_Alarm;
		break;
		
		case Set_Alarm_Min:
		option = Set_Alarm;
		break;
		
		case ALARM:
		if (USART_HasReceived(0))
		{
			option = Init;
		}
		else
		{
			option = ALARM;
		}
		break;
	}
	
	switch (option) //Actions
	{
		case Init:
		alarm_hour = alarm.hour;
		alarm_min = alarm.min;
		LCD_Clear();
		break;
		
		case Time_Display:
		RTC_Get(&rtc);
		if (rtc.hour > 0x12)
		{
			rtc.hour = 0x01;
		}
		LCD_GoToLine(0);
		LCD_Printf("time:%2x:%2x:%2x  \nDate:%2x/%2x/%2x",(uint16_t)rtc.hour,(uint16_t)rtc.min,(uint16_t)rtc.sec,(uint16_t)rtc.month,(uint16_t)rtc.date,(uint16_t)rtc.year);
		break;
		
		case Time_Display_Release:
		LCD_Clear();
		break;
		
		case Change_Time:
		RTC_Set(&rtc);
		RTC_Get(&rtc);
		LCD_GoToLine(0);
		LCD_Printf("setTime:%2x:%2x:%2x",(uint16_t)rtc.hour,(uint16_t)rtc.min,(uint16_t)rtc.sec);
		break;
		
		case Transition:
		LCD_Clear();
		break;
		
		case Change_Date:
		RTC_Set(&rtc);
		RTC_Get(&rtc);
		LCD_GoToLine(0);
		LCD_Printf("setDate:%2x/%2x/%2x",(uint16_t)rtc.month,(uint16_t)rtc.date,(uint16_t)rtc.year);
		break;
		
		case Set_Hour:
		rtc.hour = rtc.hour + 1;
		if (rtc.hour == 0x0A)
		{
			rtc.hour = 0x10;
		}
		else if (rtc.hour > 0x12)
		{
			rtc.hour = 0x01;
		}
		break;
		
		case Set_Minute:
		rtc.min = rtc.min + 1;
		if ((rtc.min & 0x0A) == 0x0A)				//check last 4 bits
		{
			rtc.min = (rtc.min & 0xF0) + 0x10;		//store first 4 bits only and increment by 10
		}
		if (rtc.min == 0x60)
		{
			rtc.min = 0x00;
		}
		break;
		
		case Set_Sec:
		rtc.sec = rtc.sec + 1;
		if ((rtc.sec & 0x0A) == 0x0A)
		{
			rtc.sec = (rtc.sec & 0xF0) + 0x10;
		}
		if (rtc.sec == 0x60)
		{
			rtc.sec = 0x00;
		}
		break;
		
		case Reset:
		LCD_Clear();
		break;

		case Set_Date:
		rtc.date = rtc.date + 1;
		if ((rtc.date & 0x0A) == 0x0A)
		{
			rtc.date = (rtc.date & 0xF0) + 0x10;
		}
		if (rtc.month == 0x01 || 0x03 || 0x05 || 0x07 || 0x08 || 0x10 || 0x12)
		{
			if (rtc.date > 0x31)
			{
				rtc.date = 0x01;
			}
		}
		if (rtc.month == 0x04 || 0x06 || 0x09 || 0x11)
		{
			if (rtc.date > 0x30)
			{
				rtc.date = 0x01;
			}
		}
		if (rtc.month == 0x02)
		{
			if (rtc.date > 0x28)
			{
				rtc.date = 0x01;
			}
		}
		break;
		
		case Set_Month:
		rtc.month = rtc.month + 1;
		if (rtc.month == 0x0A)
		{
			rtc.month = 0x10;
		}
		else if (rtc.month > 0x12)
		{
			rtc.month = 0x01;
		}
		break;
		
		case Set_Year:
		rtc.year = rtc.year + 1;
		if ((rtc.year & 0x0A) == 0x0A)				//check last 4 bits
		{
			rtc.year = (rtc.year & 0xF0) + 0x10;		//store first 4 bits only and increment by 10
		}
		if (rtc.year == 0x60)
		{
			rtc.year = 0x00;
		}
		break;

		case Set_Alarm:
		RTC_Set(&rtc);
		RTC_Get(&rtc);
		LCD_GoToLine(0);
		LCD_Printf("alarm:%2x:%2x",(uint16_t)alarm.hour, (uint16_t)alarm.min);
		break;
		
		case Set_Alarm_Hour:
		alarm.hour = alarm.hour + 1;
		if (alarm.hour == 0x0A)
		{
			alarm.hour = 0x10;
		}
		else if (alarm.hour > 0x12)
		{
			alarm.hour = 0x01;
		}
		break;
		
		case Set_Alarm_Min:
		alarm.min = alarm.min + 1;
		if ((alarm.min & 0x0A) == 0x0A)				//check last 4 bits
		{
			alarm.min = (alarm.min & 0xF0) + 0x10;		//store first 4 bits only and increment by 10
		}
		if (alarm.min == 0x60)
		{
			alarm.min = 0x00;
		}
		break;
		
		case ALARM:
		if (USART_IsSendReady(0))
		{
			USART_Send(0xFF, 0);
		}
		
		if (USART_HasTransmitted(0))
		{
			LCD_Clear();
		}
		break;
		
	}
}

int main()
{
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x03; PORTA = 0xFC;		//2 lower bits set to output for the LCD and the rest to input for the buttons
	option = Init;
	
	initUSART(0);
	/*Connect RS->PA0, RW->GND, EN->PA1 and data bus to PB0-PB7*/
	LCD_SetUp(PA_0,P_NC,PA_1,PB_0,PB_1,PB_2,PB_3,PB_4,PB_5,PB_6,PB_7);
	LCD_Init(2,16);
	
	/*Connect SCL->PC0, SDA->PC1*/
	RTC_init();
	TimerSet(10);
	TimerOn();
	rtc.hour = 0x12;		//  testing to hard code all values for hour, min, sec, date, etc.
	rtc.min =  0x44;
	rtc.sec =  0x30;

	rtc.date = 0x05; //1st Jan 2016
	rtc.month = 0x02;
	rtc.year = 0x18;
	rtc.weekDay = 5; // Friday: 5th day of week considering Monday as first day.
	
	alarm.hour = 0x12;
	alarm.min = 0x46;

	
	RTC_Set(&rtc);  //sets the date and the time using RTC and I2C

	while(1)
	{
		Menu();
		while(!TimerFlag);
		TimerFlag = 0;
	}

	return (0);
}