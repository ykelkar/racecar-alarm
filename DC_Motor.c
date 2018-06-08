/*	This program is used to program the slave microcontroller to receive the signal 
	from the master microcontroller indicating that the alarm has gone off. When the 
	alarm goes off, the motors are activated. To turn off the motors, a button needs 
	to be pressed. When the button is pressed, a signal is sent back to the master
	microcontroller and resets the alarm*/

#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 8000000UL
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include "usart_ATmega1284.h"


#define  Trigger_pin	PC7	/* Trigger pin */

int TimerOverflow = 0;

unsigned char obj_detect = 0;

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

void InitPWM()
{
 	TCCR0A = (1 << COM0A1 | 1 << COM0A0 | 1 << WGM00 | 1 << WGM01); // Toggle OC1A on Compare Match Fast PWM
	TCCR0B = (1 << CS00);	

	TCCR2A = (1 << COM2A1 | 1 << COM2A0 | 1 << WGM20 | 1 << WGM21);
	TCCR2B = (1 << CS20); //Toggle OC2A on Compare Match Fast PWM
	
}
unsigned char PD5_high = 255;
unsigned char PD5_low = 10;
unsigned char PD7_high = 0;
unsigned char PD7_low = 20;

enum PWMStates {Init, Wait, Turn} PWM_state;
void PWM_Tick() 
{
	// Transitions
	switch(PWM_state) 
	{
		
		case Init:
		PWM_state = Wait;
		break;
		
		case Wait:
		if (obj_detect == 1)
		{
			PWM_state = Turn;
		}
		else 
		{
			PWM_state = Wait;
		}
		break;
		
		case Turn:
		if (obj_detect == 1)
		{
			PWM_state = Turn;
		}
		else 
		{
			PWM_state = Wait;
		}
		break;
	
		default:
		PWM_state = Init;
		break;
	}
	
	// Actions
	switch(PWM_state) 
	{
		case Init:
		OCR1A = 0;
		break;
		
		case Wait:
		OCR1A =	PD5_high; //Toggle PD5
		OCR2A = PD7_high; //Toggle PD7
		PORTD |= 0xA0;
		break;
		
		case Turn:
		OCR1A =	PD5_low; //Toggle PD5
		OCR2A = PD7_high; //Toggle PD7
		PORTD |= 0xA0;
		
		default:
		break;
	}
}

enum sensorState {Sense} sensor_state;
void Sensor()
{
	unsigned long count;
	double distance;	
	switch (sensor_state)
	{
		case Sense:
		/* Give 10us trigger pulse on trig. pin to HC-SR04 */
		PORTC |= (1 << Trigger_pin);
		_delay_us(10);
		PORTC &= (~(1 << Trigger_pin));
		
		TCNT1 = 0;	/* Clear Timer counter */
		TCCR1B = 0x41;	/* Capture on rising edge, No prescaler*/
		TIFR1 = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
		TIFR1 = 1<<TOV1;	/* Clear Timer Overflow flag */

		/*Calculate width of Echo by Input Capture (ICP) */
		
		while ((TIFR1 & (1 << ICF1)) == 0);/* Wait for rising edge */
		TCNT1 = 0;	/* Clear Timer counter */
		TCCR1B = 0x01;	/* Capture on falling edge, No prescaler */
		TIFR1 = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
		TIFR1 = 1<<TOV1;	/* Clear Timer Overflow flag */
		TimerOverflow = 0;/* Clear Timer overflow count */

		while ((TIFR1 & (1 << ICF1)) == 0);/* Wait for falling edge */
		count = ICR1 + (65535 * TimerOverflow);	/* Take count */
		distance = (double)count / 466.47;
		
		if (distance < 10)
		{
			PORTC = 0x40;
			obj_detect = 1;
		}
		else if (distance > 10)
		{
			PORTC = 0x00;
			obj_detect = 0;
		}
		_delay_ms(200);
		break;
	}
}


enum States {Init1, Receive, Off_Release, On_Press, On_Release, Off_Press} state;

void Tick() {
	unsigned char A0 = ~PINA & 0x01;
	
	switch(state) { //Transitions
		case Init1:
		
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
		case Init1:
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

int main()
{
	DDRA = 0xF0; PORTA = 0x0F;
	DDRD = 0xBF; PORTD = 0x40;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	
	unsigned long count;
	double distance;
	
	sei();			/* Enable global interrupt */
	TIMSK1 = (1 << TOIE1);	/* Enable Timer1 overflow interrupts */
	TCCR1A = 0;		/* Set all bit to zero Normal operation */
	
	PWM_state = Init;
	sensor_state = Sense;
	state = Init1;
	TimerSet(100);
	TimerOn();
	InitPWM();

	while(1)
	{
		PWM_Tick();
		Sensor();
		Tick();
	}
}
