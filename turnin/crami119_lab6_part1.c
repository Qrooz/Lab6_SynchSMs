/*	Author: Cruz Ramirez
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab 6  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *	Create a set of three lights that alternate being on for 1 second.
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: Youtube URL> https://www.youtube.com/watch?v=onPxOvSYjJM
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


enum LED_States{LED_Start, LED_ZeroOn, LED_OneOn, LED_TwoOn}LED_State;

void TickFct_LED(){

	switch(LED_State){ //state transitions

		case LED_Start:
		LED_State = LED_ZeroOn;
		break;

		case LED_ZeroOn:
		LED_State = LED_OneOn;
		break;

		case LED_OneOn:
		LED_State = LED_TwoOn;
		break;

		case LED_TwoOn:
		LED_State = LED_ZeroOn;
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
		default:
		break;

	}
}



int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF;
	PORTB = 0x00;
	TimerSet(1000);
	TimerOn();
    /* Insert your solution below */
    while (1) {

	TickFct_LED();
	while(!TimerFlag);
	TimerFlag = 0;

    }
    return 1;
}
