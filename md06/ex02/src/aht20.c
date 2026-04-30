#include "../embd.h"

#define AHT20_INIT_COMMAND 0xBE
#define AHT20_INIT_PARAM1 0x08
#define AHT20_INIT_PARAM2 0x00
#define AHT20_TRIG_COMMAND 0xAC
#define AHT20_TRIG_PARAM1 0x33
#define AHT20_TRIG_PARAM2 0x00

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
