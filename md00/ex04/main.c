#include "embd.h"

void display_number( int num ) {

	SET_PIN(PORTB, LED_D1, num & 1);
	SET_PIN(PORTB, LED_D2, num & (1 << 1));
	SET_PIN(PORTB, LED_D3, num & (1 << 2));
	SET_PIN(PORTB, LED_D4, num & (1 << 3));
}

void setup( void ) {

	SET_PIN(DDRB, LED_D1, OUTPUT);		// Set PB0 to output
	SET_PIN(DDRB, LED_D2, OUTPUT);		// Set PB1 to output
	SET_PIN(DDRB, LED_D3, OUTPUT);		// Set PB2 to output
	SET_PIN(DDRB, LED_D4, OUTPUT);		// Set PB4 to output

	SET_PIN(DDRD, SWITCH_1, INPUT);		// Set PD2 to input
	SET_PIN(PORTD, SWITCH_1, PULL_UP);	// Enable pull-up on PD2
	SET_PIN(DDRD, SWITCH_2, INPUT);		// Set PD4 to input
	SET_PIN(PORTD, SWITCH_2, PULL_UP);	// Enable pull-up on PD4
}

int main( void ) {
	
	setup();

	int number = 0;
	int sw1 = 0;
	int sw2 = 0;

	while (42) {

		// Switch 1
		// If Switch 1 not pressed, record sw1 as not pressed
		if (READ_PIN(PIND, SWITCH_1)) {
			_delay_ms(30);
			sw1 = 0;
		// If Switch 1 pressed, increment and record sw1 as pressed
		} else if (!sw1) {
			_delay_ms(30);
			if (number < 15)
				display_number(++number);
			sw1 = 1;
		}

		// Switch 2
		// If Switch 2 not pressed, record sw2 as not pressed
		if (READ_PIN(PIND, SWITCH_2)) {
			_delay_ms(30);
			sw2 = 0;
		// If Switch 2 pressed, decrement and record sw2 as pressed
		} else if(!sw2) {
			_delay_ms(30);
			if (number > 0)
				display_number(--number);
			sw2 = 1;
		}
	}
}