#include "../embd.h"

void setup( void ) {

	// ==== Components ====
	SET_PIN(DDRD, LED_B, OUTPUT);		// Set PD3 to Output
	SET_PIN(DDRD, LED_R, OUTPUT);		// Set PD5 to Output
	SET_PIN(DDRD, LED_G, OUTPUT);		// Set PD6 to Output
	SET_PIN(PORTD, LED_B, HIGH);		// Set PD3 to Output
	SET_PIN(PORTD, LED_R, HIGH);		// Set PD5 to Output
	SET_PIN(PORTD, LED_G, HIGH);		// Set PD6 to Output
	uart_init();

	// ==== Interrupts ====
	SREG = (1 << 7);			// Enable interrupts globally
    UCSR0B |= (1 << RXCIE0);	// Enable Rx Compete interrupts

	// ==== LED RGB ====
	// Fast PWM mode
	// TOP = 0XFF
	// Update of OCRx at: BOTTOM
	// TOV flag set on: TOP
	// -----------------
    // Timer0 (LED_R & LED_G)
    TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1);
    TCCR0B |= (1 << CS00);
    // Timer2 (LED_B)
    TCCR2A |= (1 << WGM21) | (1 << WGM20) | (1 << COM2B1);
    TCCR2B |= (1 << CS20);

	// ==== Start Timers ====
	// No prescaler /1
	// ----------------------
	// Timer0 (LED_R & LED_G)
    TCCR0B |= (1 << CS00);
    // Timer2 (LED_B)
    TCCR2B |= (1 << CS20);
}

static inline uint8_t hex_dec( uint8_t c ) {

	if ('a' <= c)
		return (c - 'a' + 10);
	else if ('A' <= c)
		return (c - 'A' + 10);
	else
		return (c - '0');
}

void set_rgb( uint8_t * clr ) {

    OCR0B = hex_dec(clr[0]) * 10 + hex_dec(clr[1]);
    OCR0A = hex_dec(clr[2]) * 10 + hex_dec(clr[3]);
    OCR2B = hex_dec(clr[4]) * 10 + hex_dec(clr[5]);
}

int valid_colour( uint8_t * clr ) {

	for (int i = 0; clr[i]; ++i)
		if (!('0' <= clr[i] && clr[i] <= '9') &&
			!('A' <= clr[i] && clr[i] <= 'F') &&
			!('a' <= clr[i]	&& clr[i] <= 'f'))
				return 0;

	return 1;
}

void __attribute__((signal, used, externally_visible)) USART_RX_vect ( void ) {

	static uint8_t buf[8] = {0};
	static uint8_t i = 0;
	static uint8_t skip = 0;

	uint8_t c = UDR0;

	// Skip ESC — arrow keys send ESC [ A/B/C/D
	if (skip) {
		--skip;
		return;
	}

	if (c == 0x1B) {
		skip = 2;
		return;
	}

	// Enter pressed
	if (c == '\r') {

        // Print newline
        uart_tx('\r');
        uart_tx('\n');
		
		// Check valid input
		// Adjust LED colour
		if (i == 7 && buf[0] == '#' && valid_colour(buf + 1))
			set_rgb(buf + 1);

		// Reset buffer and index
		for (int j = 0; j < i; ++j)
			buf[j] = 0;
		i = 0;
	}
	
	// Backspace pressed
	else if (c == 127) {

		// Print backspace
		uart_tx('\b');
        uart_tx(' ');
        uart_tx('\b');

		// Erase buf[i] and decrement index
		buf[i] = 0;
		if (i > 0)
			--i;
	}

	// Actual input
	else if (i < 7) {

		// Print input
		uart_tx(c);

		// Record in buf
		buf[i] = c;

		// Increment index
		++i;
	}
}

int main( void ) {

	setup();

	while(42);
}
