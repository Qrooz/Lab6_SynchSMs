/*	Author: Cruz Ramirez
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab 6  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *	LED increment and Decrement according to button presses.
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: Youtube URL> https://www.youtube.com/watch?v=CgRBed7xPfk
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

unsigned char i = 0x00;


enum B_States{B_Incr, B_IncrWait, B_Decr, B_DecrWait, B_Wait, B_Zero}B_State;

void TickFct_B(){

	unsigned char inv = ~PINA; //inverse inputs to account for hardware
	switch(B_State){ //transition state

        case B_Wait:
        if(inv == 0x01){
        B_State = B_Incr;
        }
	else if(inv == 0x02){
	B_State = B_Decr;
	}
	else if(inv == 0x03){
	B_State = B_Zero;
	}
        break;

        case B_Incr:
	if(inv == 0x03){
	B_State = B_Zero;
	}
	else if(inv == 0x02){
	B_State = B_Decr;
	}
	else if(inv == 0x00){
	B_State = B_Wait;
	}
	else{
	B_State = B_IncrWait;
	}
        break;

	case B_IncrWait:
	if(inv == 0x00){
	B_State = B_Wait;
	i = 0x00;
	}
	else if(inv == 0x02){
	B_State = B_Decr;
	i = 0x00;
	}
	else if(inv == 0x03){
	B_State = B_Zero;
	i =0x00;
	}
	else if(inv == 0x01 && i < 9){
		B_State = B_IncrWait;
		++i;
	}
	else if(inv == 0x01 && i <= 9){
		B_State = B_Incr;
		i = 0x00;
	}
	break;

        case B_Decr:
	if(inv == 0x03){
	B_State = B_Zero;
	}
	else if(inv == 0x01){
	B_State = B_Incr;
	}
	else if(inv == 0x00){
	B_State = B_Wait;
	}
	else{
	B_State = B_DecrWait;
	}
        break;

	case B_DecrWait:
	if(inv == 0x00){
	B_State = B_Wait;
	i = 0x00;
	}
	else if(inv == 0x01){
	B_State = B_Incr;
	i = 0x00;
	}
	else if (inv == 0x03){
	B_State = B_Zero;
	i = 0x00;
	}
	else if(inv == 0x02 && i < 9){
        B_State = B_DecrWait;
        ++i;
        }
        else if(inv == 0x02 && i <= 9){
        B_State = B_Decr;
        i = 0x00;
        }

	break;

        case B_Zero:
        B_State = B_Wait;
	
	if(inv == 0x01){
        B_State = B_Incr;
        }
        else if(inv == 0x02){
        B_State = B_Decr;
        }
        else if(inv == 0x03){
        B_State = B_Zero;
        }
        break;

        default:
        break;
        }

	switch(B_State){ //state actions

	case B_Incr:
	if(PORTB < 9){
	++PORTB;
	}
	break;

	case B_Decr:
	if(PORTB > 0){
	--PORTB;
	}
	break;

	case B_Zero:
	PORTB = 0x00;
	break;

	case B_Wait:
	case B_IncrWait:
	case B_DecrWait:
	default:
	break;

	}

}



int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x07;
	B_State = B_Wait;

	TimerSet(100);
	TimerOn();
	//assign first state variable
    /* Insert your solution below */
    while (1) {
	TickFct_B();
	
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 0;
}
