#include "../embd.h"

static inline int valid_hex( uint8_t c ) {

	return (('0' <= c && c <= '9') ||
			('A' <= c && c <= 'F') ||
			('a' <= c && c <= 'f'));
}

int valid_input( volatile uint8_t * cmd ) {

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

uint8_t get_value( volatile uint8_t * buf ) {
	
	// Find separator
	int index = 0;
	while (buf[index] != ' ')
		++index;

	return (hex_value(buf[index + 1]) * 16 + hex_value(buf[index + 2]));
}