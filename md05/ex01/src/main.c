#include "../embd.h"

#define POT 0
#define LDR 1
#define NTC 2

void adc_init( void ) {

	// ==== Components ====
	SET_PIN(DDRC, ADC_POT, INPUT);
	SET_PIN(DDRC, ADC_LDR, INPUT);
	SET_PIN(DDRC, ADC_NTC, INPUT);

	// ==== ADC Multiplexer Selection Register ====
	// ref: datasheet pg256 table24-3
	ADMUX |= (0 << REFS1) | (1 << REFS0);					// Voltage Reference: AVCC with external capacitor at AREF pin
	ADMUX |= (1 << ADLAR);									// ADC Left Adjust Result
	ADMUX |= POT;											// Analog Channel: Potentiometer
	DIDR0 |= (1 << ADC2D)| (1 << ADC1D) | (1 << ADC0D);		// Disable corresponding digital input buffer(s)

	// ==== ADC Control and Status Register A ====
	// ref: datasheet pg257
	ADCSRA |= (1 << ADEN);									// Enable ADC
	ADCSRA |= (1 << ADIE);									// Enable ADC interrupts
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	// Division Factor: 128
	ADCSRA |= (1 << ADSC);									// Start ADC conversions
}

volatile uint8_t adc[3] = {0};

void __attribute__(( signal, used, externally_visible )) ADC_vect( void ) {

	static uint8_t channel = 0;

	adc[channel] = ADCH;			// Record conversion result

	channel = (channel + 1) % 3;	// Switch Channel
	ADMUX &= ~(0b00001111);			// Clear MUX3:0
	ADMUX |= channel;				// Reset MUX3:0

	ADCSRA |= (1 << ADSC);			// Trigger next conversion
}

void __attribute__(( signal, used, externally_visible )) TIMER1_COMPA_vect( void ) {

    // Print POT result
    uart_print_hex(adc[POT]);

    uart_tx(',');
    uart_tx(' ');

    // Print LDR result
    uart_print_hex(adc[LDR]);

    uart_tx(',');
    uart_tx(' ');

    // Print NTC result
    uart_print_hex(adc[NTC]);

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
