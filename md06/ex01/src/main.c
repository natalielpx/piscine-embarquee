#include "../embd.h"

#define READ 0b00000001
#define WRITE 0b00000000

#define AHT20_ADDR 0x38
#define AHT20_INIT_COMMAND 0xBE
#define AHT20_INIT_PARAM1 0x08
#define AHT20_INIT_PARAM2 0x00
#define AHT20_TRIG_COMMAND 0xAC
#define AHT20_TRIG_PARAM1 0x33
#define AHT20_TRIG_PARAM2 0x00

#define RETRY 3

int aht20_init( void ) {

	// Wait 40ms after power-on
	_delay_ms(40);

	// Send and confirm START condition
	i2c_start();

	// Call and confirm Slave
	i2c_write((AHT20_ADDR << 1) | WRITE);
	if (!i2c_check_status(0x18)) return 1;

	// Send Initialisation command and confirm reception
	i2c_write(AHT20_INIT_COMMAND);
	i2c_write(AHT20_INIT_PARAM1);
	i2c_write(AHT20_INIT_PARAM2);
	if (!i2c_check_status(0x28)) return 2;

	// Terminate communication
	i2c_stop();

	return (_delay_ms(10), 0);
}

int aht20_trig( void ) {

	// Send and confirm START condition
	i2c_start();

	// Call and confirm Slave
	i2c_write((AHT20_ADDR << 1) | WRITE);
	if (!i2c_check_status(0x18)) return 1;

	// Trigger measurment
	i2c_write(AHT20_TRIG_COMMAND);
	i2c_write(AHT20_TRIG_PARAM1);
	i2c_write(AHT20_TRIG_PARAM2);
	if (!i2c_check_status(0x28)) return 2;

	// Terminate communication
	i2c_stop();

	return 0;
}

int aht20_read_status( uint8_t bit ) {

	// Send and confirm START condition
	i2c_start();

	// Call and confirm Slave
	i2c_write((AHT20_ADDR << 1) | READ);
	if (!i2c_check_status(0x40)) return -1;

	// Read status
	TWCR = (1 << TWINT) | (1 << TWEN);
	i2c_wait();
	if (!i2c_check_status(0x58)) return -2;
	uint8_t status = (TWDR >> bit) & 0b00000001;

	// Terminate communication
	i2c_stop();

	return status;
}

volatile uint8_t data[7] = {0};

int i2c_recv( void ) {

	// Send and confirm START condition
	i2c_start();

	// Call and confirm Slave
	i2c_write((AHT20_ADDR << 1) | READ);
	if (!i2c_check_status(0x40)) return 1;

	// Receive data and confirm reception
	for (int bits = 0; bits < 6; ++bits) {
		i2c_read();
		data[bits] = TWDR;
	}
	i2c_read_NACK();
	data[6] = TWDR;


	// Terminate communication
	i2c_stop();

	return 0;
}

static inline void print_hex_value( char c ) {

	uart_print_hex((uint8_t)c);
}

int ard_init( void ) {

	i2c_init();
	int status = 0;
	for (int i = 0; i < RETRY; ++i) {
		aht20_init();
		status = aht20_read_status(3);
		if (status == 1)
			break;
	}
	if (status != 1)
		return 1;

	uart_init();
	
	SREG |= (1 << 7);	// Global Interrupt
	timers_init();

	return 0;
}

enum States {
	
	IDLE,
	TRIG,
	WAIT,
	WAITING,
	CHECK,
	RECV,
};

volatile enum States aht20 = IDLE;

// Trigger new conversion (every 500ms)
void __attribute__(( signal, used, externally_visible )) TIMER1_COMPA_vect( void ) {

	if (aht20 != IDLE)
		return;

	aht20 = TRIG;
}

// After conversion triggered, wait 80ms for measurement to complete
void __attribute__(( signal, used, externally_visible )) TIMER0_COMPA_vect( void ) {

	static uint8_t counter = 0;

	if (aht20 != WAITING)
		return;

	if (++counter >= 10) {
		counter = 0;
		aht20 = CHECK;
	}
}

int main( void ) {

	ard_init();

	int tries = 0;

	// Super loop
	// ref: https://en.wikibooks.org/wiki/Embedded_Systems/Super_Loop_Architecture
	while (42) {

		if (aht20 == IDLE) continue;

		// Trigger measurement
		if (aht20 == TRIG) {
			tries = 0;
			if (aht20_trig() != 0) continue;
			aht20 = WAIT;
		}

		// Wait for 80ms
		if (aht20 == WAIT) {
			SET_PIN(TIFR0, OCF0A, CLR_FLAG);	// Clear flag in Timer0
			TCNT0 = 0;							// Ensure Timer0 starts from 0
			start_timer0(1024);
			aht20 = WAITING;
		}

		// Check if measurement complete
		if (aht20 == CHECK) {
			if (aht20_read_status(7) == 0)
				aht20 = RECV;
			else {
				++tries;
				if (tries < RETRY)
					aht20 = WAIT;
				else
					aht20 = IDLE;
			}
		}

		// Receive results
		if (aht20 == RECV) {
			if (i2c_recv() != 0) continue;
			for (int i = 0; i < 7; ++i) {
				print_hex_value(data[i]);
				uart_tx(' ');
			}
			uart_tx('\r');
			uart_tx('\n');
			aht20 = IDLE;
		}
	}
}

// ref: https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf
