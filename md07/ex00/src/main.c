#include "../embd.h"

unsigned char eeprom_read(unsigned int addr) {

	while(EECR & (1 << EEPE));	// Wait for completion of previous write
	EEAR = addr;				// Set up address register
	EECR |= (1 << EERE);		// Trigger eeprom read
	return EEDR;				// Return data
}

static inline void uart_print_printable( uint8_t c ) {

	if (32 <= c && c <= 126)
		uart_tx(c);
	else
		uart_tx(' ');
}

int main( void ) {

	uart_init();

	uint16_t addr = 0x0000;
	while (addr < E2END + 1) {

		//  Print out each address where addr % 16 == 0
		if ((addr & 0xf) == 0x0) {
			uart_print_addr(addr);
			uart_tx(' ');
		}
		
		//  Print out data (hex) in each address
		uart_print_hex(eeprom_read(addr));
		uart_tx(' ');

		//  At end of line, print out data (char) in prev 16 addresses
		if ((addr & 0xf) == 0xf) {
			uart_tx('|');
			for (int i = 15; i >= 0; --i) {
				uart_print_printable(eeprom_read(addr - i));
				uart_tx(' ');
			}
			uart_tx('|');
			if (addr != 0x3ff)
				uart_print_newline();
		}
		
		++addr;
	}
}
