/*	Author: Cruz Ramirez
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab 6  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *	Create a light timer game that stops the light where it is when button is pressed.
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: Youtube URL> https://www.youtube.com/watch?v=5uxRma6EHHg
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {

        TCCR1B = 0x0B;

        OCR1A = 125;

        TIMSK1 = 0x02;

        TCNT1 = 0;

        _avr_timer_cntcurr = _avr_timer_M;

        SREG |= 0x80;
}

void TimerOff(){
        TCCR1B = 0x00;
}

void TimerISR(){
        TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){

        _avr_timer_cntcurr--;
        if(_avr_timer_cntcurr == 0){
                TimerISR();
                _avr_timer_cntcurr = _avr_timer_M;
        }
}

void TimerSet(unsigned long M){
        _avr_timer_M = M;
        _avr_timer_cntcurr = _avr_timer_M;
}


enum LED_States{LED_Start, LED_ZeroOn, LED_OneOn, LED_TwoOn, LED_Wait1, LED_Wait2}LED_State;

void TickFct_LED(){

	unsigned char temp = ~PINA;
	switch(LED_State){ //state transitions

		case LED_Start:
		LED_State = LED_ZeroOn;
		break;

		case LED_ZeroOn:
		if(temp == 0x00){
		LED_State = LED_OneOn;
		}
		else{
		LED_State = LED_Wait1;
		}
		
		break;

		case LED_OneOn:
		if(temp == 0x00){
		LED_State = LED_TwoOn;
		}
		else{
		LED_State = LED_Wait1;
		}

		break;

		case LED_TwoOn:
		if(temp == 0x00){
		LED_State = LED_ZeroOn;
		}
		else{
		LED_State = LED_Wait1;
		}
		break;

		case LED_Wait1:
		if(temp == 0x00){
		LED_State = LED_Wait2;
		}
		else{
		LED_State = LED_Wait1;
		}
		break;
		
		case LED_Wait2:
		if(temp == 0x01){
		LED_State = LED_ZeroOn;
		}
		else{
		LED_State = LED_Wait2;
		}
		break;

		default:
		break;
	}

	switch(LED_State){ //state actions

		case LED_ZeroOn:
		PORTB = 0x01;
		break;

		case LED_OneOn:
		PORTB = 0x02;
		break;

		case LED_TwoOn:
		PORTB = 0x04;
		break;

		case LED_Start:
		case LED_Wait1:
		case LED_Wait2:
		default:
		break;

	}
}



int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00;
	DDRB = 0xFF;
	PORTB = 0x00;
	PORTA = 0xFF;
	TimerSet(300);
	TimerOn();
	LED_State = LED_Start;
    /* Insert your solution below */
    while (1) {
	TickFct_LED();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 0;
}
