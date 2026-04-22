#include "../embd.h"

void setup( void ) {

	// Components
	uart_init();

	// Timers
	TCCR1B |= (1 << WGM12);		// Clear Timer on Compare match (CTC)
	OCR1A = 31249;				// F_CPU / 1024 * 2 - 1

	// Interrupts
	SREG = (1 << 7);				// Enable interrupts globally
	TIMSK1 |= (1 << OCIE1A); 		// Enable interrupts specifically on Timer1
	
	// Start
	TCCR1B |= (1 << CS12)| (1 << CS10);		// prescaler /1024
}

void __attribute__(( signal, used, externally_visible )) TIMER1_COMPA_vect( void ) {

	uart_printstr("Hello World!\r\n");
}

int main( void ) {

	setup();

    // Empty loop
	while (42);
}
