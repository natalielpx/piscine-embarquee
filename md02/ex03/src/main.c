#include "../embd.h"

void setup( void ) {

	// Components
	uart_init();

	// Interrupts
	SREG |= (1 << 7);				// Enable interrupts globally
	UCSR0B |= (1 << RXCIE0);			// Enable Rx Compete interrupts
}

void __attribute__((signal, used, externally_visible)) USART_RX_vect ( void ) {

	uint8_t c = UDR0;

	// Enter
	if (c == '\r') {
		uart_tx('\r');
		uart_tx('\n');
	}
	
	// Backspace
	else if (c == 127) {
		uart_tx('\b');
		uart_tx(' ');
		uart_tx('\b');
	}
	
	// Lower to Upper
	else if (('a' <= c) && (c <= 'z'))
		uart_tx(c - 'a' + 'A');

	// Upper to Lower
	else if (('A' <= c) && (c <= 'Z'))
		uart_tx(c - 'A' + 'a');

	else
		uart_tx(c);
}

int main( void ) {

	setup();

	// Emtpy loop
	while (42);
}
