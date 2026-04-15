#include <avr/io.h>
#include <util/delay.h>

#define LED_D1 PB0
#define SWITCH_1 PD2
#define INPUT 0
#define OUTPUT 1
#define LOW 0
#define HIGH 1

void set_pin( volatile uint8_t * reg, unsigned int pin, int state) {

	if (state)
		*reg |= (1 << pin);
	else
		*reg &= ~(1 << pin);
}

int read_pin( volatile uint8_t * reg, unsigned int pin) {

	return !!(*reg & (1 << pin));
}

int main( void ) {
	
	set_pin(&DDRB, LED_D1, OUTPUT);		// Set PB0 to output
	set_pin(&DDRD, SWITCH_1, INPUT);	// Set PD2 to input
	set_pin(&PORTD, SWITCH_1, 1);		// Enable pull-up on PD2

	int prev = 0, curr = 0;

	while (42) {

		curr = read_pin(&PIND, SWITCH_1);

		if (curr != prev) {
			_delay_ms(50);
			// if curr == 0, set LED_D1 to 1 (HIGH)
			// if curr == 1, set LED_D1 to 0 (LOW)
			set_pin(&PORTB, LED_D1, !curr);
			prev = curr;
		}
	}
}