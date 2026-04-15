#include "embd.h"

void setup( void ) {

	SET_PIN(DDRB, LED_D1, OUTPUT);		// Set PB0 to output
}

int main( void ) {
	
	setup();

	while (42) {
		
		// Toggle LED D1
		TOGGLE_PIN(PORTB, LED_D1);

		// Wait 0.5 seconds
		_delay_ms(500);
	}
}
