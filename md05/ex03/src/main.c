#include "../embd.h"

void adc_init( void ) {

	// ==== ADC Multiplexer Selection Register ====
	// ref: datasheet pg256 table24-3
	ADMUX |= (1 << REFS1) | (1 << REFS0);			// Voltage Reference: Internal 1.1V with external capacitor at AREF pin
	ADMUX &= ~(1 << ADLAR);							// ADC Right Adjust Result
	ADMUX |= 0b1000;								// Analog Channel: Temperature sensor

	// ==== ADC Control and Status Register A ====
	// ref: datasheet pg257
	ADCSRA |= (1 << ADEN);									// Enable ADC
	ADCSRA |= (1 << ADIE);									// Enable ADC interrupts
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	// Division Factor: 128
	ADCSRA |= (1 << ADATE);									// Enable ADC auto triggering
	ADCSRA |= (1 << ADSC);									// Start ADC conversions
	
	// ==== ADC Control and Status Register B ====
	// ref: datasheet pg260
	ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));	// Free Running
}

volatile uint16_t temperature = 0;

void __attribute__(( signal, used, externally_visible )) ADC_vect( void ) {

	// Read result
	uint16_t reading;
	reading = ADCL;
	reading |= (ADCH << 8);

	// Calculate temperature
	if (reading < 289)
		temperature = 0;				// Sorry, no negative numbers
	else
		temperature = reading - 289;	// Magic number
}

void __attribute__(( signal, used, externally_visible )) TIMER1_COMPA_vect( void ) {

    // Print adc result
	uart_print_dec(temperature);
    uart_tx('\r');
    uart_tx('\n');
}

int main( void ) {

	SREG |= (1 << 7);	// Global Interrupt
	uart_init();
	adc_init();
	timers_init();

	// Empty loop
	while (42);
}
