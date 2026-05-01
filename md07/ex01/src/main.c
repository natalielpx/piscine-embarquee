#include "../embd.h"

unsigned char EEPROM_read(unsigned int addr) {

	// Wait for completion of previous write
	while(EECR & (1 << EEPE));

	// Set up address register
	EEAR = addr;

	// Trigger eeprom read
	EECR |= (1 << EERE);

	// Return data
	return EEDR;
}

static inline int valid_hex( uint8_t c ) {

	return (('0' <= c && c <= '9') ||
			('A' <= c && c <= 'F') ||
			('a' <= c && c <= 'f'));
}

static inline int valid_input( volatile uint8_t * cmd ) {

	// Check first characters
	if (!valid_hex(cmd[0]))	return 0;

	// Check 0 00
	if (cmd[1] == ' ') {
		if (!valid_hex(cmd[2]))	return 0;
		if (!valid_hex(cmd[3]))	return 0;
		if (cmd[4] != 0)		return 0;
		if (cmd[5] != 0)		return 0;
		if (cmd[6] != 0)		return 0;
		return 1;
	}

	// Check 00 00
	else if (valid_hex(cmd[1])) {

		if (cmd[2] == ' ') {
			if (!valid_hex(cmd[3]))	return 0;
			if (!valid_hex(cmd[4]))	return 0;
			if (cmd[5] != 0)		return 0;
			return 1;
		}

	// Check 000 00
		else if (valid_hex(cmd[2])) {

			if (cmd[0] != '1' && cmd[0] != '2' && cmd[0] != '3') return 0;
			if (cmd[3] != ' ')		return 0;
			if (!valid_hex(cmd[4]))	return 0;
			if (!valid_hex(cmd[5]))	return 0;
			if (cmd[6] != 0)		return 0;
			return 1;
		}
		else return 0;
	}
	else return 0;
}

static inline uint8_t hex_value( uint8_t c ) {

	if		('a' <= c)	return (c - 'a' + 10);
	else if	('A' <= c)	return (c - 'A' + 10);
	else				return (c - '0');
}

uint32_t get_addr( volatile uint8_t * buf ) {

	// Find separator
	int index = 0;
	while (buf[index] != ' ')
		++index;

	uint32_t addr = 0;
	uint8_t pow = 0;
	while(index > 0) {
		--index;
		addr += hex_value(buf[index]) << (4 * pow);
		++pow;
	}

	return addr;
}

uint32_t get_value( volatile uint8_t * buf ) {
	
	// Find separator
	int index = 0;
	while (buf[index] != ' ')
		++index;

	return (hex_value(buf[index + 1]) * 16 + hex_value(buf[index + 2]));
}

volatile uint32_t addr = 0;
volatile uint8_t value = 0;

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
			value = get_value(buf);
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

	if (valid) {

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
			if (EEPROM_read(addr) != value) {
				uart_print_str("write to eeprom\r\n");
				action = WRITE_EEPROM;
			}
			else {
				uart_print_str("same value, do nothing\r\n");
				action = IDLE;
			}
		}

		if (action == WRITE_EEPROM) {
			// uart_print_str("write into eeprom");

			uint32_t index = 0x0000;
			while (index < 0x0400) {

				if ((index & 0b1111) == 0) {
					uart_tx('\r');
					uart_tx('\n');
					uart_print_addr(index);
					uart_tx(' ');
				}
				uart_print_hex(EEPROM_read(index));
				uart_tx(' ');

				++index;
			}
		}
	}
}
