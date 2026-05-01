#include "../embd.h"

unsigned char eeprom_read(unsigned int addr) {

	// Wait for completion of previous write
	while(EECR & (1 << EEPE));

	// Set up address register
	EEAR = addr;

	// Trigger eeprom read
	EECR |= (1 << EERE);

	// Return data
	return EEDR;
}

int main( void ) {

	uart_init();

	uint32_t addr = 0x0000;
	while (addr < 0x0400) {

		if ((addr & 0b1111) == 0) {
			uart_tx('\r');
			uart_tx('\n');
			uart_print_addr(addr);
			uart_tx(' ');
		}
		uart_print_hex(eeprom_read(addr));
		uart_tx(' ');

		++addr;
	}
}
