#include "../embd.h"

void adc_init( void ) {

	// ==== Components ====
	SET_PIN(DDRC, ADC_POT, INPUT);

	// ==== ADC Multiplexer Selection Register ====
	// ref: datasheet pg256 table24-3
	ADMUX |= (0 << REFS1) | (1 << REFS0);	// Voltage Reference: AVCC with external capacitor at AREF pin
	ADMUX |= (1 << ADLAR);					// ADC Left Adjust Result
	ADMUX |= 0b0000;						// Analog Channel: Potentiometer
	DIDR0 |= (1 << ADC0D);					// Disable corresponding digital input buffer(s)

	// ==== ADC Control and Status Register A ====
	// ref: datasheet pg257
	ADCSRA |= (1 << ADEN);									// Enable ADC
	ADCSRA |= (1 << ADATE);									// Enable ADC auto triggering
	ADCSRA |= (1 << ADIE);									// Enable ADC interrupts
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	// Division factor: 128
	ADCSRA |= (1 << ADSC);									// Start ADC conversions

	// ==== ADC Control and Status Register B ====
	// ref: datasheet pg260
	ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));	// Free Running
}

volatile uint8_t adc_result = 0;

void __attribute__(( signal, used, externally_visible )) ADC_vect( void ) {

	adc_result = ADCH;
}

void __attribute__(( signal, used, externally_visible )) TIMER1_COMPA_vect( void ) {

    // Print result from ADC_vect
    uart_print_hex(adc_result);
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
