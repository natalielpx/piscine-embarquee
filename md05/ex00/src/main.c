#include "../embd.h"

void adc_init( void ) {

	// ==== Components ====
	SET_PIN(DDRC, ADC_POT, INPUT);

	// ==== ADC Multiplexer Selection Register ====
	// Voltage Reference Selection: AVCC with external capacitor at AREF pin
	// REFS1 = 0
	// REFS0 = 1
	// ref: datasheet pg256 table24-3
	ADMUX |= (1 << REFS0);
	// ADC Left Adjust Result
	ADMUX |= (1 << ADLAR);
	// Analog Channel Selection
	ADMUX |= ADC_CH0;
	// Disable digital input buffer
	DIDR0 |= (1 << ADC0D);

	// ==== ADC Control and Status Register A ====
	// ref: datasheet pg257
	ADCSRA |= (1 << ADEN);									// Enable ADC
	ADCSRA |= (1 << ADATE);									// Enable ADC auto triggering
	ADCSRA |= (1 << ADIE);									// Enable ADC interrupts
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	// Division Factor: 128
	
	// ==== ADC Control and Status Register B ====
	// ref: datasheet pg260
	ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));	// Free Running
}

volatile uint8_t adc_result = 0;

void __attribute__(( signal, used, externally_visible )) ADC_vect( void ) {

	adc_result = ADCH;
}

uint8_t bin_to_hex_char( uint8_t bin ) {

    return ((bin < 10) ? bin + '0': bin - 10 + 'a');
}

void __attribute__(( signal, used, externally_visible )) TIMER1_COMPA_vect( void ) {

    // Print result from ADC_vect
    uart_tx(bin_to_hex_char(adc_result >> 4));
    uart_tx(bin_to_hex_char(adc_result & 0b00001111));
    uart_tx('\r');
    uart_tx('\n');
}

void mc_init( void ) {

	SREG |= (1 << 7);	// Global Interrupt
	uart_init();
	adc_init();
	timers_init();

	// Start ADC
	ADCSRA |= (1 << ADSC);
}

int main( void ) {

	mc_init();

	// Empty loop
	while (42);
}
