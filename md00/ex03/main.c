#include "embd.h"

void setup( void ) {

	SET_PIN(DDRB, LED_D1, OUTPUT);		// Set PB0 to output
	SET_PIN(DDRD, SWITCH_1, INPUT);		// Set PD1 to input
	SET_PIN(PORTD, SWITCH_1, PULL_UP);	// Enable pull-up on PD2
}

int main( void ) {

	setup();

	while (42) {

		// Switch not pressed
		if (READ_PIN(PIND, SWITCH_1))
			continue;
		
		// Swtich press detected

		// Debounce 
		_delay_ms(50);
		
		// Confirm switch pressed
		if (!READ_PIN(PIND, SWITCH_1)) {

			TOGGLE_PIN(PORTB, LED_D1);

			// Wait for switch release
			while (!READ_PIN(PIND, SWITCH_1))
				_delay_ms(20);
		}
	}
}