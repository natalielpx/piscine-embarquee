#include "embd.h"

void setup( void ) {

	SET_PIN(DDRB, LED_D1, OUTPUT);		// Set PB0 to output
	SET_PIN(DDRD, SWITCH_1, INPUT);		// Set PD2 to input
	SET_PIN(PORTD, SWITCH_1, PULL_UP);	// Enable pull-up on PD2
}

int main( void ) {
	
	int prev = PULL_UP, curr = PULL_UP;

	while (42) {

		// Obtain current status of switch
		curr = READ_PIN(PIND, SWITCH_1);

		if (curr != prev) {
			// Debounce
			_delay_ms(50);
			// Turn LED_D1 on or off
			SET_PIN(PORTB, LED_D1, !curr);
			// Record status of switch
			prev = curr;
		}
	}
}