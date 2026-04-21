#include "../embd.h"

int main( void ) {

	uart_init();

	while (42) {

		// Read input
		unsigned char c = uart_rx();

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
		
		// Other
	 	else
			uart_tx(c);
	}
}
