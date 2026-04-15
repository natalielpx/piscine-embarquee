#include "embd.h"

void setup( void ) {

	SET_PIN(DDRB, LED_D2, OUTPUT);		// Set PB1 to output
}

int main( void ) {
	
	setup();

	while (42) {
		
		// Toggle LED D2
		TOGGLE_PIN(PORTB, LED_D2);

		// Wait 0.5 seconds
		_delay_ms(500);
	}
}
