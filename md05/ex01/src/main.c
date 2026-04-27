#include "../embd.h"

#define POT 0
#define LDR 1
#define NTC 2

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
	ADMUX |= POT;
	// Disable digital input buffer
	DIDR0 |= (1 << ADC2D)| (1 << ADC1D) | (1 << ADC0D);

	// ==== ADC Control and Status Register A ====
	// ref: datasheet pg257
	ADCSRA |= (1 << ADEN);									// Enable ADC
	ADCSRA |= (1 << ADIE);									// Enable ADC interrupts
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	// Division Factor: 128
	ADCSRA |= (1 << ADSC);									// Start Conversion
	
	// ==== ADC Control and Status Register B ====
	// ref: datasheet pg260
	ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));	// Free Running
}

volatile uint8_t adc[3] = {0};

void __attribute__(( signal, used, externally_visible )) ADC_vect( void ) {

	static uint8_t channel = 0;

	adc[channel] = ADCH;	// Record conversion result

	channel = (channel + 1) % 3;	// Switch Channel
	ADMUX &= ~(0b00001111);			// Clear MUX3:0
	ADMUX |= channel;				// Reset MUX3:0

	ADCSRA |= (1 << ADSC);		// Trigger next conversion
}

static inline uint8_t bin_to_hex_char( uint8_t bin ) {

    return ((bin < 10) ? bin + '0': bin - 10 + 'a');
}

void __attribute__(( signal, used, externally_visible )) TIMER1_COMPA_vect( void ) {

    // Print POT result
    uart_tx(bin_to_hex_char(adc[POT] >> 4));
    uart_tx(bin_to_hex_char(adc[POT] & 0b00001111));

    uart_tx(',');
    uart_tx(' ');

    // Print LDR result
    uart_tx(bin_to_hex_char(adc[LDR] >> 4));
    uart_tx(bin_to_hex_char(adc[LDR] & 0b00001111));

    uart_tx(',');
    uart_tx(' ');

    // Print NTC result
    uart_tx(bin_to_hex_char(adc[NTC] >> 4));
    uart_tx(bin_to_hex_char(adc[NTC] & 0b00001111));

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
