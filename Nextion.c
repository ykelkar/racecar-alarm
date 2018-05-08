//Turns on LED if receives value from Nextion

#include <avr/interrupt.h>
#include "usart_ATmega1284.h"


volatile unsigned char TimerFlag = 0;
unsigned long avr_timer_M = 1;
unsigned long avr_timer_cntcurr = 0;

unsigned char valB = 0x00;
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


enum States{Start, PORT_A_RECEIVE} state;
unsigned char port_A_value=0x00;
unsigned char counter_val=0x00;
void Tick()
{
	switch (state){
		
		case Start:
		state=PORT_A_RECEIVE;
		break;
		
		case PORT_A_RECEIVE:
		state=PORT_A_RECEIVE;
		break;
		
		default:
		break;
	}
	
	switch (state){

		case PORT_A_RECEIVE:
		if(USART_HasReceived(0))
		{
			port_A_value = 0x01;
			USART_Flush(0);
			PORTA = port_A_value;
		}
		break;
		
		default:
		break;
	}
}

int main(void)
{
	DDRA = 0xFF;PORTA = 0x00;
	initUSART(0);
	USART_Flush(0);
	TimerSet(100);
	TimerOn();
	
	state = Start;
	while(1)
	{
		Tick();
		
		while(!TimerFlag);
		TimerFlag = 0;
	}
}