#ifndef EMBD_H
# define EMBD_H

# include <avr/io.h>
# include <util/delay.h>

# ifndef F_CPU
#  define F_CPU = 16000000UL
# endif

# define LED_D1 PB0
# define LED_D2 PB1
# define LED_D3 PB2
# define LED_D4 PB4

# define ADC_POT PC0
# define ADC_LDR PC1
# define ADC_NTC PC2

# define SWITCH_1 PD2
# define SWITCH_2 PD4

# define INPUT 0
# define OUTPUT 1

# define LOW 0
# define HIGH 1

# define FLOAT 0
# define PULL_UP 1

# define DISABLE 0
# define ENABLE 1

# define SET_FLAG 0
# define CLR_FLAG 1

# define READ_PIN(reg, bit) !!(reg & (1 << bit))
# define SET_PIN(reg, bit, val) (val ? (reg |= (1 << bit)) : (reg &= ~(1 << bit)))
# define TOGGLE_PIN(reg, bit) (reg ^= (1 << bit))

// ----- UART -----

# define BIN 2
# define DEC 10
# define HEX 16

# define MYUBRR 8   // ref: datasheet pg199 tabe20-7

void    uart_init( void );
uint8_t uart_rx( void );
void    uart_tx( char );
void    uart_print_str( const char * );
void    uart_print_hex( uint8_t );

// ----- TIMER -----

void timers_init( void );
void start_timer0( uint16_t );
void start_timer1( uint16_t );
void start_timer2( uint16_t );

// ----- I2C -----

void i2c_init( void );
void i2c_start( void );
void i2c_wait( void );
void i2c_write( unsigned char );
void i2c_read( void );
void i2c_read_NACK( void );
void i2c_stop( void );
void i2c_print_status( void );
bool i2c_check_status( uint8_t );

#endif
