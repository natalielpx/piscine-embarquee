#include "../embd.h"

void setup() {

	// ==== Components ====
	// ---- LED D1 ----
	SET_PIN(DDRB, LED_D1, OUTPUT);		// Set PB0 to output
	// ---- Switch 1 ----
	SET_PIN(DDRD, SWITCH_1, INPUT);		// Set INT0 to input
	SET_PIN(PORTD, SWITCH_1, PULL_UP);	// Enable pull-up on INT0

	// ==== Interrupts ====
	SREG = (1 << 7);		// Enable interrupts globally
	// ---- Switch 1 ----
	// External Interrupt Mask Register
	EIMSK = (1 << INT0);
	// External Interrupt Control Register A
	// ISC01 = 0
	// ISC00 = 1
	// Any logical change of INT0 generates an interrupt request
	// ref: datasheet pg80 table13-2
	EICRA = (1 << ISC00);
	// timer
	TIMSK1 = (1 << OCIE1A);

	// ==== Timers ====
	// ---- Timer1 ----
    TCCR1B |= (1 << WGM12);		// CTC Mode; Updates OCRx immediately; TOV set on MAX
	OCR1A = 1249;				// TOP value (20ms when prescaler /256)
	// Timer1 not started yet
}

volatile uint8_t prev = 0;
volatile uint8_t curr = 0;
volatile uint8_t debounced = 0;

void __attribute__(( signal, used, externally_visible )) TIMER1_COMPA_vect( void ) {

	// Stop Timer1
	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));

	SET_PIN(EIFR, INTF0, CLR_FLAG);		// Discard all accumulated bounces
	SET_PIN(EIMSK, INT0, ENABLE);		// Re-enable interrupt

	debounced = 1;
}

void __attribute__(( signal, used, externally_visible )) INT0_vect( void ) {

	if (!debounced) {

		// Disable interrupts for the time being
		SET_PIN(EIMSK, INT0, DISABLE);
	
		curr = !curr;
	
		// Reset and start Timer1
		SET_PIN(TIFR1, OCF1A, CLR_FLAG);	// Clear flag in Timer1
		TCNT1 = 0;							// Ensure Timer1 starts from 0
		TCCR1B |= (1 << CS12);				// Start Timer1 with prescaler /256
	}

	else if (prev != curr) {

		TOGGLE_PIN(PORTB, LED_D1);

		// Update variables
		prev = curr;
		debounced = 0;
	}
}

int main( void ) {

	setup();

	// Empty loop
	while (42);
}

// other references:
// debouncing on interrupts: https://gammon.com.au/interrupts
// millis(): https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/wiring.c