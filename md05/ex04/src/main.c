#include "../embd.h"

void adc_init( void ) {

	// ==== Components ====
	SET_PIN(DDRC, ADC_POT, INPUT);

	// ==== ADC Multiplexer Selection Register ====
	// ref: datasheet pg256 table24-3
	ADMUX |= (0 << REFS1) | (1 << REFS0);	// Voltage Reference: AVCC with external capacitor at AREF pin
	ADMUX |= (1 << ADLAR);					// ADC Left Adjust Result
	ADMUX |= 0x0000;						// Analog Channel: Potentiometer
	DIDR0 |= (1 << ADC0D);					// Disable corresponding digital input buffer(s)

	// ==== ADC Control and Status Register A ====
	// ref: datasheet pg257
	ADCSRA |= (1 << ADEN);									// Enable ADC
	ADCSRA |= (1 << ADIE);									// Enable ADC interrupts
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	// Division Factor: 128
	ADCSRA |= (1 << ADSC);									// Start ADC conversions
}

volatile uint8_t adc_result = 0;

void __attribute__(( signal, used, externally_visible )) ADC_vect( void ) {

	adc_result = ADCH;
}

#define MAX 0xff

void __attribute__(( signal, used, externally_visible )) TIMER1_COMPA_vect( void ) {

	// Adjust colour wheel
	wheel(adc_result);

	// Light up LEDs
	SET_PIN(PORTB, LED_D1, (adc_result > MAX / 4));
	SET_PIN(PORTB, LED_D2, (adc_result > MAX / 2));
	SET_PIN(PORTB, LED_D3, (adc_result > MAX * 3 / 4));
	SET_PIN(PORTB, LED_D4, (adc_result == MAX));

	// Start another conversion
	ADCSRA |= (1 << ADSC);
}

int main( void ) {

	SET_PIN(DDRB, LED_D1, OUTPUT);
	SET_PIN(DDRB, LED_D2, OUTPUT);
	SET_PIN(DDRB, LED_D3, OUTPUT);
	SET_PIN(DDRB, LED_D4, OUTPUT);
	SET_PIN(DDRD, LED_R, OUTPUT);
	SET_PIN(DDRD, LED_G, OUTPUT);
	SET_PIN(DDRD, LED_B, OUTPUT);

	SREG |= (1 << 7);	// Global Interrupt
	adc_init();
	timers_init();

	// Empty loop
	while (42);
}
