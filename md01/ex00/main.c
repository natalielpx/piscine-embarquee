#include "embd.h"

void setup( void ) {

	SET_PIN(DDRB, LED_D2, OUTPUT);		// Set PB1 to output
}

int main( void ) {

	setup();
	
	// rjmp (2 cycles)
	while(42) {

		// ldi r24,lo8(-122)	(1 cycle)
        // ldi r25,lo8(5)		(1 cycle)
        // std Y+2,r25			(2 cycle)
        // std Y+1,r24			(2 cycle)
		// nop					(1 cycle)
		volatile unsigned int i = 700;		// 7 cycles


		// ldd r24,Y+1		(2 cycles)
        // ldd r25,Y+2		(2 cycles)
        // sbiw r24,1		(2 cycles)
        // std Y+2,r25		(2 cycles)
        // std Y+1,r24		(2 cycles)
        // sbiw r24,0		(2 cycles)
        // brne .L2			(1 or 2 cycles)
		while(--i) {		// 13~14 + 7 + j * 13~14 cycles for each loop

			// ldi r24,lo8(-122)	(1 cycle)
			// ldi r25,lo8(5)		(1 cycle)
			// std Y+2,r25			(2 cycles)
			// std Y+1,r24			(2 cycles)
			// nop					(1 cycle)
			volatile unsigned int j = 815;		// 7 cycles

			// ldd r24,Y+3		(2 cycles)
			// ldd r25,Y+4		(2 cycles)
			// sbiw r24,1		(2 cycles)
			// std Y+4,r25		(2 cycles)
			// std Y+3,r24		(2 cycles)
			// sbiw r24,0		(2 cycles)
			// brne .L3			(1 or 2 cycles)
			while(--j);		// 13~14 cycles for each empty loop
		}

		// EOR (1 cycle)
		TOGGLE_PIN(PORTB, LED_D2);

		// Total: 2 + 7 + i * (13~14 + 7 + j * 13~14) + 1 cycles
	}
}