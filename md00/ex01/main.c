#include <avr/io.h>

#define LED_D1 PB0

int main( void ) {

	// Turn On LED_D1
	DDRB |= (1 << LED_D1);		// Set LED_D1 to output
	PORTB |= (1 << LED_D1);		// Set LED_D1 to HIGH
}