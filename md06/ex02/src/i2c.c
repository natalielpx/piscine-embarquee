#include "../embd.h"

static inline uint8_t twi_prescaler( uint16_t prescaler ) {

    switch (prescaler) {
        case 4:     return 0b01;
        case 16:    return 0b10;
        case 64:    return 0b11;
        default:    return 0b00;    // Includes prescaler /1
    }
}

static inline char * status_codes( uint8_t code ) {

    switch (code) {

        case 0x00:  return "Bus error due to an illegal START or STOP condition";
        case 0x08:  return "A START condition has been transmitted";
        case 0x10:  return "A repeated START condition has been transmitted";
        case 0x18:  return "SLA+W has been transmitted; ACK has been received";
        case 0x20:  return "SLA+W has been transmitted; NOT ACK has been received";
        case 0x28:  return "Data byte has been transmitted; ACK has been received";
        case 0x30:  return "Data byte has been transmitted; NOT ACK has been received";
        case 0x38:  return "Arbitration lost in SLA+R or NOT ACK bit";
        case 0x40:  return "SLA+R has been transmitted; ACK has been received";
        case 0x48:  return "SLA+R has been transmitted; NOT ACK has been received";
        case 0x50:  return "Data byte has been received; ACK has been returned";
        case 0x58:  return "Data byte has been received; NOT ACK has been returned";
        case 0xF8:  return "No relevant state information available; TWINT = “0”";

        default:    return "Error, Code Not Found";
    }
}

#define COMM_FREQ   100000
#define PRESCALER   4
#define BIT_RATE    ((F_CPU / COMM_FREQ - 16) / 2 / PRESCALER)

void i2c_init( void ) {

    // TWI Bit Rate Register
    // ref: datasheet pg239
    TWBR = BIT_RATE;

    // TWI Control Register
    // ref: datasheet pg239
    TWCR |= (1 << TWIE);     // Enable TWI interrupts
    TWCR |= (1 << TWEN);     // Enable TWI operation and activate TWI interface

    // TWI Status Register
    // ref: datasheet pg240
    TWSR |= twi_prescaler(PRESCALER);    // Set TWI Prescaler
}

// Wait for TWINT Flag set
// May indicate:
// 1. Transmission of START condition
// 2. Transmission of SLA+W and reception of ACK/NACK
// 3. Transmission of DATA and reception of ACK/NACK
// ref: datasheet pg225
void i2c_wait( void ) {
    
    while (!(TWCR & (1 << TWINT)));
}

// Send START condition
// ref: datasheet pg225
void i2c_start( void ) {
    
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    i2c_wait();
}

// Load DATA into TWDR Register
// Clear TWINT bit in TWCR to start transmission
// ref: datasheet pg225
void i2c_write( unsigned char data ) { 
    
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    i2c_wait();
}

extern volatile uint8_t data[7];

int i2c_recv( uint8_t addr ) {

	// Send and confirm START condition
	i2c_start();

	// Call and confirm Slave
	i2c_write((addr << 1) | READ);
	if (!i2c_check_status(0x40)) return 1;

	// Receive data and confirm reception
	for (int bits = 0; bits < 7; ++bits) {
		i2c_read(bits == 6);
		data[bits] = TWDR;
	}

	// Terminate communication
	i2c_stop();

	return 0;
}

void i2c_read( uint8_t ack ) { 
    
    if (ack == ACK)
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    else
        TWCR = (1 << TWINT) | (1 << TWEN);

    i2c_wait();
}

// Transmit STOP condition
// ref: datasheet pg225
void i2c_stop( void ) {
    
    TWCR = (1 << TWINT) |(1 << TWEN) | (1 << TWSTO);
    while(TWCR & (1 << TWSTO));
}

// Check value of TWI Status Register
bool i2c_check_status( uint8_t target) {

    return ((TWSR & 0xF8) == target);
}

// Print value of TWI Status Register
void i2c_print_status( void ) { 
    
    uart_tx('0');
    uart_tx('x');
    uart_print_hex(TWSR & 0xF8);
    uart_tx(':');
    uart_tx(' ');
    uart_print_str(status_codes(TWSR & 0xF8));
    uart_tx('\r');
    uart_tx('\n');
}
