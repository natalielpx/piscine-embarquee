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

void uart_init( void );
void uart_tx( char );
void uart_printstr( const char * );
void uart_printuint16( uint16_t );
char uart_rx( void );

// ----- TIMER -----
void timers_init( void );

#endif
