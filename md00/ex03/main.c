#include <avr/io.h>
#include <util/delay.h>

#define LED_D1 PB0
#define SWITCH_1 PD2

#define INPUT 0
#define OUTPUT 1

#define LOW 0
#define HIGH 1

#define OFF 0
#define ON 1

void set_pin( volatile uint8_t * reg, unsigned int pin, int state) {

	if (state)
		*reg |= (1 << pin);
	else
		*reg &= ~(1 << pin);
}

int read_pin( volatile uint8_t * reg, unsigned int pin) {

	return !!(*reg & (1 << pin));	// !! turns any non-zero values to 1
}

int main( void ) {
	
	set_pin(&DDRB, LED_D1, OUTPUT);		// Set PB0 to output
	set_pin(&DDRD, SWITCH_1, INPUT);	// Set PD1 to input
	set_pin(&PORTD, SWITCH_1, 1);		// Enable pull-up on PD2

	int state = OFF;

	while (42) {

		if (read_pin(&PIND, SWITCH_1))
			continue;
		
		while (!read_pin(&PIND, SWITCH_1))
			_delay_ms(10);
	
		set_pin(&PORTB, LED_D1, !state);
		state = !state;
	}
}