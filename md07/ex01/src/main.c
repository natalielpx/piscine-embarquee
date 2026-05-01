#include "../embd.h"

unsigned char eeprom_read(unsigned int addr) {

	while(EECR & (1 << EEPE));	// Wait for completion of previous write
	EEAR = addr;				// Set up address register
	EECR |= (1 << EERE);		// Trigger eeprom read
	return EEDR;				// Return data
}

void eeprom_write( uint32_t addr, uint8_t data ) {

	while(READ_PIN(EECR, EEPE) != 0);		// Wait until EEPE becomes zero.
	while(READ_PIN(SPMCSR, SPMEN) != 0);	// 	Wait until SPMEN in SPMCSR becomes zero.
	EEAR = addr;							// 	Write new EEPROM address to EEAR (optional).
	EEDR = data;							// 	Write new EEPROM data to EEDR (optional).
	EECR = (1 << EEMPE);						// 	Write 1 to EEMPE while writing 0 to EEPE
	EECR |= (1 << EEPE);					// 	Within four clock cycles, write 1 to EEPE
}

volatile uint32_t addr = 0;
volatile uint8_t data = 0;

int get_input( void ) {

	// Handle read character
	uint8_t c = UDR0;
	static uint8_t i = 0;
	uint8_t valid = 0;
	static uint8_t buf[7] = {0};

	// Skip ESC — arrow keys (sends ESC [ A/B/C/D) and backspaces
	static uint8_t skip = 0;
	if (skip)		return (--skip, 0);
	if (c == 0x1B)	return (skip = 2, 0);
	if (c == 127)	return 0;

	// Enter pressed
	if (c == '\r') {
		uart_print_newline();			// Print newline
		valid = valid_input(buf);		// Check valid input
		if (valid) {
			addr = get_addr(buf);
			data = get_value(buf);
		}
		for (int j = 0; j < 7; ++j)
			buf[j] = 0;					// Clear buffer
		i = 0;							// Reset index
	}

	// Actual input
	else if (i < 6) {
		uart_tx(c);		// Print input
		buf[i] = c;		// Record in input
		++i;			// Increment index
	}

	return valid;
}

volatile uint8_t action = IDLE;

void __attribute__((signal, used, externally_visible)) USART_RX_vect ( void ) {

	if (get_input())
		action = READ_EEPROM;
	else
		action = IDLE;
}

int main( void ) {

	uart_init();
	SREG = (1 << 7);			// Enable interrupts globally
	UCSR0B |= (1 << RXCIE0);	// Enable Rx Compete interrupts

	while (42) {

		if (action == IDLE) continue;

		if (action == READ_EEPROM) {
			if (eeprom_read(addr) != data)
				action = WRITE_EEPROM;
			else
				action = IDLE;
		}

		if (action == WRITE_EEPROM) {

			// Write to eeprom
			eeprom_write(addr, data);

			//  Print results
			uint32_t index = 0x0000;
			while (index < 0x0400) {

				if ((index & 0b1111) == 0) {
					uart_tx('\r');
					uart_tx('\n');
					uart_print_addr(index);
					uart_tx(' ');
				}
				if (index == addr)
					uart_print_str("\x1b[31m");
				uart_print_hex(eeprom_read(index));
				uart_print_str("\x1b[0m ");

				++index;
			}

			// Reset loop
			action = IDLE;
		}
	}
}
