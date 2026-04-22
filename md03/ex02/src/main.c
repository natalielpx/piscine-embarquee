#include "../embd.h"

void init_rgb( void ) {

	// ==== Components ====
	SET_PIN(DDRD, LED_B, OUTPUT);		// Set PD3 to Output
	SET_PIN(DDRD, LED_R, OUTPUT);		// Set PD5 to Output
	SET_PIN(DDRD, LED_G, OUTPUT);		// Set PD6 to Output

	// ==== Interrupts ====
	SREG = (1 << 7);
	TIMSK1 = (1 << OCIE1A);

	// ==== Timers ====

    // -- Wave Generation Mode  --
    // ---------------------------
    // Fast PWM mode
    // TOP = 0xFF
    // Update of OCRx at: BOTTOM
    // TOV flag set on: TOP
	// ref: datasheet pg115 & pg164
	// ---------------------------
	// Timer0 (LED_R & LED_G)
    TCCR0A |= (1 << WGM01) | (1 << WGM00);
	// Timer2 (LED_B)
    TCCR2A |= (1 << WGM21) | (1 << WGM20);
	// ---------------------------
	// CTC mode
	// TOP = OCR1A
	// Update of OCRx at: Immediate
	// TOV flag set on: MAX
	// ---------------------------
	// Timer1 (counter)
	TCCR1B |= (1 << WGM12);
	OCR1A = 245;
	// ---------------------------

	// -- Output Behaviour --
	// ----------------------
    // Clear OCnx on Compare Match
    // Set OCnx at BOTTOM
    // ref: datasheet pg113 table15-2, pg114 table15-6
    // ----------------------
    // Timer0 (LED_R & LED_G)
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
    // Timer2 (LED_B)
	TCCR2A |= (1 << COM2B1);
	// ----------------------

    // -- Start Timers --
    // ------------------
    // No prescaler /1
    // ------------------
    // Timer0 (LED_R & LED_G)
	TCCR0B |= (1 << CS00);
    // Timer2 (LED_B)
    TCCR2B |= (1 << CS20);
	// ------------------
	// Prescaler /256
	// ------------------
	// Timer1 (counter
	TCCR1B |= (1 << CS12);
	// ------------------
}

void set_rgb( uint8_t r, uint8_t g, uint8_t b ) {

	OCR0B = r;
	OCR0A = g;
	OCR2B = b;
}

void wheel( uint8_t pos ) {

	pos = 255 - pos;
	
	if (pos < 85) {
		set_rgb(255 - pos * 3, 0, pos * 3);
	}

	else if (pos < 170) {
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	}

	else {
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}

void __attribute__(( signal, used, externally_visible )) TIMER1_COMPA_vect( void ) {

	static uint8_t pos = 0;	

	wheel(pos);
	++pos;
}

int main( void ) {

	init_rgb();

	while(42);
}
