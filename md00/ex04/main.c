#include <avr/io.h>
#include <util/delay.h>

#define LED_D1 PB0
#define LED_D2 PB1
#define LED_D4 PB4
#define SWITCH_1 PD2
#define SWITCH_2 PD4

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

	return !!(*reg & (1 << pin));	// !! turns any non-zero values to 1
}

int button_pressed( volatile uint8_t * reg, unsigned int pin ) {

	if (!read_pin(reg, pin))
		return (_delay_ms(30), 1);

	return 0;
}

int button_letgo( volatile uint8_t * reg, unsigned int pin ) {

	if (read_pin(reg, pin))
		return (_delay_ms(30), 1);

	return 0;
}

int main( void ) {
	
	set_pin(&DDRB, LED_D1, OUTPUT);		// Set PB0 to output
	set_pin(&DDRB, LED_D2, OUTPUT);		// Set PB1 to output
	set_pin(&DDRB, LED_D4, OUTPUT);		// Set PB4 to output

	set_pin(&DDRD, SWITCH_1, INPUT);	// Set PD2 to input
	set_pin(&PORTD, SWITCH_1, 1);		// Enable pull-up on PD2
	set_pin(&DDRD, SWITCH_2, INPUT);	// Set PD4 to input
	set_pin(&PORTD, SWITCH_2, 1);		// Enable pull-up on PD4

	int number = 0;
	int sw1 = 1, sw2 = 1;

	while (42) {

		// Check if button is pressed
		sw1 = button_pressed(&PIND, SWITCH_1);
		sw2 = button_pressed(&PIND, SWITCH_2);

		// Wait for button to be let go
		if (sw1 && button_letgo(&PIND, SWITCH_1) && number < 7)
			++number;
		if (sw2 && button_letgo(&PIND, SWITCH_2) && number > 0)
			--number;

		// Display number
		set_pin(&PORTB, LED_D1, number & 1);
		set_pin(&PORTB, LED_D2, number & (1 << 1));
		set_pin(&PORTB, LED_D4, number & (1 << 2));
	}
}