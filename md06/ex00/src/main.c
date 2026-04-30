#include "../embd.h"

#define READ 0b00000001
#define WRITE 0b00000000

#define AHT20_ADDR 0x38

void __attribute__(( signal, used, externally_visible )) TIMER1_COMPA_vect( void ) {

	// Send START condition
	i2c_start();
	i2c_print_status(); 

	// Call and confirm Slave
	i2c_write((AHT20_ADDR << 1) | WRITE);
	i2c_print_status(); 
	if (!i2c_check_status(0x18))
		return;

	// Terminate communication
	i2c_stop();
	i2c_print_status(); 
}

int main( void ) {

	SREG |= (1 << 7);	// Global Interrupt
	uart_init();
	i2c_init();
	timers_init();

	// Empty loop
	while (42);
}
