#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart_ATmega1284.h"

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

#include <avr/io.h>

enum States {Init, Receive, Off_Release, On_Press, On_Release, Off_Press} state;

void Tick() {
	unsigned char A0 = ~PINA & 0x01;
	
	switch(state) { //Transitions
		case Init:
		
		case Receive:
		if (USART_HasReceived(0))
		{
			state = Off_Release;
		}
		else
		{
			state = Receive;
		}
		break;
		
		case Off_Release:
		if (!A0)
		{
			state = Off_Release;
		}
		else
		{
			state = On_Press;
		}
		break;
		
		case On_Press:
		if (A0)
		{
			state = On_Press;
		}
		else
		{
			state = On_Release;
		}
		break;
		
		case On_Release:
		if (!A0)
		{
			state = On_Release;
		}
		else
		{
			state = Off_Press;
		}
		break;
		
		case Off_Press:
		if (A0)
		{
			state = Off_Press;
		}
		else
		{
			state = Off_Release;
		}
		break;
	}
	switch(state) { //State Actions
		case Init:
		PORTB = 0x00;
		break;
		
		case Receive:
		break;
		
		case Off_Release:
		PORTB = 0x09;
		break;
		
		case On_Press:
		PORTB = 0x00;
		break;
		
		case On_Release:
		if (USART_IsSendReady(0))
		{
			USART_Send(0xFF, 0);
		}
		break;
		
		case Off_Press:
		PORTB = 0x09;
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x0F; PORTB = 0x00;          // initialize port B
	// motors connected across PC0...Pc3
	
	state = Init;
	initUSART(0);
	USART_Flush(0);
	TimerSet(100);
	TimerOn();
	
	while(1)
	{
		Tick();
		
		while(!TimerFlag);
		TimerFlag = 0;
	}
}